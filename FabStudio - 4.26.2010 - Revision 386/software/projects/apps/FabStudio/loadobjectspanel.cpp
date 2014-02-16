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
#include "loadobjectspanel.h"
#include "ui_loadobjectspanel.h"
#include "scanforobjectfilesthread.h"
#include "printableobject.h"
#include <common/confirm.h>
#include <common/todo.h>

#include <QThread>
#include <QDir>

#include <QDragEnterEvent>
#include <QDropEvent>

#include <QFileDialog>


/// This constant sets the maximum number of folders that will be watched
/// for changes at the same time.  This must be a relatively small number
/// because operating systems limit the number of simultaneous open file
/// descriptors per program, and this could interfere with other
/// operations.  The limit is something like 128 on OSX but it doesn't
/// hurt to be conservative.
static const int kMaxWatchedFolders = 8;


LoadObjectsPanel::LoadObjectsPanel(QWidget *parent) :
    QDockWidget(parent),
    m_ui(new Ui::LoadObjectsPanel),
    scan_for_object_files_thread_(0)
{
    m_ui->setupUi(this);

    connect(&object_folders_watcher_, SIGNAL(directoryChanged(const QString&)),
            this, SLOT(folderChanged(const QString&)));

    // Fake a click on the "re-scan" button to kick things off
    on_rescan_clicked();

    todo("kwg8", "enable the 'browse' button in the LoadObjectsPanel");
    m_ui->browse->hide();
}

LoadObjectsPanel::~LoadObjectsPanel() {

  { // Terminate the async loading thread
    if (scan_for_object_files_thread_) {
      scan_for_object_files_thread_->exit(0);
      scan_for_object_files_thread_->wait(1000);
      delete scan_for_object_files_thread_;
      scan_for_object_files_thread_ = 0;
    }
  }

  delete m_ui;
}


bool LoadObjectsPanel::isSourceOfEvent(QDragEnterEvent* event) const {
  confirm (event) else return false;
  return event && event->source() == m_ui->objectsList;
}

bool LoadObjectsPanel::getDraggedObjectFile(QDropEvent* event, QString* file_path) {
  confirm(event && file_path) else return false;
  if (event->source() != m_ui->objectsList) return false;
  QList<QListWidgetItem*> items = m_ui->objectsList->selectedItems();
  if (items.isEmpty()) return false;
  const QListWidgetItem* front = items.front();
  *file_path = front->toolTip();
  return true;
}

void LoadObjectsPanel::useAdvancedMode(bool enable) {
}

