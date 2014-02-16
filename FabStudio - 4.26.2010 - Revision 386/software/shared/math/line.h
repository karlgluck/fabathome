/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009-2010, Karl Gluck (kwg8@cornell.edu)
 
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


#include "float.h"
#include "structs.h"
#include "vector3.h"

namespace Math {

/**
 *
 */
 struct Line {

  Vector3 a, b;

  /**
   * Constructor, basic
   */
  Line();

  /**
   * Constructor
   *
   * Clones the Line object given as input
   */
  Line(const Line& v);

  /**
   * Constructor
   * 
   * Clones the Line object given as input
   */
  explicit Line(const Line* v);

  /**
   * Constructor
   *
   * Creates a Line object with the Vector3 objects given as input
   */
  explicit Line(const Vector3& a, const Vector3& b);

  /**
   * Sets the two points of this line to the two points of the input line
   */
  void set(const Line& other);

  /**
   * Sets the two points of this line to the two points of the input line
   */
  void set(const Line* other);

  /**
   * Sets the two points of this line to the two inputs
   */
  void set(const Vector3& a, const Vector3& b);

  /**
   * Returns true if the two points defining this line are not the same
   * 
   * @return
   */
  bool hasLength() const;

  /**
   * Returns the length of the line
   */
  Float length() const;

  /**
   * Switches the endpoints of this line
   */
  void reverse();

  /**
   * Checks intersection of a point on this line presuming infinite length
   * 
   * @param v the point in space to be checked
   * @return true if 'v' is on the infinite line
   */
  bool pointOnLine(const Vector3& v) const;

  /**
   * Checks intersection of a point on this segment presuming finite length
   * 
   * @param v the point in space to be checked
   * @return true if 'v' is on the infinite line
   */
  bool pointOnSegment(const Vector3& v) const;

  /**
   * Assuming this line specifies a line segment, finds the center of the segment.
   * Can be called on an infinite line, but returned point has no conceptual meaning.
   * 
   * @return the center of the line
   */
  Vector3 center() const;

  /**
   * Returns 'true' if other is equal to the start or end of this segment
   */
  bool isEndpoint(const Vector3& other) const;

  /**
   * Compares this line with another to see if they are the same infinitely long lines
   * 
   * @param line the line to be compared with
   * @return true if the two lines are the same infinitely long line in space
   */
  bool coincidentWithLine(const Line& line) const;

  /**
   * Compares this line with another to see if they are the same line segment.
   * Assumes lines are undirected, so segment reversal has no effect on calculation
   * 
   * @param segment line segment to be compared with
   * @return true if the two line segments occupy the same space
   */
  bool equalsSegment(const Line& segment) const;

  /** 
   * Assumes this line and 'other' are infinitely long, and returns the shortest
   * segment with one end on each line.  This segment defines the closest
   * approach of the lines.  The returned line's first point will be from
   * 'this', and its second point will be from 'other'.
   * If the lines are skew, the returned line will have length; otherwise,
   * a true intersection will return a line with no length and no direction.
   * If the lines are coincident, this method will return a segment that is
   * coincident with both lines.
   * If either line has no length, this is equivalent to a line-point
   * intersection.  If both lines have no length, the returned segment will
   * simply be the segment containing a point from each line. 
   */
  Line intersectLineWithLine(const Line& other) const;

  /**
   * This method has the same behavior as intersectSegmentWithSegment, but it
   * only checks for 2D intersections on the XY plane (the Z coordinate is
   * entirely ignored on both segments).
   */
  bool intersectSegmentWithSegment2DXY(const Line& other,
                                       Vector3* result) const;

  /**
   * Resets both member points to the origin.  Returns *this to allow
   * method chaining.
   */
  Line& zero();

  /**
   * Adjusts the length of this line by the given delta.  If the change
   * would cause the length of the line to become negative, the line changes
   * its direction.
   */
  Line& changeLength(Float amount);

  /**
   * Obtains the ray that points from the line's first coordinate to its
   * second.  The returned value is not normalized.  If the line has no length,
   * a zero-length ray is returned.
   */
  Vector3 ray() const;

  /**
   * Returns the value of the line equation evaluated at the given point,
   * ignoring the Z coordinate of this line and the input point
   */
  Float evaluate2DLineXY(const Vector3& point) const;

  /**
   * Gets the distance from this segment to the given point, taking into
   * account that the line has finite length
   */
  Float segmentDistanceTo2DXY(const Vector3& point) const;

  /**
   * Calculates the distance from this line to the given point.  Ignores the
   * z-coordinate.
   */
  Float lineDistanceTo2DXY(const Vector3& point) const;
};

}
