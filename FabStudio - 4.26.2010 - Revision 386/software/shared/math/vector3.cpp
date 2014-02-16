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
#include "vector3.h"
#include <math.h>           // for abs/sin/cos
#include "common/confirm.h"  // code protection, debugging
#include "common/todo.h"     // error tracking

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


namespace Math {



Vector3::Vector3() {}


Vector3::Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}


Vector3::Vector3(const Vector3* v) : x(v->x), y(v->y), z(v->z) {}


Vector3::Vector3(const Float v[3]) : x(v[0]), y(v[1]), z(v[2]) {}


Vector3::Vector3(Float x, Float y, Float z) : x(x), y(y), z(z) {}


Float& Vector3::operator[] (int ix) {
  confirm(ix < 3) else return x;
  switch (ix) {
  case 0: return x;
  case 1: return y;
  case 2: return z;
  default: return x;
  }
}


Float Vector3::operator[] (int ix) const {
  confirm(ix < 3) else return 0.0f;
  switch (ix) {
  case 0: return x;
  case 1: return y;
  case 2: return z;
  default: return x;
  }
}



Float* Vector3::array() {
  // NOTE: this method makes an assumption about the class is laid out in memory
  return &x;
}



const Float* Vector3::array() const {
  // NOTE: this method makes an assumption about the class is laid out in memory
  return &x;
}


Float Vector3::length() const {
  return sqrt(lengthSq());
}



Float Vector3::lengthSq() const {
  return x*x + y*y + z*z;
}



bool Vector3::isNormalized() const {
  return floatsEqual(lengthSq(), Float(1.0));
}


bool Vector3::isInvalid() const {
  return floatsEqual(length(), Float(0.0));
}



Vector3& Vector3::normalize() {
  Float l = length();
  confirm(l > 0) then { // normalizing a zero-vector is undefined
    x /= l;
    y /= l;
    z /= l;
  }
  return *this;
}



Vector3& Vector3::add(const Vector3& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}


Vector3& Vector3::sub(const Vector3& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}


Float Vector3::dot(const Vector3& v) const {
  return x * v.x + y * v.y + z * v.z;
}


