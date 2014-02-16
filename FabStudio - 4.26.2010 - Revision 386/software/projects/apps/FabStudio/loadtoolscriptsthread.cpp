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
#include "loadtoolscriptsthread.h"
#include "materialspanel.h"

#include <QDir>

static const char* kToolScriptFileFilter = "*.xml";

LoadToolScriptsThread::LoadToolScriptsThread(MaterialsPanel* panel)
    : panel_(panel) {
}

void LoadToolScriptsThread::run() {
  QStringList directories;
  directories.append(QDir::currentPath());

  while (!directories.isEmpty()) {

    // Pop the first directory from the worklist
    QDir directory = QDir(directories.first());
    directories.removeFirst();

    // Find the list of all object files in this directory
    QStringList files
        = directory.entryList(QStringList(kToolScriptFileFilter),
                              QDir::Files | QDir::NoSymLinks);

    // Pass this list to the load objects panel
    panel_->addToolScripts(directory.absolutePath(), files);

    // Add subdirectories to worklist
    QStringList folders = directory.entryList(QStringList("*"), QDir::Dirs|QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i) {
      directories.append(directory.absolutePath() + QDir::separator() + folders.at(i));
    }
  }
}
