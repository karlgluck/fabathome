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
#include "float.h"
#include "structs.h"
#include "quaternion.h"
#include "vector3.h"
#include <math.h>
#include <common/confirm.h>


namespace Math {

Quaternion::Quaternion() {
}

Quaternion::Quaternion(const Quaternion& q) : w(q.w), x(q.x), y(q.y), z(q.z) {
}

Quaternion::Quaternion(const Quaternion* q) : w(q->w), x(q->x), y(q->y), z(q->z) {
}

Quaternion::Quaternion(const Vector3& axis, Float angle) {
  set(axis, angle);
}

Quaternion::Quaternion(const Vector3* axis, Float angle) {
  set(axis, angle);
}

Quaternion::Quaternion(Float w, Float x, Float y, Float z) : w(w), x(x), y(y), z(z) {
}

Quaternion::Quaternion(const Float wxyz[4]) : w(wxyz[0]), x(wxyz[1]), y(wxyz[2]), z(wxyz[3]) {
}

Quaternion& Quaternion::set(const Quaternion& q) {
  return set(q.w, q.x, q.y, q.z);
}

Quaternion& Quaternion::set(const Quaternion* q) {
  return set(*q);
}

Quaternion& Quaternion::set(const Vector3& axis, Float angle) {
  confirm(axis.isNormalized()) else {
    // the axis must be normalized for this method to work
    return axis.isInvalid() ? zero() : set(axis.copy().normalize(), angle);
  }
  Float sin_by_2 = Float(sin(angle/2));
  return set(Float(cos(angle/2)),
             axis.x * sin_by_2,
             axis.y * sin_by_2,
             axis.z * sin_by_2);
}

Quaternion& Quaternion::set(const Vector3* axis, Float angle) {
  return set(*axis, angle);
}

Quaternion& Quaternion::set(Float w, Float x, Float y, Float z) {
  this->w = w;
  this->x = x;
  this->y = y;
  this->z = z;
  return *this;
}

Quaternion& Quaternion::set(const Float wxyz[4]) {
  return set(wxyz[0], wxyz[1], wxyz[2], wxyz[3]);
}


Quaternion& Quaternion::setEuler(Float x, Float y, Float z) {
  Float c2 = cos(z/2), s2 = sin(z/2);
  Float c1 = cos(y/2), s1 = sin(y/2);
  Float c3 = cos(x/2), s3 = sin(x/2);
  Float c1c2 = c1*c2;
  Float s1s2 = s1*s2;
  return set(c1c2*c3 - s1s2*s3,
             c1c2*s3 + s1s2*c3,
             s1*c2*c3 + c1*s2*s3,
             c1*s2*c3 - s1*c2*s3);
}

Quaternion& Quaternion::setEuler(const Vector3& xyz) {
  return setEuler(xyz.x, xyz.y, xyz.z);
}

Quaternion& Quaternion::mul(const Quaternion& q) {
  return set(w*q.w - x*q.x - y*q.y - z*q.z,
             w*q.x + x*q.w + y*q.z - z*q.y,
             w*q.y - x*q.z + y*q.w + z*q.x,
             w*q.z + x*q.y - y*q.x + z*q.w);
}

Quaternion& Quaternion::mul(const Quaternion* q) {
  return mul(*q);
}

Quaternion& Quaternion::zero() {
  return set(0,0,0,0);
}

Quaternion& Quaternion::identity() {
  return set(1,0,0,0);
}

Float Quaternion::magnitude() const {
  return Float(sqrt(magnitudeSq()));
}

Float Quaternion::magnitudeSq() const {
  return w*w + x*x + y*y + z*z;
}

Quaternion& Quaternion::normalize() {
  Float m = magnitude();
  return set(w/m, x/m, y/m, z/m);
}

bool Quaternion::isInvalid() const {
  return floatsEqual(magnitude(), Float(0.0));
}

bool Quaternion::isNormalized() const {
  return floatsEqual(magnitudeSq(), Float(1.0));
}

Quaternion Quaternion::copy() const {
  return *this;
}

Vector3 Quaternion::eulerAngles() const {
  Float sqw = w*w;
  Float sqx = x*x;
  Float sqy = y*y;
  Float sqz = z*z;
  Float unit = sqx + sqy + sqz + sqw; // if normalized is one, otherwise is correction factor
  Float test = x*y + z*w;
  if (test > 0.499*unit) { // singularity at north pole
    Vector3 v(0, 2 * atan2(x, w), Math::kPi/2);
    return v;
  } else if (test < -0.499*unit) { // singularity at south pole
    Vector3 v(0, -2 * atan2(x, w), -Math::kPi/2);
    return v;
  } else {
    Vector3 v(atan2(2*x*w-2*y*z, -sqx + sqy - sqz + sqw),
              atan2(2*y*w-2*x*z,  sqx - sqy - sqz + sqw),
              asin(2*test/unit));
    Vector3 f(v);
    f.scale(1.0/Math::kPi);
    return v;
  }
}

}
