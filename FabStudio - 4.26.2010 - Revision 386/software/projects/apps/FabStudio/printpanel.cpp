/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2010, Karl Gluck (kwg8@cornell.edu)

 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the Fab@Home nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNERS OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\*---------------------------------------------------------------------------*/
#include "printpanel.h"
#include "ui_printpanel.h"

#include "shared/slicer/slicer.h"
#include "shared/toolscript/toolscript.h"
#include "shared/toolscript/toolscriptsetting.h"
#include "shared/toolscript/toolscripttool.h"
#include "shared/pather/pather.h"
#include "renderablepath.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <qgl.h>
#include <QProcess> // for starting the external FabInterpreter to drive the print
#include <QDebug>   // for printing log entries
#include <QSettings> // needed to get location of FAB2.EXE for the interpreter

#include "mainwindow.h"

#include "common/confirm.h"
#include "common/todo.h"

#include "printthread.h"
#include "modifytoolandprintersettingsdialog.h"

static const QString kFabInterpreterProgramSettingKey = "fabinterpreter";
static const QString kFabInterpreterDefault = "fab2.exe";

/// Every time this many renderable paths are added to the preview paths by
/// an executing tool script, the print panel will recreate its open-GL
/// draw list.
static const int kRemakeDrawListPathsInterval = 50;


PrintPanel::PrintPanel(MainWindow* main_window, QWidget *parent) :
    QDockWidget(parent),
    m_ui(new Ui::PrintPanel),
    main_window_(main_window),
    tool_script_(0),
    is_print_in_progress_(false),
    generate_support_material_(true),
    pather_implementation_(kDefaultPatherImplementation),
    print_thread_(0),
    gl_draw_list_(-1) {

  m_ui->setupUi(this);

  resetDialog();
}


PrintPanel::~PrintPanel() {
  clearPreviewPaths();
  //delete tool_script_; // DONT delete tool_script_; it is the property of the materials panel
  delete m_ui;
}


void PrintPanel::resetDialog() {

  clearPreviewPaths();

  m_ui->progressGroupBox->setTitle("Working...");
  fab_file_.clear();
  print_log_.clear();

  // These buttons are only enabled once the print job has been generated
  m_ui->sendToPrinter->setEnabled(false);
  m_ui->savePrintJob->setEnabled(false);

  // Erase all of the text-output
  m_ui->textToolScriptDescription->clear();

  // Restore stacked widgets & tabs to their default locations
  m_ui->stackedWidget->setCurrentWidget(m_ui->pageBeforePrint);

  // Reset plane height
  processing_plane_z_ = 0.0f;

  // Force an update
  m_ui->toolCombo->setCurrentIndex(m_ui->toolCombo->currentIndex());

  // No longer printing anything
  if (is_print_in_progress_) {
    emit printingStatus(false);
    is_print_in_progress_ = false;
  }
}


bool PrintPanel::isPrintInProgress() const {
  return is_print_in_progress_;
}


void PrintPanel::useAdvancedMode(bool enable) {
  // When advanced mode is disabled, the complex functions can't be accessed
  // since these buttons disappear
  m_ui->modifyPrintSettings->setVisible(enable);
  m_ui->savePrintJob->setVisible(enable);
}

