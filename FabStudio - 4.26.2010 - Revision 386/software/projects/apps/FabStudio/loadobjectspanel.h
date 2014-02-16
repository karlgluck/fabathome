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
#ifndef LOADOBJECTSPANEL_H
#define LOADOBJECTSPANEL_H

#include <QtGui/QDockWidget>
#include <QMutex>
#include <QListWidget>
#include <QFileSystemWatcher>

namespace Ui {
    class LoadObjectsPanel;
}

class ScanForObjectFilesThread;

class LoadObjectsPanel : public QDockWidget {
  Q_OBJECT

public:
  LoadObjectsPanel(QWidget *parent = 0);
  ~LoadObjectsPanel();

  /**
   * Returns 'true' if the load objects panel's object list is the source of the
   * drag-and-drop event.
   */
  bool isSourceOfEvent(QDragEnterEvent* event) const;

  /**
   * Returns the path of the object file that was dragged to the main window if
   * this event was generated by the user performing that action.  If this event
   * was not created by the user dragging an object file to the main window,
   * this method returns 'false' and does not modify file_path.
   */
  bool getDraggedObjectFile(QDropEvent* event, QString* file_path);

  /**
   * Turns on or off all "advanced" features of the program.  This is used to turn FabStudio
   * from a simple, general and fail-proof tool into a fully-featured 3d printing program
   * or back again.
   */
  void useAdvancedMode(bool enable);

protected:
  void changeEvent(QEvent *e);

private:
  Ui::LoadObjectsPanel *m_ui;

private slots:
  void on_browse_clicked();
  void on_rescan_clicked();
  void finishedScanningForObjectFiles();
  void folderChanged(const QString& path);
  void on_clearFileNameTypeahead_clicked();
  void on_fileNameTypeahead_textChanged(QString );

public:
  void addDiscoveredObjectFiles(const QString& path, const QStringList& files);

private:

  /// List of object files that can be used in the workspace
  QStringList object_files_;

  /// Used to share access to the object files list between the
  /// GUI thread and the one that's discovering files on disk
  QMutex object_files_mutex_;

  /// Asynchronous thread that reads the disk looking for files to load
  ScanForObjectFilesThread* scan_for_object_files_thread_;

  /// Observes changes in some number of folders that have the most objects
  /// and updates the listing of objects in that folder when it changes
  QFileSystemWatcher object_folders_watcher_;
  QMap<QString, int> objects_per_folder_;
};

#endif // LOADOBJECTSPANEL_H