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
#pragma once

#include "patherinterface.h"
#include "pathslicematerial.h"
#include "pather.h"

/**
 * This pather is meant for debugging purposes and only generates paths
 * composed of the bounding edges of a Slicer region.
 */
class EdgeOnlyPather: public Pather {
  protected:

  /**
  * runPathingAlgorithm executes the edge-only pathing algorithm upon a
  * region of a slice as created by the library Slicer algorithms. Note that
  * the edge-only pathing algorithm only actually paths the bounding edges
  * of a given slice, hence it is only used for debugging and has little
  * functional use.
  *
  * @param region a SliceMaterialRegion reference to the region to be pathed
  * @param returned_paths a PathSliceRegion pointer to the paths generated
  *        by the pathing algorithm
  * @param callback a PatherProgressCallback that can monitor the progress
  *        of the pathing algorithm
  */
  virtual bool runPathingAlgorithm(
        const SliceMaterialRegion& region,
        PathSliceRegion* returned_paths,
        PatherProgressCallback* callback);
};

