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

#include "pathsliceregion.h"

typedef QVector<PathSliceRegion*> PathSliceRegionArray;

/**
 * Contains the set of all the discontinuous regions covered by a particular
 * material within a slice.
 *
 * Contains a list of PathSliceRegions
 * @see PathSliceRegion
 */
class PathSliceMaterial {
public:

  typedef int MaterialID;

  /**
   *  Constructor takes an argument for the type of material that this
   *  PathSliceMaterial should take as a property
   *
   *  @param material the integer ID number for the material to be utilized
   */
  PathSliceMaterial(MaterialID material);
  ~PathSliceMaterial();

  /**
   * Returns the expected diameter of the circle of material that will be
   * deposited along paths in this material.
   */
  float pathWidth() const;

  /**
   * Sum of the length of all of the paths in all regions in this slice
   */
  float totalPathLength() const;

  /**
   * Sum of the length of all of the paths in all regions in this slice
   */
  float totalLengthBetweenPaths() const;
  
  /**
   * Obtains the set of all of the areas that this material covers
   *
   * @return a PathSliceRegionArray reference to the areas of this material
   */
  const PathSliceRegionArray& getRegions() const;

  /**
   * Adds a region to regions.  This region must be allocated with new()
   * and not be used anywhere else.
   */
  void add(PathSliceRegion* region);

  /**
   * Gets the ID of this material
   */
  MaterialID materialID() const;


private:
  PathSliceRegionArray regions_; /// all PathSliceRegions contained in this
                                 /// PathSliceMaterial
  MaterialID material_; /// the material property this PathSliceMaterial holds
};
