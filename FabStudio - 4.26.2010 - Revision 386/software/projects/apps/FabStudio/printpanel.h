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
#ifndef PRINTPANEL_H
#define PRINTPANEL_H

#include <QtGui/QDockWidget>
#include <QSharedPointer>
#include "shared/pather/patherimplementation.h"

namespace Ui {
    class PrintPanel;
}

class MainWindow;
class ToolScript;
class PrintThread;
class Path;
class RenderablePath;

/**
 * The docking panel that allows the user to print the current scene.
 */
class PrintPanel : public QDockWidget {
  Q_OBJECT

public:
  PrintPanel(MainWindow *main_window,
             QWidget *parent = 0);
  ~PrintPanel();

  /**
   * Returns the dialog to its initial state
   */
  void resetDialog();

  /**
   * Returns true if there is an active printing job.  When a print job is active, the
   * main window should not be usable for interaction.
   */
  bool isPrintInProgress() const;

  /**
   * Turns on or off all "advanced" features of the program.  This is used to turn FabStudio
   * from a simple, general and fail-proof tool into a fully-featured 3d printing program
   * or back again.
   */
  void useAdvancedMode(bool enable);

  /**
   * Invoked by the main window to render any preview graphics this panel has generated
   */
  void glRender();

private:
  void clearPreviewPaths();

  void glRenderPath(const RenderablePath* path);

protected:
  void changeEvent(QEvent *e);

public slots:
  void onChangedMaterials(QStringList compatible_script_names);

signals:

  /**
   * Emitted when the OpenGL 3d scene should be redrawn
   */
  void glRepaint();

  /**
   * Emitted with a 'true' when the scene printing begins, 'false'
   * when it ends.
   */
  void printingStatus(bool);

private slots:
  void on_sendToPrinter_clicked();
  void on_modifyPrintSettings_clicked();
  void on_savePrintJob_clicked();
  void on_abandonPrintJob_clicked();
  void on_toolCombo_currentIndexChanged(QString );
  void on_print_clicked();

  void addPrintLogEntry(QString entry);
  void finishedFabFile(QString fab_file);
  void finishedPrintingThread();

  void setProgressGroupBoxTitle(QString text);

  /**
   * This function is called whenever a path has been created by the print job.
   */
  void addRenderablePath(QColor color, Path* path);

  /**
   * Notifies the listener what slice is being computed; this is just to give
   * some visual feedback.
   * @param z Processing plane height
   */
  void setProcessingPlane(float z);

private:
  Ui::PrintPanel *m_ui;

  MainWindow *main_window_;

  ToolScript* tool_script_;

  bool is_print_in_progress_;

  /// Text of the fab file after compilation has finished
  QString fab_file_;

  /// Record of output during printing
  QString print_log_;

  /// Whether or not the print thread should attempt to make support material
  bool generate_support_material_;

  /// Which kind of pather to use
  PatherImplementation pather_implementation_;

  /// The thread that executes printing
  PrintThread* print_thread_;

  /// Current set of paths that were created by the print job.  This member use used
  /// soley for the purpose of rendering a preview; the actual print data will appear
  /// in the fab file that the printing thread generates.  These paths are allocated
  /// by the PrintThreadPatherProgressCallbackSignals class, but should be freed
  /// by this class once it is finished.
  QList<RenderablePath*> preview_paths_;

  /// Improves rendering
  int gl_draw_list_;
  int paths_in_draw_list_;

  /// The height of the plane being processed
  float processing_plane_z_;

private:
  void connectPrintThreadSignals();
  void disconnectPrintThreadSignals();
};

#endif // PRINTPANEL_H
