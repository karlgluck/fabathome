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

#include "line.h"
#include "structs.h"
#include "vector3.h"


namespace Math {

/**
 *  Plane represents a geometric plane in 3D space as defined by three
 *  coefficients a, b, c, and d that solve the equation aX + bY + cZ + d = 0
 */
struct Plane {

  Float a, b, c, d;

  /**
   *  Create a plane taht contains the three given points. Ordering will
   *  cause the three points taken to appear clockwise when viewing the plane
   *  from its "front" i.e. the direction in which its normal points.
   */
  explicit Plane(const Vector3& a, const Vector3& b, const Vector3& c);

  /**
   *  Creates a plane from a normal and a point
   *
   * @param point point on the plane
   * @param normal normal vector of the plane
   */
  explicit Plane(const Vector3& point, const Vector3& normal);

  Vector3 pointOnPlane() const;

  /**
   *  Calculates the point at which the given line intersects this plane.
   *
   *  @param line the line to be intersected with this plane
   *  @param intersection the returned intersection of the line and plane
   *  @return returns false if the line does not intersect or is coincident
   */
  bool intersectLine(const Line& line, Vector3* intersection) const;

  /**
   *  Calculates the point at which the given segment intersects this plane.
   *  Note that the segment is treated as a finite object, in that it has
   *  two end-points.
   *
   *  @param segment the Line that is to be intersected with the plane.
   *                 Requires that the Line is defined as a finite segment.
   *  @param intersection the returned intersection of the segment and plane
   *  @return returns false if the segment does not intersect or is coincedent
   */
  bool intersectSegment(const Line& segment, Vector3* intersection) const;

  /**
   * Calculates the point at which the given line intersects this plane.  If
   * the line does not intersect the plane or is coincident, a runtime error
   * is generated and returned point is somewhere on the line on the surface
   * of the plane that is closest to the given line.
   */
  Vector3 intersectLine(const Line& line) const;

  enum RelativeLocation {
    kAbove,
    kCoincident,
    kBelow,
  };

  /**
   *  Returns the relative location of a point in comparison to this plane
   *
   *  @param point the point to be compared with this plane
   *  @return a RelativeLocation enumeration that indicates whether the
   *          point is "above" i.e. in the direction of the normal of the plane,
   *          coincident upon the plane, or below the plane.
   */
  RelativeLocation relativeLocationOfPoint(const Vector3& point) const;

  /**
   *  Sets the value of this plane to be incident upon three points.
   *  Ordering matters in that a, b, and c will be observed to be clockwise
   *  when seen from the "front" i.e. the direction the normal of the plane
   *  points
   */
  void set(const Vector3& a, const Vector3& b, const Vector3& c);

  /**
   *  Sets the value of this plane to be incident upon a point and have
   *  the given normal.
   *
   *  @param point the point incident on the plane
   *  @param normal the normal of the plane
   */
  void set(const Vector3& point, const Vector3& normal);
};


}
