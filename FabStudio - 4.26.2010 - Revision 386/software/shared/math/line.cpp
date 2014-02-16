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
#include "line.h"
#include <common/confirm.h>  // code protection, debugging
#include <common/todo.h>     // error tracking
#include "math.h"

namespace Math {


Line::Line() {}


Line::Line(const Line& v) : a(v.a), b(v.b) {}


Line::Line(const Line* v) : a(v->a), b(v->b) {}


Line::Line(const Vector3& a, const Vector3& b) : a(a), b(b) {}


void Line::set(const Line& other) {
  a.set(other.a);
  b.set(other.b);
}


void Line::set(const Line* other) {
  confirm(other) then {
    a.set(other->a);
    b.set(other->b);
  }
}


void Line::set(const Vector3& a, const Vector3& b) {
  this->a.set(a);
  this->b.set(b);
}


bool Line::hasLength() const {
  return a.equals(b) == false;
}


Float Line::length() const {
  return b.copy().sub(a).length();
}




void Line::reverse() {
  Vector3 temp(a);
  a.set(b);
  b.set(temp);
}


bool Line::pointOnLine(const Vector3& v) const {
  // a + ray(from:a, to:v) * scalar = b
  // ray(from:a, to:v) * scalar = b - a
  // abs(ray(from:a, to:v)) = abs(ray(from:a, to:b))
  Vector3 t0, t1;
  return a.equals(v) ||
         b.equals(v) || 
         t0.ray(a, v).abs().equals(t1.ray(a, b).abs());
}


bool Line::pointOnSegment(const Vector3& v) const {
  // the point is in the segment if a ray from a->v is the reverse of
  // the ray from b->v, or either endpoint is equal to the point.
  bool point_on_segment = a.equals(v) || b.equals(v);
  if (!point_on_segment) {
    Vector3 t0, t1;
    float dot_product = t0.ray(a, v).dot(t1.ray(b, v));
    point_on_segment = floatsEqual(dot_product, -1.0);
  }
  return point_on_segment;
}



Vector3 Line::center() const {
  return a.copy().add(b).scale(0.5);
}


bool Line::isEndpoint(const Vector3& other) const {
  return a.equals(other) || b.equals(other);
}



bool Line::coincidentWithLine(const Line& line) const {
  return line.pointOnLine(a) && line.pointOnLine(b);
}



bool Line::equalsSegment(const Line& segment) const {
  return (a.equals(segment.a) && b.equals(segment.b)) ||
         (a.equals(segment.b) && b.equals(segment.a));
}



Line Line::intersectLineWithLine(const Line& other) const {
  confirm(!"intersectLineWithLine is not implemented") else {
    todo("kwg8","implement intersectLineWithLine");
  }
  Line invalid;
  return invalid.zero();
}


 Line& Line::zero() {
  a.zero();
  b.zero();
  return *this;
}


Line& Line::changeLength(Float amount) {
  confirm(hasLength()) else {
    // can't do anything if there is no length, since we can't find a
    // direction in which to extend the line
    return *this;
  }
  amount /= Float(2.0);
  Vector3 v(ray());
  v.normalize();
  a.sub(v.copy().scale(amount)); // a += norm(b-a) * amount/2
  b.add(v.copy().scale(amount)); // b -= norm(b-a) * amount/2
  return *this;
}


 Vector3 Line::ray() const {
  return b.copy().sub(a);
}



bool Line::intersectSegmentWithSegment2DXY(
        const Line& other,
        Vector3* result) const {

  // algorithm from:
  // http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

  Float ua = (other.b.x - other.a.x)*(a.y - other.a.y)
           - (other.b.y - other.a.y)*(a.x - other.a.x);
  Float ub = (b.x - a.x)*(a.y - other.a.y) - (b.y - a.y)*(a.x - other.a.x);
  Float denom = (other.b.y - other.a.y)*(b.x - a.x)
              - (other.b.x - other.a.x)*(b.y - a.y);

  if (denom == Float(0.0)) {
    return false; // no intersection; parallel or coincident lines
  }

  ua /= denom;
  ub /= denom;

  // ensure it is within both segments
  if (ua < 0.0 || ua > 1.0 || ub < 0.0 || ub > 1.0) {
    return false;
  }

  confirm(result) then {
    result->set(a.x + ua * (b.x - a.x),
                a.y + ua * (b.y - a.y),
                (a.z+b.z+other.a.z+other.b.z)/4.0);
  }
  return true;
}



Float Line::evaluate2DLineXY(
        const Vector3& point) const {
  Float dx = b.x - a.x, dy = b.y - a.y;
  // y - y0 = m(x - x0)
  // y - m(x - x0) - y0 = 0
  if (dx == Float(0.0)) {
    // this is an exactly vertical line
    Float difference = point.x - a.x;
    if (floatsEqual(difference, 0.0)) {
      return point.y - /* Float(0.0) -*/ a.y;
    } else if (difference > Float(0.0)) {
      return /* point.y */- dy * Float(+1e10) /* - a.y */;
    } else { //if (difference < Float(0.0)) {
      return /* point.y */- dy * Float(-1e10) /* - a.y */;
    }
  } else {
    return (point.y - (dy / dx) * (point.x - a.x) - a.y);
  }
}


Float Line::segmentDistanceTo2DXY(const Vector3& point) const {
  Float A = point.x - a.x;
  Float B = point.y - a.y;
  Float C = b.x - a.x;
  Float D = b.y - a.y;
  Float dot = A*C + B*D;
  Float len_sq = C*C + D*D;
  Float param = dot / len_sq;

  Float xx, yy;
  if (param < (Float)0.0) {
    xx = a.x;
    yy = a.y;
  } else if (param > (Float)1.0) {
    xx = b.x;
    yy = b.y;
  } else {
    xx = a.x + param * C;
    yy = a.y + param * D;
  }

  Float dx = point.x - xx;
  Float dy = point.y - yy;

  return sqrt(dx*dx + dy*dy);
}



Float Line::lineDistanceTo2DXY(const Vector3& point) const {
  Float A = point.x - a.x;
  Float B = point.y - a.y;
  Float C = b.x - a.x;
  Float D = b.y - a.y;
  Float dot = A*C + B*D;
  Float len_sq = C*C + D*D;
  Float param = dot / len_sq;

  Float xx, yy; // closest point on line
  xx = a.x + param * C;
  yy = a.y + param * D;

  Float dx = point.x - xx;
  Float dy = point.y - yy;

  return sqrt(dx*dx + dy*dy);
}

}
