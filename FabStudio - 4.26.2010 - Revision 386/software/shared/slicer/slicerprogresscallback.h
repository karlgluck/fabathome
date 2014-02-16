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

class QString;
class SliceStack;

/**

  TODO: refactor "setEstimatedNumberOfSlices" --> "setNumberOfSlices"

**/

/**
 * Passing in a valid member of this type to the doSlicing() method allows the
 * caller to receive feedback about the progress of the operation as it occurs.
 */
class SlicerProgressCallback {
public:
  virtual ~SlicerProgressCallback() = 0;

  /**
   * Called before slicing begins to define how many slices will need to be
   * created.
   */
  virtual void setEstimatedNumberOfSlices(int count);

  /**
   * Invoked each time a slice has been filled out.  Typically, this is used
   * to increment a progress bar.
   */
  virtual void sliceHasBeenComputed(int index, float z);

  /**
   * Called when the slicer has some sort of problem.
   */
  virtual void encounteredIssue(const QString& description);

  /**
   * Invoked after slicing has completed
   */
  virtual void finishedSlicing(const SliceStack& slice_stack);
};
