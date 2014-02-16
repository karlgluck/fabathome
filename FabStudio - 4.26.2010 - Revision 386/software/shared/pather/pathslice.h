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

#include "pathslicematerial.h"

typedef QVector<PathSliceMaterial*> PathSliceMaterialsArray;

/**
 * One horizontal plane (perpendicular to the Z axis) that defines the strips
 * of material that need to be deposited in order to create this layer in the
 * original model
 *
 * Contains a list of PathSliceMaterial
 * @see PathSliceMaterial
 */
class PathSlice {
public:

  /**
   * Initializes the collection of paths on a given layer with its Z height
   */
  PathSlice(float z);

  /**
   * Frees memory
   */
  ~PathSlice();

  /**
   * Returns a reference to the list of PathSliceMaterials in this slice
   * @return reference to a PathSliceMaterialsArray containing all materials
   * on this slice
   */
  const PathSliceMaterialsArray& getPathSliceMaterials() const;

  /**
   * Computes the sum of the lengths of all paths in this slice
   */
  float totalPathLength() const;

  /**
   * Finds the sum of all of the distances between each path's end and the
   * next one's start
   */
  float totalLengthBetweenPaths() const;

  /**
   * 
   */
  bool testForSelfIntersection() const;

  /**
   * Returns the Z-coordinate of this slice
   */
  float getZ() const;

  /**
   * Appends a material to this slice.  'delete' will be called on this
   * object when the slice is deallocated.
   */
  void addMaterial(PathSliceMaterial* material);

private:
  /// The z-coordinate height of this slice
  float z_;

  /// the set of materials contained within this PathSlice
  PathSliceMaterialsArray materials_;

};

