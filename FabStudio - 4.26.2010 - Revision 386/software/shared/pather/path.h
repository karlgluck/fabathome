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

#include "shared/math/loopinxyplane.h"
#include "shared/fabathome-constants.h"
#include "path.h"
#include "shared/math/vector3.h"


/**
 *  Path represents a single continuous path of material on a slice. Path
 *  is contained by the slice and material that defines its properties
 *  besides its coodinates on the x-y plane.
 */
class Path {
private:
  /**
   *  Represents the path in terms of points in space. First
   *  point is the beginning of the path, and the last point
   *  overlaps with the first point.
   */
  QVector<FAHVector3> points_;

public:
  Path();

  /**
   * Duplicates the contents of the other path
   */
  explicit Path(const Path* other_path);

  /**
   * Initializes this path with all of the points in the given loop
   */
  Path(const FAHLoopInXYPlane& loop);

  /**
   * Returns the list of points in this path
   */
  const QVector<FAHVector3>& points() const;

  /**
   * Computes, for each point in the loop, the number of overlap
   */
  const QVector<float> pileup() const;
  
  /**
   *  Prepends a point to the path
   *
   *  @param point the FAHVector3 point to be added to the path
   */
  void addPathPointStart(const FAHVector3& point);

  /**
   *  Appends a point to the path at the end
   *
   * @param point the FAHVector3 point to be added to the path
   */
  void addPathPointEnd(const FAHVector3& point);

  /**
   * Gets the area of space this path occupies
   *
   * @param min returned as the minimum point in space the path occupies
   * @param max returned as the maximum point in space the path occupies
   */
  void limits(FAHVector3* min, FAHVector3* max);

  /**
   * Obtains the total length of the path
   */
  FAHFloat length();

  /**
   *  Returns the beginning point of the path
   */
  const FAHVector3& start() const;

  /**
   *  Returns the end-point of the path
   */
  const FAHVector3& end() const;

  /**
   * Returns true if the given segment crosses this path
   */
  bool crossedBy(const FAHLine& segment) const;

  /**
   * if other = {x y z} and this = {A B C},
   * this becomes {A B C x y z}
   */
  void append(const Path& other);
  void append(const QVector<FAHVector3>& other);

  /**
   * if other = {x y z} and this = {A B C},
   * this becomes {A B C z y x}
   */
  void appendReversed(const Path& other);
  void appendReversed(const QVector<FAHVector3>& other);

  /**
   * if other = {x y z} and this = {A B C},
   * this becomes {x y z A B C}
   */
  void prepend(const Path& other);

  /**
   * if other = {x y z} and this = {A B C},
   * this becomes {z y x A B C}
   */
  void prependReversed(const Path& other);

  /**
   * Erases all points from this path
   */
  void clear();
};

