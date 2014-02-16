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

#include "float.h"
#include "structs.h"


namespace Math {

/**
 * Represents a point, ray or normal in 3d space.  The tolerance is a
 * single-parameter template class that has kTolerance and kToleranceSq
 * const static member variables.
 */
struct Vector3 {

  // Coordinate values
  Float x, y, z;

  /**
   * Constructor, basic
   */
  Vector3();

  /**
   * Constructor
   *
   * Clones input v
   */
  Vector3(const Vector3& v);

  /**
   * Makes this vector3 identical to input v
   */
  explicit Vector3(const Vector3 v[3]);

  /**
   * Sets this vector3's x, y, and z to the Floats in the input array
   */
  explicit Vector3(const Float* v);

  /**
   * Sets this vectors' x, y, and z to the respective inputs
   */
  explicit Vector3(Float x, Float y, Float z);

  /**
   * Accesses an element by an index:  {0: x, 1: y, 2: z}
   */
  Float& operator [] (int ix);

  /**
   * Accesses an element by an index:  {0: x, 1: y, 2: z}
   */
  Float operator [] (int ix) const;

  /**
   * Provides access to this vector as an array {x, y, z}
   */
  Float* array();

  /**
   * Provides access to this vector as an array {x, y, z}
   */
  const Float* array() const;

  /**
   * Returns the magnitude of this vector.
   * This method involves taking a square root, which can be an expensive
   * operation, so substitute lengthSq whenever possible.
   */
  Float length() const;

  /**
   * Returns the square of the magnitude of this vector
   */
  Float lengthSq() const;

  /**
   * Returns 'true' if this vector has unit length
   */
  bool isNormalized() const;

  /**
   * Detects if the vector has no magnitude (can't be normalized)
   */
  bool isInvalid() const;

  /**
   * Normalizes this vector in place.
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& normalize();

  /**
   * Adds each element of 'v' to this object's corresponding element.
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& add(const Vector3& v);

  /**
   * Subtracts each element of 'v' from this object's corresponding
   * element.
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& sub(const Vector3& v);

  /**
   * Returns the dot product of this vector with the v vector
   */
  Float dot(const Vector3& v) const;

  /**
   * Sets this object to the cross product of this vector with the provided
   * vector.
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& cross(const Vector3& v);

  /**
   * Removes all components of this vector not in the direction of 'normal'.
   * 'normal' must be normalized before invoking this method.
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& project(const Vector3& normal);

  /**
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& set(const Vector3& v);

  /**
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& set(const Vector3* v);

  /**
   * Returns a reference to a newly created object
   * with x, y, and z set to the respective input params to allow method chaining.
   */
  Vector3& set(Float x, Float y, Float z);

  /**
   * Sets this vector to the weighted combination of itself and the
   * provided vector.
   * This can be used for linear interpolation or geometric approach.
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& mix(const Vector3& v, float weight);

  /**
   * Stores the normalized vector in the direction of 'end' from 'start'
   * in this object.
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& ray(const Vector3& start, const Vector3& end);

  /**
   * Returns a duplicate of this vector as an object.
   */
  Vector3 copy() const;

  /**
   * Returns the distance from this point in space to 'v'.
   * This method involves taking a square root, which can be an expensive
   * operation, so substitute distanceToSq whenever possible.
   */
  Float distanceTo(const Vector3& v) const;

  /**
   * Returns the square of the distance from this point in space to 'v'
   */
  Float distanceToSq(const Vector3& v) const;

  /**
   * Returns the length of this vector.  This calculation involves a square-root
   * so prefer magnitudeSq whenever possible.
   */
  Float magnitude() const;

  /**
   * Returns the square of the length of this vector
   */
  Float magnitudeSq() const;

  /**
   * Resets the Float to the origin
   */
  Vector3& zero();

  /**
   * Multiplies the magnitude of this vector by the given amount
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& scale(Float s);

  /**
   * Multiplies each component in this vector by the corresponding one in the
   * input vector
   */
  Vector3& scalePairwise(const Vector3& s);

  /**
   * Ensures this vector is entirely in the first quadrant (each component's
   * value is greater than or equal to zero).
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& abs();

  /**
   * Sets each component to the lower among itself and the corresponding
   * component of 'v'.
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& min(const Vector3& v);

  /**
   * Sets each component to the higher among itself and the corresponding
   * component of 'v'.
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& max(const Vector3& v);

  /**
   * Rotates this location around the X-axis
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& rotateX(Float radians);

  /**
   * Rotates this location around the Y-axis
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& rotateY(Float radians);

  /**
   * Rotates this location around the Z-axis
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& rotateZ(Float radians);

  /**
   * Rotates this location around the given arbitrary axis
   * Returns a reference to this object to allow method chaining.
   */
  Vector3& rotateAxisAngle(const Vector3& axis, Float radians);

  /**
   * Radians of rotation between this vector and another
   */
  Float angleTo(const Vector3& v) const;

  /**
   * In-place operator. Returns *this.
   */
  Vector3& operator = (const Vector3& v);

  /**
   * In-place operator. Returns *this.
   */
  Vector3& operator += (const Vector3& v);

  /**
   * In-place operator. Returns *this.
   */
  Vector3& operator -= (const Vector3& v);

  /**
   * In-place operator. Returns *this.
   */
  Vector3& operator *= (Float s);

  /**
   * In-place operator. Returns *this.
   */
  Vector3& operator /= (Float s);

  /**
   * Operators returning a temporary object
   */
  Vector3 operator + (const Vector3& v) const;
  Vector3 operator - (const Vector3& v) const;

  /**
   * Vector-Vector boolean operator.  Equality is based on square of the
   * radial distance from this vector to 'v' being floatEquals to 0.
   */
  bool equals(const Vector3& v) const;

  /**
   * Vector-Vector boolean operator.  Equality is based on some tolerance.
   */
  bool equals(const Vector3& v, float tolerance) const;

  /**
   * Vector-Vector boolean operator.  Equality is based on some tolerance.
   */
  bool equalsSq(const Vector3& v, float tolerance_sq) const;

  /**
   * Lazy operation to make a new vector for an call
   */
  static Vector3 make(float x, float y, float z);
};



}


Math::Vector3 operator * (const Math::Vector3& lhs, Math::Float s);
Math::Vector3 operator / (const Math::Vector3& lhs, Math::Float s);
Math::Vector3 operator * (Math::Float s, const Math::Vector3& v);

