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

#include "path.h"
#include "patherprogresscallback.h"
#include "pathstack.h"
#include "shared/utils/flexiblesettings.h"
#include "shared/slicer/slicestack.h"
#include "shared/slicer/slicematerialregion.h"
#include "pathsliceregion.h"

/**
 * Turns a set of slices into a set of paths with certain properties that will
 * cover those slices in material when printed.
 *
 * This is a base class whose pathing algorithm is virtual so that a subclass
 * can implement one of many algorithms to execute pathing.
 *
 * This class is not intended for direct use; instead, use PatherInterface.
 *
 * @see PatherInterface
 */
class Pather {

  enum Error {
    kErrorNone
  };

public:

  /**
   * Virtual destructor allows subclasses to run their destructors when the
   * superclass is deleted.
   */
  virtual ~Pather() = 0;

  /**
   * All of the settings that can be provided to a pather implementation
   * are mapped into this "flexible settings" structure.
   *
   * @returns a reference to the current FlexibleSettings
   */
  FlexibleSettings& getSettings();

  /**
   * Executes the implementation's pathing algorithm.  If something goes
   * wrong, this method returns false and the error code will be set.
   * The default implementation will call the subclass-implemented
   * "runPathingAlgorithm" for every region in each slice.
   * As the algorithm proceeds, the progress callback will be updated
   * with information.
   *
   * @param slices the SliceStack
   * @param callback
   * @returns true if pathing is successful, false otherwise
   */
  virtual bool doPathing(const SliceStack& slices,
                         PatherProgressCallback* callback);

  /**
   * Used for reporting problems during slicing
   */
  Error getLastError() const;

  /**
   * Returns a string based on the last error code
   */
  QString getLastErrorString() const;
  
  /**
   * Returns descriptive text indicating what a given error code means.  The
   * implementing pather is responsible for this method.  It can assign any
   * meaning to any error code except 0, which indicates that error occurred.
   */
  virtual QString getErrorString(Error error) const;

  /**
   * Obtains the data structure that holds the results of the pathing algorithm
   */
  const PathStack& getPathStack();


protected:

  /**
   * This method is invoked for every region of each slice by the default
   * doPathing implementation.  The default implementation of this method
   * set an error code and returns false.
   * @param returned_paths Will be filled with a set of newly-generated paths
   *                       based on the boundaries provided.
   * @return 'true' if pathing succeeded.  'false' indicates that
   *         getLastError will return some value elaborating on what went wrong
   */
  virtual bool runPathingAlgorithm(const SliceMaterialRegion& region,
                                   PathSliceRegion* returned_paths,
                                   PatherProgressCallback* callback);

  /**
   * Changes the value reported by getLastError.  If this method is called
   * multiple times, only the result of the last call will be available.
   */
  void setLastError(Error error);


private:

  /// All settings that are passed into the implementing pather class.
  FlexibleSettings settings_;

  /// The subclass sets this value when 
  Error last_error_;

  /// Computed paths go here
  PathStack path_stack_;
};

