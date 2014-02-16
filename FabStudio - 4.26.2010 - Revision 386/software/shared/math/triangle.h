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

#include "vector3.h"
#include "structs.h"

namespace Math {

//----[  Triangle  ]-----------------------------------------------------------
 struct Triangle {

  Vector3 v[3];

  /**
   * Constructor, basic
   */
  Triangle();

  /**
   * Constructor
   *
   * Clones the input Triangle object
   */
  Triangle(const Triangle& other);

  /**
   * Makes the triangle identical to the input Triangle object
   */
  explicit Triangle(const Triangle* other);

  /**
   * Sets the vertices of the Triangle to a, b, and c
   */
  explicit Triangle(const Vector3& a, const Vector3& b, const Vector3& c);

  /** 
   * Returns true if the triangle's three vertices are distinct.
   */
  bool isValid() const;

  /**
   * Returns the line segment defining the edge of this triangle as specified
   * by the edge_number parameter:
   *  0:    vertex[0], vertex[1]
   *  1:    vertex[1], vertex[2]
   *  2:    vertex[2], vertex[0]
   *  else: (error) a zero-length line at the origin will be returned
   */
  void edge(int edge_number, Line* segment) const;

  /**
   * Returns edge 0 of the triangle
   */
  Line edge0() const;

  /**
   * Returns edge 1 of the triangle
   */
  Line edge1() const;

  /**
   * Returns edge 2 of the triangle
   */
  Line edge2() const;

  /**
   * Calculates and returns the center of this triangle
   */
  Vector3 center() const;

  /**
   * Calculates the normal vector for this triangle using the right-hand rule;
   * i.e. the normal is positive on the side from which the triangle's points
   * circle counterclockwise.
   * This triangle should be valid before finding the normal.
   */
  Vector3 normal() const;

  /**
   * Sets the vertices of the triangle to a, b, and c
   */
  void set(const Vector3& a, const Vector3& b, const Vector3& c);

  /**
   * Calculates the area of this triangle
   */
  Float area() const;

  /**
   * Returns true and sets the values of 'line'
   */

  /*
    after finding the intersecting line
    0<n.{[((y+z)/2)-x]x(b-a)} has to always be followed
   b and a are the start and ends of the line segment
   where xy is the edge where the intersecting line starts
         zx is the edge where the intersecting line ends
   Also needs to return two edge numbers for starting edge and ending edge
   edge numbers are defined as
    0:    vertex[0], vertex[1]
    1:    vertex[1], vertex[2]
    2:    vertex[2], vertex[0]
  */
  bool intersect(const Plane& plane,
                 Line* line,
                 int* edge1, int* edge2) const;

  /**
   * Finds a line segment that is both in this triangle and in the given
   * plane.  The returned segment will always satisfy the condition that:
   *
   *    0 < dot( normal, cross[ (y + z)/2 - x, b - a ] )
   *
   * where normal, x, y, and z are properties of this triangle such that
   * x is the common vertex of the two intersected edges, y and z are the
   * distinct vertices, and b and a are properties of the intersection line.
   *
   * Corner cases are defined as follows:
   *  - if one corner of this triangle is in the plane and the plane crosses
   *    no edge of the triangle, no intersection is returned
   *  - if two corners of the triangle are in the plane (i.e. one
   *    edge of the triangle is in the plane), an intersection is only
   *    returned if the center of the triangle is in the positive space of
   *    the plane
   *  - if the entire triangle is coincident with the plane, no intersection
   *    is returned
   */
  bool intersectSkewPlane(const Plane& plane,
                          Line* intersection) const;

  /**
   * Finds a line that intersects this triangle at the given z coordinate
   *
   * @param intersection line that intersects the triangle
   * @return True if there is an intersection, False if not
   */
  bool intersectXYPlaneAtZ(Float z, Line* intersection) const;

  /**
   * Returns true if the given ray intersects the interior of this triangle
   * @author Chris Hogan (coh5@cornell.edu)
   */
  bool intersectedByRay(const Vector3& origin,
                               const Vector3& ray);
};

}
