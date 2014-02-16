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
#include "triangle.h"
#include <common/confirm.h>  // code protection, debugging
#include <common/todo.h>     // error tracking
#include "line.h"            // edges are lines
#include "plane.h"
#include <QDebug>



namespace Math {


Triangle::Triangle() {
}


Triangle::Triangle(const Triangle& other) {
  v[0].set(other.v[0]);
  v[1].set(other.v[1]);
  v[2].set(other.v[2]);
}


Triangle::Triangle(const Triangle* other) {
  confirm(other) then {
    v[0].set(other->v[0]);
    v[1].set(other->v[1]);
    v[2].set(other->v[2]);
  }
}


Triangle::Triangle(const Vector3& a,
                             const Vector3& b,
                             const Vector3& c) {
  set(a, b, c);
}


void Triangle::edge(int edge_number, Line* segment) const {
  switch (edge_number) {
  default: {
      confirm(!"trying to access invalid edge; returning empty segment instead");
      segment->zero();
    } break;
  case 0: segment->set(edge0()); break;
  case 1: segment->set(edge1()); break;
  case 2: segment->set(edge2()); break;
  }
}


Line Triangle::edge0() const {
  Line e(v[0], v[1]);
  return e;
}


Line Triangle::edge1() const {
  Line e(v[1], v[2]);
  return e;
}


Line Triangle::edge2() const {
  Line e(v[2], v[0]);
  return e;
}


Vector3 Triangle::center() const {
  static const Float kOneThird = 1.0 / 3.0;
  return v[0].copy().add(v[1]).add(v[2]).scale(kOneThird);
}


Vector3 Triangle::normal() const {
  // normal is found by:
  //  normalize( (v1-v0) cross (v2 - v0) )
  return v[1].copy().sub(v[0]).cross(v[2].copy().sub(v[1])).normalize();
}


void Triangle::set(const Vector3& a,
                   const Vector3& b,
                   const Vector3& c) {
  v[0].set(a);
  v[1].set(b);
  v[2].set(c);
}

bool Triangle::intersectXYPlaneAtZ(Float z, Line* intersection) const {

  if ((v[0].z >= z && v[1].z >= z && v[2].z >= z) ||
      (v[0].z <= z && v[1].z <= z && v[2].z <= z)) {
    return false;
  }

  Vector3 p, p1, p2;
  int points_found = 0;

  for (int i0=0; i0<3; i0++) {
    int i1 = (i0+1)%3;
    if ((v[i0].z <= z && v[i1].z > z) || (v[i1].z <= z && v[i0].z > z)) {
      p.set(v[i0]).add(v[i1].copy().sub(v[i0]).scale((z - v[i0].z)/(v[i1].z-v[i0].z)));
      //p = v[i0]+(v[i1]-v[i0])*(z-v[i0].z)/(v[i1].z-v[i0].z);
      if (points_found == 0) {
        p1.set(p);
        points_found++;
      } else {
        p2.set(p);
        // set right order
        if ((normal().cross(p2.copy().sub(p1))).dot(Vector3::make(0,0,1)) < 0) { // swap
          p.set(p1);
          p1.set(p2);
          p2.set(p);
        }
        intersection->set(p1, p2);
        return true;
      }
    }
  }

  return false; // less than 2 points found
}



bool Triangle::intersectedByRay(const Vector3& origin,
                                const Vector3& ray) {
  Vector3 e1,e2;
  e1.set(v[1]).sub(v[0]);
  e2.set(v[2]).sub(v[0]);

  Vector3 q;
  q.set(ray).cross(e2);
  double a=e1.dot(q);

  // vector is parallel to plane
  if (floatsEqual(a, 0)) return false;

  double f=1/a;
  Vector3 s;
  s.set(origin).sub(v[0]);
  double u=f*s.dot(q);

  // the intersection is outside the triangle
  if (u<0.0) return false;

  Vector3 r;
  r=s.cross(e1);
  double v=f*ray.dot(r);

  // the intersection is outside the triangle
  if (v<0.0 || u+v>1.0) return false;

  return true;
}


}
