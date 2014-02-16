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
#pragma once

#include "float.h"
#include "structs.h"
#include "vector3.h"


namespace Math {

struct Sphere {

  Vector3 center;
  Float radius;

  /**
   * Default initializer; does nothing
   */
  Sphere();

  /**
   * Initialize this sphere
   */
  explicit Sphere(const Vector3& v, Float r);

  /**
   * Copy values from the given sphere
   */
  explicit Sphere(const Sphere& s);

  /**
   * Copy values from the given sphere
   */
  explicit Sphere(const Sphere* s);

  /**
   * Initialize this sphere
   */
  Sphere& set(const Vector3& v, Float r);

  /**
   * Copy values from the given sphere
   */
  Sphere& set(const Sphere& s);

  /**
   * Copy values from the given sphere
   */
  Sphere& set(const Sphere* s);

  /**
   * Sets this sphere to contain the two vertices.  This is usually used to contain the min
   * and max coordinates of a mesh
   */
  Sphere& set(const Vector3& v1, const Vector3& v2);

  /**
   * Returns true if a ray cast from the given origin would intersect this sphere's boundaries
   */
  bool intersects(const Vector3& origin, const Vector3& ray) const;

  /**
   * Finds the distances at which a ray cast from 'origin' would enter and exit the sphere.
   * this method returns the number of intersections, which can be 0, 1, or 2.  1 is returned
   * if the ray just barely skims the surface of the sphere.
   */
  int intersectDistances(const Vector3& origin, const Vector3& ray,
                         float* enter_distance, float* exit_distance) const;

  /**
   * Finds the 3d coordinates of the closer intersection point of a ray cast toward the sphere.
   * Returns 'false' and does not modify 'intersection' if no intersection is found.
   */
  bool intersectNearLocation(const Vector3& origin, const Vector3& ray, Vector3* intersection) const;
};


}


