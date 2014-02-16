/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009, Karl Gluck (kwg8@cornell.edu)

 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the <organization> nor the
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
#include "concentricloopspather.h"

/*

THIS PATHER EXPOSES AN ISSUE WITH THE LoopInXYPlane CLASS!

If you run this pather, you'll notice that as the loops get closer to the
center, the contraction algorithm begins to break down.  What were once smooth
curves turn into chaotic, jittery lines.



*/

ConcentricLoopsPather::ConcentricLoopsPather()
  : initialized_(false),
    path_width_(0.0) {
}



//----[  initialize  ]---------------------------------------------------------
void ConcentricLoopsPather::initialize() {
  if (initialized_) {
  } else {
    initialized_ = true;
    path_width_ = getSettings().get("PathWidth", 1.5f);
  }
}


//----[  runPathingAlgorithm  ]------------------------------------------------
bool ConcentricLoopsPather::runPathingAlgorithm(
        const SliceMaterialRegion& region,
        PathSliceRegion* returned_paths,
        PatherProgressCallback* callback) {
  initialize();

  QVector<FAHLoopInXYPlane> boundaries;
  boundaries.append(region.getOuterBoundary());
  float last = 10000000.0f;
  while (boundaries.isEmpty() == false) {
    QVector<FAHLoopInXYPlane> contracted;
    FAHLoopInXYPlane front = boundaries.at(0);
    Path p(front);
    returned_paths->addPath(p);
    boundaries.remove(0);
    front.expand(-path_width_, &contracted);
    float e = contracted.at(0).areaEstimate();
    if (e > last) break;
    last = e;
    boundaries += contracted;
  }

  return true;
}