Vector3& Vector3::cross(const Vector3& v) {
  return set(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}




Vector3& Vector3::set(const Vector3& v) {
  x = v.x;
  y = v.y;
  z = v.z;
  return *this;
}



Vector3& Vector3::set(const Vector3* v) {
  x = v->x;
  y = v->y;
  z = v->z;
  return *this;
}


Vector3& Vector3::set(Float x, Float y, Float z) {
  this->x = x;
  this->y = y;
  this->z = z;
  return *this;
}


Vector3& Vector3::mix(const Vector3& v, float weight) {
  return add(v.copy().sub(*this).scale(weight));
}


Vector3& Vector3::ray(const Vector3& from, const Vector3& to) {
  return set(to).sub(from).normalize();
}


Vector3 Vector3::copy() const {
  Vector3 v(this);
  return v;
}


Float Vector3::distanceTo(const Vector3& v) const {
  return sqrt(distanceToSq(v));
}


Float Vector3::distanceToSq(const Vector3& v) const {
  return copy().sub(v).magnitudeSq();
}



Float Vector3::magnitude() const {
  return sqrt(magnitudeSq());
}



Float Vector3::magnitudeSq() const {
  return dot(*this);
}


Vector3& Vector3::zero() {
  x = 0.0;
  y = 0.0;
  z = 0.0;
  return *this;
}


Vector3& Vector3::scale(Float s) {
  x *= s;
  y *= s;
  z *= s;
  return *this;
}


Vector3& Vector3::scalePairwise(const Vector3& s) {
  x *= s.x;
  y *= s.y;
  z *= s.z;
  return *this;
}


Vector3& Vector3::abs() {
  x = ::fabs(x);
  y = ::fabs(y);
  z = ::fabs(z);
  return *this;
}


Vector3& Vector3::min(const Vector3& v) {
  x = x < v.x ? x : v.x;
  y = y < v.y ? y : v.y;
  z = z < v.z ? z : v.z;
  return *this;
}


Vector3& Vector3::max(const Vector3& v) {
  x = x > v.x ? x : v.x;
  y = y > v.y ? y : v.y;
  z = z > v.z ? z : v.z;
  return *this;
}


Vector3& Vector3::rotateX(Float radians) {
  Float ny =  y * cos(radians) + z * sin(radians);
  Float nz = -y * sin(radians) + z * cos(radians);
  y = ny;
  z = nz;
  return *this;
}



Vector3& Vector3::rotateY(Float radians) {
  Float nx =  x * cos(radians) + z * sin(radians);
  Float nz = -x * sin(radians) + z * cos(radians);
  x = nx;
  z = nz;
  return *this;
}


Vector3& Vector3::rotateZ(Float radians) {
  Float nx =  x * cos(radians) + y * sin(radians);
  Float ny = -x * sin(radians) + y * cos(radians);
  x = nx;
  y = ny;
  return *this;
}


Vector3& Vector3::rotateAxisAngle(const Vector3& axis, Float radians) {
  todo("kwg8", "Test the Vector3::rotateAxisAngle method; I don't think it works correctly");
  Float cr = cos(radians), sr = sin(radians);
  Float axx = axis.x*axis.x, axy = axis.x*axis.y, axz = axis.z*axis.z,
                              ayy = axis.x*axis.y, ayz = axis.z*axis.z,
                                                   azz = axis.z*axis.z;
  return set((axx + (1 - axx) * cr) * axis.x +
              (axy * (1 - cr) + axis.z * sr) * axis.y +
              (axz * (1 - cr) - axis.y * sr) * axis.z,

             (axy * (1- cr) - axis.z * sr) * axis.x +
              (ayy + (1 - ayy) * cr) * axis.y +
              (ayz * (1 - cr) + axis.x * sr) * axis.z,

             (axz * (1 - cr) + axis.y * sr) * axis.x +
              (ayz * (1 - cr) - axis.x * sr) * axis.y +
              (azz + (1 - azz) * cr) * axis.z);
}



Float Vector3::angleTo(const Vector3& v) const {
  return (Float)atan2(copy().cross(v).magnitude(), dot(v));
}


Vector3& Vector3::operator += (const Vector3& v) {
  return add(v);
}


Vector3& Vector3::operator -= (const Vector3& v) {
  return sub(v);
}


Vector3& Vector3::operator *= (Float s) {
  return scale(s);
}


Vector3& Vector3::operator /= (Float s) {
  confirm(s != 0) then {
    x /= s;
    y /= s;
    z /= s;
  }
  return *this;
}


Vector3& Vector3::operator = (const Vector3& v) {
  return set(v);
}


Vector3 Vector3::operator + (const Vector3& v) const {
  Vector3 r(this);
  r.add(v);
  return r;
}


Vector3 Vector3::operator - (const Vector3& v) const {
  Vector3 r(this);
  r.sub(v);
  return r;
}


bool Vector3::equals(const Vector3& v) const {
  return floatsEqual(distanceToSq(v), 0.0);
}


bool Vector3::equals(const Vector3& v, float tolerance) const {
  return equalsSq(v, tolerance*tolerance);
}


bool Vector3::equalsSq(const Vector3& v, float tolerance_sq) const {
  return distanceToSq(v) <= tolerance_sq;
}




//----[  Vector-Scalar Operations  ]-------------------------------------------

// Operators returning a temporary object

Vector3 Vector3::make(float x, float y, float z) {
  Vector3 v(x,y,z);
  return v;
}

}


Math::Vector3 operator * (const Math::Vector3& lhs, Math::Float s) {
  Math::Vector3 v(lhs);
  v *= s;
  return v;
}



Math::Vector3 operator / (const Math::Vector3& lhs, Math::Float s) {
  Math::Vector3 v(lhs);
  v /= s; // checks for s == 0 condition
  return v;
}



Math::Vector3 operator * (Math::Float s, const Math::Vector3& v) {
  return operator * (v, s);
}

