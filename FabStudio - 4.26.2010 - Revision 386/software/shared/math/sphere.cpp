/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2010, Karl Gluck (kwg8@cornell.edu)

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
#include "sphere.h"
#include <math.h>           // for abs/sin/cos
#include "common/confirm.h"  // code protection, debugging
#include "common/todo.h"     // error tracking

namespace Math {


Sphere::Sphere() {}


Sphere::Sphere(const Vector3& v, Float r) {
  set(v,r);
}


Sphere::Sphere(const Sphere& s) {
  set(s);
}


Sphere::Sphere(const Sphere* s) {
  set(s);
}


Sphere& Sphere::set(const Vector3& v, Float r) {
  center.set(v);
  radius = r;
  return *this;
}



Sphere& Sphere::set(const Sphere& s) {
  center.set(s.center);
  radius = s.radius;
  return *this;
}


Sphere& Sphere::set(const Sphere* s) {
  center.set(s->center);
  radius = s->radius;
  return *this;
}


Sphere& Sphere::set(const Vector3& v1, const Vector3& v2) {
  center.set(v1).add(v2).scale(0.5f); // center = average of coordinates
  radius = v2.copy().sub(center).magnitude(); // radius contains both
  return *this;
}


bool Sphere::intersects(const Vector3& origin, const Vector3& ray) const {
  float enter, exit;
  return intersectDistances(origin, ray, &enter, &exit) > 0;
}


int Sphere::intersectDistances(const Vector3& origin, const Vector3& ray,
                               float* enter_distance, float* exit_distance) const {

  confirm(ray.isNormalized()) else {
    if (floatsEqual(ray.length(), 0)) return 0;
    return intersectDistances(origin,
                              ray.copy().normalize(),
                              enter_distance,
                              exit_distance);
  }

  Vector3 distance;
  distance.set(origin).sub(center);
  Float b = distance.dot(ray);
  Float c = distance.magnitudeSq() - radius*radius;
  Float d = b*b - c;
  if (d > Float(0)) {
    d = sqrt(d);
    *enter_distance = -b - d;
    if (b > d) {
      // intersects twice; ray origin outside the sphere & pointing at sphere
      *exit_distance = -b + d;
      return 2;
    } else {
      // intersects once; ray origin leaving sphere or ray just barely skims surface
      *exit_distance = *enter_distance;
      return 1;
    }
  } else {
    return 0;
  }
}


bool Sphere:: intersectNearLocation(const Vector3& origin, const Vector3& ray, Vector3* intersection) const {
  float enter, exit;
  if (0 == intersectDistances(origin, ray, &enter, &exit)) return false;
  intersection->set(ray).scale(enter).add(origin);
  return true;
}




}
