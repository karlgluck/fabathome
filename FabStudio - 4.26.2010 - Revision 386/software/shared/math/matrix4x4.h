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
#pragma once

#include "vector3.h"
#include "structs.h"

namespace Math {


/**
 * This matrix class is used to represent 3d transformations
 */
 struct Matrix4x4 {

  /// Members can be accessed either via array indices or through member
  /// variables.  Elements are stored in row-major order.
  union {
    Float m[4][4];
    struct {
      Float _11, _12, _13, _14;
      Float _21, _22, _23, _24;
      Float _31, _32, _33, _34;
      Float _41, _42, _43, _44;
    };
  };

  /**
   * Default constructor does nothing
   */
  Matrix4x4();

  /**
   * Copy constructor
   */
  Matrix4x4(const Matrix4x4& m);

  /**
   * Copy constructor from pointer
   */
  explicit Matrix4x4(const Matrix4x4* m);

  /**
   * Build from set of float values
   */
  explicit Matrix4x4(const Float v[16]);

  /**
   * Resets the contents of this matrix
   */
  Matrix4x4& zero();

  /**
   * Sets this matrix to be the identity matrix
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& identity();

  /**
   * Returns a new object filled with the same values as this matrix.
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4 copy() const;

  /**
   * Fills this matrix with values from another matrix
   */
  Matrix4x4& set(const Matrix4x4& m);

  /**
   * Fills this matrix with values from another matrix
   */
  Matrix4x4& set(const Matrix4x4* m);

  /**
   * Fills this matrix with values from another matrix
   */
  Matrix4x4& set(const Float v[16]);

  /**
   * Accesses a row array by an index
   */
  Float* operator [] (int ix);

  /**
   * Accesses a row array by an index
   */
  const Float* operator [] (int ix) const;

  /**
   * Returns the internal float array
   */
  Float* array();

  /**
   * Sets this matrix to the result of (this * m).
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& mul(const Matrix4x4& m);

  /**
   * Returns a version of the input vector transformed by this matrix
   */
  Vector3 transform(const Vector3& v) const;

  /**
   * Transforms the given vector, and sets that vector to the transformed value
   */
  void transformInPlace(Vector3* v) const;

  /**
   * Initializes this structure as a translation matrix
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& translation(const Vector3& v);

  /**
   * Initializes this structure as a rotation matrix about the X axis
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& rotationX(Float a);

  /**
   * Initializes this structure as a rotation matrix about the Y axis
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& rotationY(Float a);

  /**
   * Initializes this structure as a rotation matrix about the Z axis
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& rotationZ(Float a);

  /**
   * Initializes this structure as a rotation matrix about the given axis
   * by the requested angle.  The axis must be normalized.
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& rotationAxisAngle(const Vector3& axis, Float angle);

  /**
   * Initializes this structure as a rotation matrix defined by
   * the given quaternion.  The input must be a unit quaternion.
   */
  Matrix4x4& rotationQuaternion(const Quaternion& q);

  /**
   * Initializes this structure as a rotation matrix with components
   * about each x/y/z axes
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& rotationEuler(const Vector3& v);

  /**
   * Initializes this structure as a rotation matrix with components
   * about each x/y/z axes
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& rotationEuler(Float x, Float y, Float z);

  /**
   * Initializes this structure as a scaling matrix with individual x/y/z components
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& scaling(const Vector3& v);

  /**
   * Initializes this structure as a scaling matrix with individual x/y/z components
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& scaling(Float x, Float y, Float z);

  /**
   * Initializes this structure as a scaling matrix where x=y=z=s
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& scalingUniform(Float s);

  /**
   * Sets this matrix to the inverse of the current matrix.  If the matrix
   * is uninvertible (i.e. the determinant is zero) a 'confirm' error will
   * be triggered and the matrix will remain unchanged.
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& invert();

  /**
   * Sets this matrix to the inverse of the current matrix.  This involves
   * calculating the determinant, so if you provide the determinant
   * parameter it will be filled in for free.  If the determinant is zero,
   * this matrix is uninvertible.
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& invert(Float* determinant);

  /**
   * Calculates the determinant of this matrix
   */
  Float determinant() const;

  /**
   * Swaps rows with columns in this matrix
   * Returns a reference to this object to allow method chaining.
   */
  Matrix4x4& transpose();

  /**
   * Sets this matrix to a matrix that transforms vertices from camera space
   * to screen space such that objects with a greater depth are smaller than
   * those with less depth.
   * Returns a reference to this object to allow method chaining.
   * @see http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
   */
  Matrix4x4& glProjectionPerspective(Float fov, Float aspect, Float near_z, Float far_z);

  ///**
  // * Sets this matrix to a matrix that transforms vertices from camera space
  // * to screen space such that objects are not distorted by their distance
  // * from the camera.
  // * Returns a reference to this object to allow method chaining.
  // */
  //Matrix4x4& glProjectionOrthographic();

  /**
   * Builds a left-handed view matrix that transforms vertices from 3d space
   * to camera space.
   * Returns a reference to this object to allow method chaining.
   * @see http://www.opengl.org/sdk/docs/man/xhtml/gluLookAt.xml
   */
  Matrix4x4& glView(const Vector3& eye,
                    const Vector3& at,
                    const Vector3& up);
};



}
