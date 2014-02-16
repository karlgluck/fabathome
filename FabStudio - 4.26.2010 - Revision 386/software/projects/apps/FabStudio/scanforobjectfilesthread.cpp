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
#include "scanforobjectfilesthread.h"
#include "loadobjectspanel.h"
#include "printableobject.h" // for loadable file filters

#include <QDir>

ScanForObjectFilesThread::ScanForObjectFilesThread(LoadObjectsPanel* panel) : panel_(panel) {
}

void ScanForObjectFilesThread::run() {
  QStringList directories;
  directories.append(QDir::currentPath());
  directories.append(QDir::homePath());

  while (!directories.isEmpty()) {

    // Pop the first directory from the worklist
    QDir directory = QDir(directories.first());
    directories.removeFirst();

    // Find the list of all object files in this directory
    QStringList files
        = directory.entryList(PrintableObject::loadableFileFilterExtensions(),
                              QDir::Files | QDir::NoSymLinks);

    // Pass this list to the load objects panel
    panel_->addDiscoveredObjectFiles(directory.absolutePath(), files);

    // Add subdirectories to worklist
    QStringList folders = directory.entryList(QStringList("*"), QDir::Dirs|QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i) {
      directories.append(directory.absolutePath() + QDir::separator() + folders.at(i));
    }
  }
}


void ScanForObjectFilesThread::scanDirectory(const QString& path,
                                             const QStringList& files_to_ignore,
                                             LoadObjectsPanel* panel) {

  QDir directory = QDir(path);
  QString absolute_path = directory.absolutePath();

  // Find the list of all object files in this directory
  QStringList files
      = directory.entryList(PrintableObject::loadableFileFilterExtensions(),
                            QDir::Files | QDir::NoSymLinks);

  // Remove any entries that are already in the list
  for (int i = files.size() - 1; i >= 0; --i) {
    QString object_file_path = absolute_path + QDir::separator() + files.at(i);
    if (files_to_ignore.contains(object_file_path, Qt::CaseSensitive)) {
      files.removeAt(i);
    }
  }

  // Add the list
  panel->addDiscoveredObjectFiles(absolute_path, files);
}
