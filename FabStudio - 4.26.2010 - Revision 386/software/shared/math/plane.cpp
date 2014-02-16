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
#include "plane.h"
#include "line.h"
#include "vector3.h"
#include "triangle.h"
#include <common/todo.h>
#include <common/confirm.h>


namespace Math {

Plane::Plane(const Vector3& a,
                       const Vector3& b,
                       const Vector3& c) {
  set(a,b,c);
}


Plane::Plane(const Vector3& point,
                       const Vector3& normal) {
  set(point, normal);
}



Vector3 Plane::pointOnPlane() const {
  // ax + by + cz + d = 0
  Line line;
  line.set(Vector3::make(0,0,0), Vector3::make(a,b,c));
  return intersectLine(line);
}


bool Plane::intersectLine(const Line& line, Vector3* intersection) const {
  Vector3 abc(a,b,c);
  Float numer = d + abc.dot(line.a);
  Float denom = -abc.dot(line.ray());

  if (denom == 0) {
    // parallel to the plane
    return false;
  }

  intersection->set(line.a).add(line.ray().scale(numer/denom));

  return true;
}


bool Plane::intersectSegment(const Line& segment, Vector3* intersection) const {
  Vector3 abc(a,b,c);
  Float numer = d + abc.dot(segment.a);
  Float denom = -abc.dot(segment.ray());

  if (denom == 0) {
    // parallel to the plane
    return false;
  }

  Float t = numer / denom;

  // check to make sure it falls between the segment's endpoints
  if (t < 0 || t > 1) return false;

  intersection->set(segment.a).add(segment.ray().scale(t));

  return true;
}


Vector3 Plane::intersectLine(const Line& line) const {
  Vector3 intersection;
  confirm(intersectLine(line, &intersection)) else {
    Vector3 v;
    return v;
  }
  return intersection;
}





Plane::RelativeLocation Plane::relativeLocationOfPoint(const Vector3& point) const {
  Float f = a*point[0] + b*point[1] + c*point[2] + d;
  if (f < 0) {
    return kBelow;
  } else if (floatsEqual(f, 0.0)) {
    return kCoincident;
  } else {
    return kAbove;
  }
}


void Plane::set(const Vector3& a, const Vector3& b, const Vector3& c) {
  struct Triangle triangle(a,b,c);
  set(a, triangle.normal());
}


void Plane::set(const Vector3& point, const Vector3& normal) {
  Vector3 abc(normal);
  abc.normalize();
  a = abc.x;
  b = abc.y;
  c = abc.z;
  d = -abc.dot(point);
}

}