void PrintPanel::glRender() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPushMatrix();
  glLoadIdentity();
  glPushAttrib(GL_LIGHTING);
  glDisable(GL_LIGHTING);

  if ((preview_paths_.size() - paths_in_draw_list_) < kRemakeDrawListPathsInterval &&
      gl_draw_list_ != -1 && glIsList(gl_draw_list_)) {
    glCallList(gl_draw_list_);

    // Draw all the paths that aren't yet in the call list
    for (int i = paths_in_draw_list_; i < preview_paths_.size(); ++i) {
      glRenderPath(preview_paths_.at(i));
    }

  } else {
    if (glIsList(gl_draw_list_)) glDeleteLists(gl_draw_list_, 1);

    gl_draw_list_ = glGenLists(1);
    paths_in_draw_list_ = preview_paths_.size();
    glNewList(gl_draw_list_, GL_COMPILE_AND_EXECUTE);

    // Render all of the paths with red as the start point,
    // green as the end point and fading from one to the other
    // along the length.
    foreach (const RenderablePath* path, preview_paths_) {
      glRenderPath(path);
    }

    glEndList();
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBegin(GL_TRIANGLES);

  // Draw the processing plane
//  glColor4f(1.0f, 0.0f, 0.0f, 0.1f);
//  glVertex3f(0.0f, 0.0f, processing_plane_z_);
//  glVertex3f(300.0f, 0.0f, processing_plane_z_);
//  glVertex3f(0.0f, 300.0f, processing_plane_z_);
//  glVertex3f(300.0f, 0.0f, processing_plane_z_);
//  glVertex3f(0.0f, 300.0f, processing_plane_z_);
//  glVertex3f(300.0f, 300.0f, processing_plane_z_);

  glEnd();

  glDisable(GL_BLEND);


  glPopAttrib();
  glPopMatrix();
}

void PrintPanel::clearPreviewPaths() {
  if (gl_draw_list_ != -1 && glIsList(gl_draw_list_)) { glDeleteLists(gl_draw_list_, 1); gl_draw_list_ = -1; }

  foreach (const RenderablePath* path, preview_paths_) {
    delete path;
  }
  preview_paths_.clear();
}

void PrintPanel::glRenderPath(const RenderablePath* path) {
  glBegin(GL_LINE_STRIP);
  const QVector<FAHVector3>& points = path->path->points();
  int vertex = 0;
  float r = path->color.red() / 255.0f,
        g = path->color.green() / 255.0f,
        b = path->color.blue() / 255.0f;
  glColor4f(r,g,b,0.5f);

  //float r_increment = r / points.size();
  //float g_increment = g / points.size();
  //float b_increment = b / points.size();

  foreach (const FAHVector3& v, points) {

// this doesn't work on MacBook display drivers
//    glColor3f(r_increment * vertex,
//              g_increment * vertex,
//              b_increment * vertex);

    glVertex3d(v.x, v.y, v.z);
    ++vertex;
  }
  glEnd();
}


void PrintPanel::changeEvent(QEvent *e) {
  QDockWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    m_ui->retranslateUi(this);
    break;
  default:
    break;
  }
}


void PrintPanel::onChangedMaterials(QStringList compatible_script_names) {
  m_ui->toolCombo->clear();
  foreach (QString ts, compatible_script_names) {
    m_ui->toolCombo->addItem(ts);
  }
}



void PrintPanel::on_print_clicked() {

  // The printing thread should never exist at this point
  confirm(!print_thread_);
  emit printingStatus(true);
  is_print_in_progress_ = true;

  m_ui->masterProgressBar->setValue(0);
  m_ui->taskProgressBar->setValue(0);

  print_thread_ = new PrintThread(main_window_,
                                  tool_script_,
                                  pather_implementation_,
                                  generate_support_material_);
  connectPrintThreadSignals();
  print_thread_->start();

  show();

  m_ui->stackedWidget->setCurrentWidget(m_ui->pageDuringPrint);

  emit glRepaint();
}




void PrintPanel::on_toolCombo_currentIndexChanged(QString toolscript_name) {

  resetDialog();

  if (toolscript_name.isEmpty()) return;

  // update the contents of the fields
  tool_script_ = main_window_->getToolScriptByName(toolscript_name);
  confirm (tool_script_) else return;
  m_ui->textToolScriptDescription->setPlainText(tool_script_->description());

}


void PrintPanel::addPrintLogEntry(QString entry) {
  QTextStream(&print_log_) << entry << "\n";
  qDebug() << entry << "\n";
}



