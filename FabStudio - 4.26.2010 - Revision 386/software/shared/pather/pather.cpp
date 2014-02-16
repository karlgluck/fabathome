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
#include "pather.h"
#include "pathslicematerial.h"


//----[  ~Pather  ]------------------------------------------------------------
Pather::~Pather() {}



//----[  getSettings  ]--------------------------------------------------------
FlexibleSettings& Pather::getSettings() {
  return settings_;
}



//----[  doPathing  ]----------------------------------------------------------
bool Pather::doPathing(const SliceStack& slices,
                       PatherProgressCallback* callback) {
  const SliceArray& slice_array = slices.getSlices();

  // Let the callback know how many slices are going to be computed
  if (callback) callback->setNumberOfSlices(slice_array.size());

  for (int i = 0; i < slice_array.size(); i++) {

    // Start building a new slice
    PathSlice* current_slice = new PathSlice(slice_array[i].getZ());

    // Put the paths for all of the materials into the slice
    const SliceMaterialArray& material_array = slice_array[i].getMaterials();
    for (int j = 0; j < material_array.size(); j++) {

      // Build this material
      PathSliceMaterial* current_material
          = new PathSliceMaterial(material_array[j].getMaterialID());

      // For each of the different regions this material contains in this
      // layer, add all of the paths that are needed to cover that region.
      const SliceMaterialRegionArray& regions = material_array[j].getRegions();
      for (int k = 0; k < regions.size(); ++k) {
        PathSliceRegion* current_region = new PathSliceRegion();
        if (!runPathingAlgorithm(regions[k], current_region, callback)) {
          if (callback) callback->encounteredIssue(getLastErrorString());
          return false;
        }
        current_material->add(current_region);
      }

      current_slice->addMaterial(current_material);
    }
    path_stack_.addSlice(current_slice);
    if (callback) callback->sliceHasBeenComputed(i, current_slice);
  }

  // successs!
  return true;
}



//----[  getLastError  ]-------------------------------------------------------
Pather::Error Pather::getLastError() const {
  return last_error_;
}




//----[  getLastErrorString  ]-------------------------------------------------
QString Pather::getLastErrorString() const {
  return getErrorString(getLastError());
}




//----[  getErrorString  ]-----------------------------------------------------
QString Pather::getErrorString(Error error) const {
  return (error == kErrorNone) ? "No error." : "Unknown error.";
}




//----[  getPathStack  ]-------------------------------------------------------
const PathStack& Pather::getPathStack() {
  return path_stack_;
}





//----[  runPathingAlgorithm  ]------------------------------------------------
bool Pather::runPathingAlgorithm(const SliceMaterialRegion& region,
                                 PathSliceRegion* returned_paths,
                                 PatherProgressCallback* callback) {
  // default implementation does nothing
  if (callback) callback->encounteredIssue("Pathing algorithm not implemented");
  return true;
}




//----[  setLastError  ]-------------------------------------------------------
void Pather::setLastError(Error error) {
  last_error_ = error;
}

