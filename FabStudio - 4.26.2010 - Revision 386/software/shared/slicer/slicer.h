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
#pragma once

#include "shared/fabathome-constants.h"
#include "shared/math/triangle.h"
#include "shared/math/vector3.h"
#include "shared/math/plane.h"
#include "shared/math/line.h"
#include "shared/amf/amffile.h"
#include "slicestack.h"
#include <QList>
#include "shared/amf/amfvertex.h"
#include "shared/amf/amftriangle.h"
#include "shared/amf/amfregion.h"
#include "shared/amf/amfobject.h"
#include <QStringList>


class SlicerProgressCallback;
class SliceStack;
class SlicerSettings;

/**
 * 
 */
class Slicer {
public:

  /**
   * Different ways to align the slice stack
   */
  enum Alignment {
    kAlignTray,
    kAlignTop,
    kAlignBottom,
    kAlignCenter
  };

  /**
   * Returns the list of string versions of the different alignment enumeration
   * members such that the string for a member is at the index of that member's
   * integer value.
   */
  static QStringList alignmentStrings();

  /**
   * Error types the slicer can encounter while performing its job
   */
  enum Error {
    kNone,
    kGenericError,
  };


public:

  Slicer();

  /**
   * Changes how the slicer aligns itself based on the input geometry.  This
   * will override the path-width alignment setting if it was
   * previously activated.
   */
  void setAlignment(Alignment alignment);
  
  /**
   * Sets the distance between consecutive slices.
   */
  void setSliceHeight(FAHFloat slice_height);

  /**
   * Returns true if the slicer was able to process the input region.
   * If this method fails, it returns false and sets the internal
   * error code.
   * While slicing happens, the progress callback is updated each time
   * a slice is computed.  The callback can be NULL if it's not needed.
   */
  bool doSlicing(const AMFRegion* input_region,
                 SlicerProgressCallback* callback);

  /**
   * Used for reporting problems during slicing
   */
  Error getLastError() const;

  /**
   * Returns a string based on the last error code
   */
  QString getLastErrorString() const;
  
  /**
   * Returns descriptive text indicating what a given error code means
   */
  static QString getErrorString(Error error);

  /**
   * Access the result of the slicing operation
   */
  const SliceStack& getSliceStack();

private:

  /**
   * Discovers all of the loops in the input set of segments
   */
  void findLoops(const QList<FAHLine>& segments,
                 QVector<FAHLoopInXYPlane>* loops_out);

private:

  SlicerProgressCallback* callback;
  Alignment alignment;
  SliceStack slicestack;
  FAHFloat slice_height;
};