void PrintPanel::finishedFabFile(QString fab_file) {
  paths_in_draw_list_ = -kRemakeDrawListPathsInterval;
  m_ui->progressGroupBox->setTitle("Finished.");
  fab_file_ = fab_file;

  // Ensure all the progress bars are full
  m_ui->masterProgressBar->setRange(0, 1);
  m_ui->masterProgressBar->setValue(1);
  m_ui->taskProgressBar->setRange(0, 1);
  m_ui->taskProgressBar->setValue(1);

  // Enable the post-print buttons
  m_ui->sendToPrinter->setEnabled(true);
  m_ui->savePrintJob->setEnabled(true);
}


void PrintPanel::finishedPrintingThread() {
  delete print_thread_;
  print_thread_ = 0;
}

void PrintPanel::setProgressGroupBoxTitle(QString text) {
  m_ui->progressGroupBox->setTitle(text);
}

void PrintPanel::addRenderablePath(QColor color, Path* path) {
  processing_plane_z_ = path->points().first().z;
  preview_paths_.append(new RenderablePath(color,path));
  emit glRepaint();
}

void PrintPanel::setProcessingPlane(float z) {
  processing_plane_z_ = z;
  emit glRepaint();
}

void PrintPanel::on_abandonPrintJob_clicked() {

  // erase the print thread, if it is valid
  if (print_thread_) {
    disconnectPrintThreadSignals();
    print_thread_->exit(0);
    print_thread_->wait(100);
    if (print_thread_->isFinished()) {
      delete print_thread_;
    } else {
      // This is a problem... we'll leak memory...but there
      // is really no way to avoid it that I can see.
    }
    print_thread_ = 0;
  }

  // go back to the first screen
  resetDialog();
}


void PrintPanel::on_savePrintJob_clicked() {

  // Save results to a file on disk
  QString script_file_name =
      QFileDialog::getSaveFileName(main_window_,
                                   QString("Save Print Job to Disk"),
                                   QDir::currentPath(),
                                   QString("Fab File (*.fab)"));

  // Discard if the dialog was canceled
  if (script_file_name.isEmpty()) return;

  // Write out the file (including any manual changes)
  QFile file(script_file_name);
  if (file.open(QFile::WriteOnly|QFile::Text)) {
    QTextStream(&file) << fab_file_;
  }
  file.close();
}


void PrintPanel::on_modifyPrintSettings_clicked() {

  // Display the parameter-modification panel
  ModifyToolAndPrinterSettingsDialog* dialog
      = new ModifyToolAndPrinterSettingsDialog(tool_script_,
                                               generate_support_material_,
                                               pather_implementation_,
                                               main_window_);

  dialog->exec();

  // Read back settings
  generate_support_material_ = dialog->generateSupportMaterial();
  pather_implementation_ = dialog->patherImplementation();
}


void PrintPanel::connectPrintThreadSignals() {
  connect(print_thread_, SIGNAL(finished()), this, SLOT(finishedPrintingThread()), Qt::QueuedConnection);
  connect(print_thread_, SIGNAL(setMasterProgress(int)), m_ui->masterProgressBar, SLOT(setValue(int)), Qt::QueuedConnection);
  connect(print_thread_, SIGNAL(setMasterRange(int,int)), m_ui->masterProgressBar, SLOT(setRange(int,int)), Qt::QueuedConnection);
  connect(print_thread_, SIGNAL(setTaskProgress(int)), m_ui->taskProgressBar, SLOT(setValue(int)), Qt::QueuedConnection);
  connect(print_thread_, SIGNAL(setTaskRange(int,int)), m_ui->taskProgressBar, SLOT(setRange(int,int)), Qt::QueuedConnection);
  connect(print_thread_, SIGNAL(logEntry(QString)), this, SLOT(addPrintLogEntry(QString)), Qt::QueuedConnection);
  connect(print_thread_, SIGNAL(setTaskTitle(QString)), this, SLOT(setProgressGroupBoxTitle(QString)), Qt::QueuedConnection);
  connect(print_thread_, SIGNAL(finishedFabFile(QString)), this, SLOT(finishedFabFile(QString)), Qt::QueuedConnection);
  connect(print_thread_, SIGNAL(addRenderablePath(QColor, Path*)), this, SLOT(addRenderablePath(QColor, Path*)), Qt::QueuedConnection);
  connect(print_thread_, SIGNAL(setProcessingPlane(float)), this, SLOT(setProcessingPlane(float)), Qt::QueuedConnection);
}


