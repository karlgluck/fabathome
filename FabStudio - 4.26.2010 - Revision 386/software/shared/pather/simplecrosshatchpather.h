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

#include "pather.h"
#include "shared/slicer/slicematerialregion.h"
#include "shared/fabathome-constants.h"


/**
 * Implements a basic pathing algorithm. SimpleCrossHatchPather serves
 * as the most basic working algorithm. The algorithm is described as follows:
 * The bounding box of a region is crossed with lines at intervals as
 * defined by material width. The segments of these lines that lie within
 * the region are connected end-to-end to form the longest continuous paths
 * possible. Once all paths are either connected or there are no more
 * reasonable connections, the pathing is complete. Each alternating slice
 * is rotated 90 degrees to allow for the "cross-hatching" alternate pathing
 * pattern that gives some structural strength to the overall construction.
 */
class SimpleCrossHatchPather : public Pather {
public:
  SimpleCrossHatchPather();

protected:

  /**
   * Makes local copies of flexible settings and sets up the internals of the
   * pather.  If called multiple times, it will only initialize once.
   */
  void initialize();

protected:

  /**
   *  Whether or not initialize() has been finished
   */
  bool initialized_;
  /**
   *  How far apart to space the paths
   */
  FAHFloat path_width_;

  /// Variable used to keep track of x-y axis aligned cross-hatching.
  /// Is true if the SimpleCrossHatchPather is creating paths aligned to
  /// the x-axis.
  bool on_x_axis;

  /**
   * This method executes the pathing algorithm as defined by the
   * SimpleCrossHatchPather.
   *
   * @see
   * @param region the SliceMaterialRegion to be pathed
   * @param returned_paths the PathSliceRegion that results from the pathing
   * @param callback the PatherProgressCallback that keeps track of progress
   * @return returns true if the algorithm executes successfully,
   *         false otherwise
   */
  virtual bool runPathingAlgorithm(const SliceMaterialRegion& region,
                                   PathSliceRegion* returned_paths,
                                   PatherProgressCallback* callback);
};