void LoadObjectsPanel::changeEvent(QEvent *e) {
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void LoadObjectsPanel::addDiscoveredObjectFiles(const QString& path, const QStringList& files) {
  if (files.isEmpty()) return;

  QMutexLocker lock(&object_files_mutex_);

  // Add the number of files to the map
  objects_per_folder_.insert(path, files.size());
  if (objects_per_folder_.size() > kMaxWatchedFolders) {
    // eliminate the smallest one
    QString smallest_key = objects_per_folder_.begin().key();
    int smallest = objects_per_folder_.begin().value();
    QMap<QString, int>::iterator i = objects_per_folder_.begin();
    for (; i != objects_per_folder_.end(); ++i) {
      if (i.value() < smallest) {
        smallest_key = i.key();
        smallest = i.value();
      }
    }
    if (0 != QString::compare(smallest_key, path, Qt::CaseSensitive)) {
      // We removed an old path
      object_folders_watcher_.removePath(smallest_key);
      objects_per_folder_.remove(smallest_key);
    } else {
      objects_per_folder_.remove(path);
    }
  }
  if (objects_per_folder_.contains(path)) {
    object_folders_watcher_.addPath(path);
  }

  QString filter = m_ui->fileNameTypeahead->text();
  for (int i = 0; i < files.size(); ++i) {
    const QString& file_name = files.at(i);

    // Add the object to our internal list
    QString object_file_path = path + QDir::separator() + file_name;
    object_files_.append(object_file_path);

    // Only add files with names that match the current filter to the display list
    if (filter.isEmpty() || file_name.contains(filter, Qt::CaseInsensitive)) {
      QListWidgetItem* item = new QListWidgetItem(file_name, m_ui->objectsList);
      item->setToolTip(object_file_path);
      m_ui->objectsList->addItem(item);
    }
  }
}


void LoadObjectsPanel::finishedScanningForObjectFiles() {
  delete scan_for_object_files_thread_;
  scan_for_object_files_thread_ = 0;
  m_ui->rescan->setEnabled(true);
}


void LoadObjectsPanel::folderChanged(const QString& path) {
  if (scan_for_object_files_thread_) {
    // scanning has not finished; don't update otherwise we start
    // adding files like crazy.
    todo("kwg8", "re-scan changed directory while scan for object files thread is still running");
    return;
  }

  ScanForObjectFilesThread::scanDirectory(path,
                                          object_files_,
                                          this);
  
  // remove any files that disappeared
  bool file_deleted = false;
  for (int i = object_files_.size() - 1; i >= 0; --i) {
    const QString& object_file = object_files_.at(i);
    if (object_file.startsWith(path, Qt::CaseSensitive) &&
        !QFile::exists(object_file)) {
      object_files_.removeAt(i);
      file_deleted = true;
    } 
  }

  if (file_deleted) {
    // we have to re-sync the list
    on_fileNameTypeahead_textChanged(m_ui->fileNameTypeahead->text());
  }
}


void LoadObjectsPanel::on_fileNameTypeahead_textChanged(QString text) {
  QMutexLocker lock(&object_files_mutex_);

  m_ui->objectsList->clear();

  QString filter = m_ui->fileNameTypeahead->text();
  for (int i = 0; i < object_files_.size(); ++i) {
    const QString& object_file_path = object_files_.at(i);

    QString file_name = object_file_path;
    file_name.remove(0, file_name.lastIndexOf(QDir::separator())+1);

    if (filter.isEmpty() || file_name.contains(filter, Qt::CaseInsensitive)) {
      QListWidgetItem* item = new QListWidgetItem(file_name, m_ui->objectsList);
      item->setToolTip(object_file_path);
      m_ui->objectsList->addItem(item);
    }
  }
}

void LoadObjectsPanel::on_clearFileNameTypeahead_clicked() {
  m_ui->fileNameTypeahead->clear();
}

void LoadObjectsPanel::on_rescan_clicked() {
  // Disable this button to prevent multiple clicks
  m_ui->rescan->setEnabled(false);

  confirm (!scan_for_object_files_thread_) else return;

  // Stop watching all of the old folders
  foreach (const QString& folder, objects_per_folder_.keys()) {
    object_folders_watcher_.removePath(folder);
  }
  objects_per_folder_.clear();
  m_ui->objectsList->clear();

  // Establish a thread that searches the disk for object files and
  // lists them in the loading panel.  Connecting the finished() signal
  // allows the memory to be freed once it terminates.
  scan_for_object_files_thread_ = new ScanForObjectFilesThread(this);
  scan_for_object_files_thread_->start(QThread::LowPriority);
  connect(scan_for_object_files_thread_, SIGNAL(finished()), this, SLOT(finishedScanningForObjectFiles()), Qt::QueuedConnection);
}

void LoadObjectsPanel::on_browse_clicked() {
  // Let users look for a file directly
  QString object_file_path =
      QFileDialog::getOpenFileName(this,
                                   QString("Open Object File"),
                                   QDir::currentPath(),
                                   PrintableObject::loadableFileFilterStrings().join(";;"));

  // Discard if the dialog was canceled
  if (object_file_path.isEmpty()) return;

  // Add this file to the list
  if (!object_files_.contains(object_file_path)) {
    object_files_.append(object_file_path);
  }
  QString file_name = object_file_path;
  file_name.remove(0, file_name.lastIndexOf(QDir::separator())+1);
  m_ui->fileNameTypeahead->setText(file_name);
}
