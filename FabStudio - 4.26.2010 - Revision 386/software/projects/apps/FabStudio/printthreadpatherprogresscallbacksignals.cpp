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
#include "printthreadpatherprogresscallbacksignals.h"
#include <QTextStream>

#include "shared/pather/pathstack.h"
#include "shared/pather/pathslice.h"
#include "shared/pather/pathslicematerial.h"
#include "shared/pather/pathsliceregion.h"
#include "shared/pather/path.h"



PrintThreadPatherProgressCallbackSignals::PrintThreadPatherProgressCallbackSignals()
    : QObject(0) {
}

PrintThreadPatherProgressCallbackSignals::~PrintThreadPatherProgressCallbackSignals() {
}

void PrintThreadPatherProgressCallbackSignals::setNumberOfSlices(int count) {
  emit signalSetTaskRange(0, count-1);
  emit signalSetTaskProgress(0);

  QString title;
  QTextStream(&title) << "Pathing " << count << " slices";
  emit signalSetTaskTitle(title);
}

void PrintThreadPatherProgressCallbackSignals::sliceHasBeenComputed(int index, const PathSlice* slice) {
  emit signalSetTaskProgress(index);

  // Emit each of the paths from this slice
  const PathSliceMaterialsArray& materials = slice->getPathSliceMaterials();
  for (int material = 0; material < materials.size(); ++material) {
    const PathSliceRegionArray& regions = materials[material]->getRegions();
    for (int region = 0; region < regions.size(); ++region) {
      const PathArray& paths = regions[region]->getPaths();
      for (int path = 0; path < paths.size(); ++path) {
        emit signalAddRenderablePath(materials[material]->materialID(), new Path(paths[path]));
      }
    }
  }
}

void PrintThreadPatherProgressCallbackSignals::encounteredIssue(QString description) {
  emit signalEncounteredIssue("Pather Error:  " + description);
}

void PrintThreadPatherProgressCallbackSignals::finishedPathing(const PathStack& path_stack) {
}