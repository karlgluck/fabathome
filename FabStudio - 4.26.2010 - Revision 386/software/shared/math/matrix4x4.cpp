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
#include "matrix4x4.h"
#include "vector3.h"
#include <math.h>     // for sin/cos/tan
#include "common/confirm.h"
#include "quaternion.h"


namespace Math {

Matrix4x4::Matrix4x4() { }


Matrix4x4::Matrix4x4(const Matrix4x4& m) {
  set(m);
}


Matrix4x4::Matrix4x4(const Matrix4x4* m) {
  set(m);
}


Matrix4x4::Matrix4x4(const Float v[16]) {
}


Matrix4x4& Matrix4x4::zero() {
  _11 = Float(0);  _12 = Float(0);  _13 = Float(0);  _14 = Float(0);
  _21 = Float(0);  _22 = Float(0);  _23 = Float(0);  _24 = Float(0);
  _31 = Float(0);  _32 = Float(0);  _33 = Float(0);  _34 = Float(0);
  _41 = Float(0);  _42 = Float(0);  _43 = Float(0);  _44 = Float(0);
  return *this;
}


Matrix4x4& Matrix4x4::identity() {
  zero();
  _11 = Float(1.0);
  _22 = Float(1.0);
  _33 = Float(1.0);
  _44 = Float(1.0);
  return *this;
}


Matrix4x4 Matrix4x4::copy() const {
  Matrix4x4 other(this);
  return other;
}


Matrix4x4& Matrix4x4::set(const Matrix4x4& m) {
  return set(static_cast<const Float*>(&m._11));
}


Matrix4x4& Matrix4x4::set(const Matrix4x4* m) {
  confirm (m) then {
    set(static_cast<const Float*>(&m->_11));
  }
  return *this;
}


Matrix4x4& Matrix4x4::set(const Float v[16]) {
  confirm (v) then {
    _11 = v[0x0];  _12 = v[0x1];  _13 = v[0x2];  _14 = v[0x3];
    _21 = v[0x4];  _22 = v[0x5];  _23 = v[0x6];  _24 = v[0x7];
    _31 = v[0x8];  _32 = v[0x9];  _33 = v[0xA];  _34 = v[0xB];
    _41 = v[0xC];  _42 = v[0xD];  _43 = v[0xE];  _44 = v[0xF];
  }
  return *this;
}


Float* Matrix4x4::array() {
  return static_cast<Float*>(&_11);
}


Float* Matrix4x4::operator [] (int ix) {
  confirm (ix >= 0 && ix < 4) else return 0;
  return m[ix];
}


const Float* Matrix4x4::operator [] (int ix) const {
  confirm (ix >= 0 && ix < 4) else return 0;
  return m[ix];
}


Matrix4x4& Matrix4x4::mul(const Matrix4x4& m) {
  Float v[16] = {
    _11*m._11 + _12*m._21 + _13*m._31 + _14*m._41,
    _11*m._12 + _12*m._22 + _13*m._32 + _14*m._42,
    _11*m._13 + _12*m._23 + _13*m._33 + _14*m._43,
    _11*m._14 + _12*m._24 + _13*m._34 + _14*m._44,
    _21*m._11 + _22*m._21 + _23*m._31 + _24*m._41,
    _21*m._12 + _22*m._22 + _23*m._32 + _24*m._42,
    _21*m._13 + _22*m._23 + _23*m._33 + _24*m._43,
    _21*m._14 + _22*m._24 + _23*m._34 + _24*m._44,
    _31*m._11 + _32*m._21 + _33*m._31 + _34*m._41,
    _31*m._12 + _32*m._22 + _33*m._32 + _34*m._42,
    _31*m._13 + _32*m._23 + _33*m._33 + _34*m._43,
    _31*m._14 + _32*m._24 + _33*m._34 + _34*m._44,
    _41*m._11 + _42*m._21 + _43*m._31 + _44*m._41,
    _41*m._12 + _42*m._22 + _43*m._32 + _44*m._42,
    _41*m._13 + _42*m._23 + _43*m._33 + _44*m._43,
    _41*m._14 + _42*m._24 + _43*m._34 + _44*m._44
  };

  return set(v);
}


Vector3 Matrix4x4::transform(const Vector3& v) const {
  Vector3 temp(v);
  transformInPlace(&temp);
  return temp;
}


void Matrix4x4::transformInPlace(Vector3* v) const {
  // This is the 4th component of the resulting transformation.  The input vector's
  // 4th component is assumed to be 1.0 coming in, and the resulting vector is
  // normalized such that it would have a component of 1.0.
  Float w = _14 * v->x + _24 * v->y + _34 * v->z + _44;
  v->set(
    (_11 * v->x + _21 * v->y + _31 * v->z + _41) / w,
    (_12 * v->x + _22 * v->y + _32 * v->z + _42) / w,
    (_13 * v->x + _23 * v->y + _33 * v->z + _43) / w
  );
}


Matrix4x4& Matrix4x4::translation(const Vector3& v) {
  identity();
  _41 = v.x;
  _42 = v.y;
  _43 = v.z;
  return *this;
}


Matrix4x4& Matrix4x4::rotationX(Float a) {
  identity();
  const Float cos_a = cos(a);
  const Float sin_a = sin(a);
  _22 = cos_a;  _23 = -sin_a;
  _32 = sin_a;  _33 =  cos_a;
  return *this;
}


Matrix4x4& Matrix4x4::rotationY(Float a) {
  identity();
  const Float cos_a = cos(a);
  const Float sin_a = sin(a);
  _11 =  cos_a;  _13 = sin_a;
  _31 = -sin_a;  _33 = cos_a;
  return *this;
}


Matrix4x4& Matrix4x4::rotationZ(Float a) {
  identity();
  const Float cos_a = cos(a);
  const Float sin_a = sin(a);
  _11 = cos_a;  _12 = -sin_a;
  _21 = sin_a;  _22 =  cos_a;
  return *this;
}


Matrix4x4& Matrix4x4::rotationEuler(const Vector3& v) {
  return rotationEuler(v.x, v.y, v.z);
}


Matrix4x4& Matrix4x4::rotationEuler(Float x, Float y, Float z) {
  const Float cx = cos(x);
  const Float sx = sin(x);
  const Float cy = cos(y);
  const Float sy = sin(y);
  const Float cz = cos(z);
  const Float sz = sin(z);
  _11 = cz*cy;  _12 = cz*sy*sx - sz*cx;  _13 = cz*sy*cx + sz*sx;  _14 = 0;
  _21 = sz*cy;  _22 = sz*sy*sx + cz*cx;  _23 = sz*sy*cx - cz*sx;  _24 = 0;
  _31 = -sy;    _32 = cy*sx;             _33 = cy*cx;             _34 = 0;
  _41 = 0;      _42 = 0;                 _43 = 0;                 _44 = 1;
  return *this;
}


Matrix4x4& Matrix4x4::rotationAxisAngle(const Vector3& axis, Float angle) {
  Quaternion q(axis, angle);
  return rotationQuaternion(q);

// this is the old code; not sure if it even works
//  confirm(axis.isNormalized());
//  const Float c = cos(angle);
//  const Float s = sin(angle);
//  const Float xx = axis.x * axis.x;
//  const Float yy = axis.y * axis.y;
//  const Float zz = axis.z * axis.z;
//  _11 = xx + (1 - xx) * c;
//  _12 = axis.x * axis.y * (1 - c) - axis.z * s;
//  _13 = axis.x * axis.z * (1 - c) + axis.y * s;
//  _14 = Float(0.0);
//  _21 = axis.x * axis.y * (1 - c) + axis.z * s;
//  _22 = yy + (1 - yy) * c;
//  _23 = axis.y * axis.z * (1 - c) - axis.x * s;
//  _24 = Float(0.0);
//  _31 = axis.x * axis.z * (1 - c) - axis.y * s;
//  _32 = axis.y * axis.z * (1 - c) + axis.x * s;
//  _33 = zz + (1 - zz) * c;
//  _34 = Float(0.0);
//  _41 = Float(0.0);
//  _42 = Float(0.0);
//  _43 = Float(0.0);
//  _44 = Float(1.0);
//  return *this;
}

Matrix4x4& Matrix4x4::rotationQuaternion(const Quaternion& q) {
  confirm (q.isNormalized()) else {
    return q.isInvalid() ? zero() : rotationQuaternion(q.copy().normalize());
  }

  _11 = 1 - 2*q.y*q.y - 2*q.z*q.z;  _12 =     2*q.x*q.y - 2*q.w*q.z;  _13 =     2*q.x*q.z + 2*q.w*q.y;  _14 = 0;
  _21 =     2*q.x*q.y + 2*q.w*q.z;  _22 = 1 - 2*q.x*q.x - 2*q.z*q.z;  _23 =     2*q.y*q.z - 2*q.w*q.x;  _24 = 0;
  _31 =     2*q.x*q.z - 2*q.w*q.y;  _32 =     2*q.y*q.z + 2*q.w*q.x;  _33 = 1 - 2*q.x*q.x - 2*q.y*q.y;  _34 = 0;
  _41 = 0;                          _42 = 0;                          _43 = 0;                          _44 = 1;

  return *this;
}

Matrix4x4& Matrix4x4::scaling(const Vector3& v) {
  return scaling(v.x, v.y, v.z);
}


Matrix4x4& Matrix4x4::scaling(Float x, Float y, Float z) {
  zero();
  _11 = x;
  _22 = y;
  _33 = z;
  _44 = Float(1.0);
  return *this;
}



Matrix4x4& Matrix4x4::scalingUniform(Float s) {
  return scaling(s,s,s);
}


Matrix4x4& Matrix4x4::invert() {
  Float t;
  return invert(&t);
}


Matrix4x4& Matrix4x4::invert(Float* determinant) {
  confirm (determinant) else return *this;

  // Since the values of the matrix will have to be read frequently, copy
  // them into a temporary local array for faster access.
  Float v[16] = {
    _11, _12, _13, _14,
    _21, _22, _23, _24,
    _31, _32, _33, _34,
    _41, _42, _43, _44,
  };

  Float a0 = v[ 0]*v[ 5] - v[ 1]*v[ 4];
  Float a1 = v[ 0]*v[ 6] - v[ 2]*v[ 4];
  Float a2 = v[ 0]*v[ 7] - v[ 3]*v[ 4];
  Float a3 = v[ 1]*v[ 6] - v[ 2]*v[ 5];
  Float a4 = v[ 1]*v[ 7] - v[ 3]*v[ 5];
  Float a5 = v[ 2]*v[ 7] - v[ 3]*v[ 6];
  Float b0 = v[ 8]*v[13] - v[ 9]*v[12];
  Float b1 = v[ 8]*v[14] - v[10]*v[12];
  Float b2 = v[ 8]*v[15] - v[11]*v[12];
  Float b3 = v[ 9]*v[14] - v[10]*v[13];
  Float b4 = v[ 9]*v[15] - v[11]*v[13];
  Float b5 = v[10]*v[15] - v[11]*v[14];

  Float d = a0*b5-a1*b4+a2*b3+a3*b2-a4*b1+a5*b0;

  confirm (!floatsEqual(d, 0)) else {
    // this matrix is uninvertible
    zero();
    return *this;
  }

  Float inverse_d = Float(1.0)/d;

  // Each element of the final matrix should be divided by 'd', but we can
  // do this computation on these temporaries instead because multiplication
  // is distributive.
  a0 *= inverse_d; a1 *= inverse_d; a2 *= inverse_d;
  a3 *= inverse_d; a4 *= inverse_d; a5 *= inverse_d;
  b0 *= inverse_d; b1 *= inverse_d; b2 *= inverse_d;
  b3 *= inverse_d; b4 *= inverse_d; b5 *= inverse_d;

  _11 = + v[ 5]*b5 - v[ 6]*b4 + v[ 7]*b3;
  _21 = - v[ 4]*b5 + v[ 6]*b2 - v[ 7]*b1;
  _31 = + v[ 4]*b4 - v[ 5]*b2 + v[ 7]*b0;
  _41 = - v[ 4]*b3 + v[ 5]*b1 - v[ 6]*b0;
  _12 = - v[ 1]*b5 + v[ 2]*b4 - v[ 3]*b3;
  _22 = + v[ 0]*b5 - v[ 2]*b2 + v[ 3]*b1;
  _32 = - v[ 0]*b4 + v[ 1]*b2 - v[ 3]*b0;
  _42 = + v[ 0]*b3 - v[ 1]*b1 + v[ 2]*b0;
  _13 = + v[13]*a5 - v[14]*a4 + v[15]*a3;
  _23 = - v[12]*a5 + v[14]*a2 - v[15]*a1;
  _33 = + v[12]*a4 - v[13]*a2 + v[15]*a0;
  _43 = - v[12]*a3 + v[13]*a1 - v[14]*a0;
  _14 = - v[ 9]*a5 + v[10]*a4 - v[11]*a3;
  _24 = + v[ 8]*a5 - v[10]*a2 + v[11]*a1;
  _34 = - v[ 8]*a4 + v[ 9]*a2 - v[11]*a0;
  _44 = + v[ 8]*a3 - v[ 9]*a1 + v[10]*a0;

  return *this;
}


Float Matrix4x4::determinant() const {
  Float a0 = _11*_22 - _12*_21;
  Float a1 = _11*_23 - _13*_21;
  Float a2 = _11*_24 - _14*_21;
  Float a3 = _12*_23 - _13*_22;
  Float a4 = _12*_24 - _14*_22;
  Float a5 = _13*_24 - _14*_23;
  Float b0 = _31*_42 - _32*_41;
  Float b1 = _31*_43 - _33*_41;
  Float b2 = _31*_44 - _34*_41;
  Float b3 = _32*_43 - _33*_42;
  Float b4 = _32*_44 - _34*_42;
  Float b5 = _33*_44 - _34*_43;

  return a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;
}


Matrix4x4& Matrix4x4::transpose() {
  Float t;
#define swap(a,b) t = a; a = b; b = t;
  swap(_12, _21);
  swap(_13, _31);
  swap(_14, _41);
  swap(_23, _32);
  swap(_24, _42);
  swap(_34, _43);
#undef swap
  return *this;
}


Matrix4x4& Matrix4x4::glProjectionPerspective(Float fov, Float aspect, Float near_z, Float far_z) {
  zero();
  Float f = Float(1.0) / tan(fov/2); // cot = 1.0f / tan
  _11 = f / aspect;
  _22 = f;
  Float near_minus_far = near_z - far_z;
  _33 = (far_z + near_z) / near_minus_far;
  _34 = Float(-1.0);
  _43 = 2 * far_z * near_z / near_minus_far;
  return *this;
}


Matrix4x4& Matrix4x4::glView(const Vector3& eye,
                                                        const Vector3& at,
                                                        const Vector3& up) {

  Vector3 x, y, z;
  z.set(at).sub(eye).normalize();
  y.set(up);
  x.set(y).cross(z).normalize();
  y.set(z).cross(x).normalize();

  _11 = x.x;  _12 = y.x;  _13 = -z.x;   _14 = 0;
  _21 = x.y;  _22 = y.y;  _23 = -z.y;   _24 = 0;
  _31 = x.z;  _32 = y.z;  _33 = -z.z;   _34 = 0;
  _41 = -x.dot(eye);
              _42 = -y.dot(eye);
                          _43 = z.dot(eye);
                                        _44 = 1;

  return *this;
}


}