void PrintPanel::disconnectPrintThreadSignals() {
  print_thread_->disconnect();/*
  disconnect(this, SLOT(finishedPrintingThread()));
  disconnect(m_ui->masterProgressBar, SLOT(setValue(int)));
  disconnect(m_ui->masterProgressBar, SLOT(setRange(int,int)));
  disconnect(m_ui->taskProgressBar, SLOT(setValue(int)));
  disconnect(m_ui->taskProgressBar, SLOT(setRange(int,int)));
  disconnect(this, SLOT(addPrintLogEntry(QString)));
  disconnect(this, SLOT(setProgressGroupBoxTitle(QString)));
  disconnect(this, SLOT(finishedFabFile(QString)));
  disconnect(this, SLOT(addRenderablePath(QColor, Path*)));
  disconnect(this, SLOT(setProcessingPlane(float)));*/
}


void PrintPanel::on_sendToPrinter_clicked() {
  QProcess process;

  // Save out the fab file to a temporary location
  QString temporary_fab_file_name;
  QTextStream(&temporary_fab_file_name) << "print-job-" << qrand() << ".fab";

  QFile file(temporary_fab_file_name);
  if (file.open(QFile::WriteOnly|QFile::Text)) {
    QTextStream(&file) << fab_file_;
  }
  file.close();


  todo("kwg8", "FAB2.EXE is win32-specific!  Need to change this when porting the interpreter to Qt");
  QSettings settings;
  QString fab2 = settings.value(kFabInterpreterProgramSettingKey,
                                kFabInterpreterDefault).toString();

  QFileInfo fab_file_info(temporary_fab_file_name);
  QFileInfo config_file_info(tool_script_->printerName() + ".config");

  QString fab_file_argument(fab_file_info.canonicalFilePath());
  if (fab_file_argument.startsWith('"', Qt::CaseInsensitive)) {
    // turn "c:\path\etc" into <c:\path\etc>
    fab_file_argument.replace(0, 1, '<');
    fab_file_argument.replace(fab_file_argument.length() - 1, 1, '>');
  } else {
    fab_file_argument.prepend('<');
    fab_file_argument.append('>');
  }
  fab_file_argument.prepend("--fabfile=");
  QString config_file_argument(config_file_info.canonicalFilePath());
  if (config_file_argument.startsWith('"', Qt::CaseInsensitive)) {
    config_file_argument.replace(0, 1, '<');
    config_file_argument.replace(fab_file_argument.length() - 1, 1, '>');
  } else {
    config_file_argument.prepend('<');
    config_file_argument.append('>');
  }
  config_file_argument.prepend("--config=");

  do {

    //FAB2 --config=<C:\thisisthefilepath\to\config\file> --fabfile=<C:\thisisthefilepath\to\the\fab\file.fab>

    process.start(fab2, QStringList() << config_file_argument << fab_file_argument);

    if (!process.waitForStarted()) {
      // the program couldn't start; ask the user where the interpreter is...
      QString fab2 =
          QFileDialog::getOpenFileName(this,
                                       QString("Missing FabInterpreter!"),
                                       QDir::currentPath(),
                                       "FabInterpreter (fab2.exe)");
      if (fab2.isEmpty()) {
        return;
      }

      // save to permanent storage
      settings.setValue(kFabInterpreterProgramSettingKey,
                        fab2);

      // retry launching the file
      continue;
    }

  } while (false);

  process.waitForFinished(-1);

}
