/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009, Karl Gluck (kwg8@cornell.edu)
                     Shuning Liu (scl82@cornell.edu)

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

#include <QList>
#include "shared/stl/stlfacet.h"
#include "shared/fabathome-constants.h"
#include "shared/amf/amftriangle.h"

/**
 *	Converts an object mesh in STL format into its equivalent AMF formatted
 *	vertices and triangles. STLs represent a 3D object as individual facets and
 *	vertices, with each vertex distinct from all others ("shared" vertices are
 *	only microns distance apart). weld combines vertices that are effectively the 
 *	same point in space into the same vertex and re-indexes all the STL triangles
 *	such that the resulting list of indexed vertices and triangles represent the
 *	same model except condensed as necessary.
 *
 * @param unwelded_vertices a QList of FAHVector pointers that refer to the STL vertices
 * @param unwelded_facets a QList of STLFacet pointers that refer to STL-indexed triangles
 * @param welded_vertices a QList of FAHVector pointers that will contain the AMF-welded
 *		  vertices
 * @param welded_traingles a QList of AMFTriangle pointers that will contain AMF-formatted
 * 		  triangles that refer to the re-indexed vertices in welded_vertices
 * @return true if welding was successful
 */
bool weld(const QList<FAHVector3*>& unwelded_vertices,
          const QList<STLFacet*>& unwelded_facets,
          QList<FAHVector3*>* welded_vertices,
          QList<AMFTriangle*>* welded_triangles) {

  //-Detects self in tree
  //-find nearby vertices, not including self within some weld radius

  //Read off the first QList (of vertices) one at a time
  // -> compare each vertex into the data structure
  // 		-> if already exists, merge
  //		-> if not, create a new one

  FAHVector3 *cur_vertex;
  bool exists = false;

  //for each unwelded vertex, merge together duplicates. Duplicates are determined
  //by Vector3 according to some micro-tolerance
  for (int i = 0; i < unwelded_vertices.size(); i++) {
    exists = false;
    cur_vertex = unwelded_vertices.at(i);

    //performs a linear search of all vertices already added
    //to find duplicates. May require optimization with an octree
    todo("scl82@cornell.edu", "replace linear search with octree");
    int j = 0;
    while (j < welded_vertices->size() && !exists) {
      exists = (*(welded_vertices->at(j)) == *cur_vertex);

      j++;
    }

    if (!exists) {
      welded_vertices->append(cur_vertex);
    }
  }

  exists = false;
  AMFTriangle cur_triangle;
  STLFacet cur_facet;
  FAHTriangle cur_stl_triangle, temp_triangle;
  float normal_dot;

  struct MicronTolerance microtolerance;

  //Go through each unwelded facet
  for (int i = 0; i < unwelded_facets.size(); i++) {
    cur_facet = *(unwelded_facets.at(i));
    cur_stl_triangle = cur_facet.triangle; //triangle to be re-indexed

    //Shove into a Triangle
    //Use Triangle.normal()


    //Indexing the triangles
    for (int j = 0; j < 3; j++) {
      int k = 0;
      //Finds the index at which the correct vertex can be found
      while (k < welded_vertices->size() && !exists) {
        //exists = (*(welded_vertices->at(k)) == *cur_facet[j]);
        exists = welded_vertices->at(k)->equals(cur_facet.triangle.v[j]);

        if (!exists) k++;
      }

      //Create a temporary triangle for validating normal
      //temp_triangle[j] = (*(welded_vertices->at(k)));
      temp_triangle.v[j].set(welded_vertices->at(k));

      //Sets the vertex indices on the AMFtriangle
      switch (j) {
      case 0:
      {
        cur_triangle.setV1(k);
        break;
      }
      case 1:
      {
        cur_triangle.setV2(k);
        break;
      }
      case 2:
      {
        cur_triangle.setV2(k);
        break;
      }
      default: /*Error check*/ break;
      }

    }

    //Dotting the two normals together to determine if the normal is correct
    normal_dot = (cur_facet.normal).dot(temp_triangle.normal());


    //If the normal is not correct, then arbitrarily switch two of the vertices
    //to create the correct normal.
    if (normal_dot < (1 - microtolerance.kTolerance)) {
      int v1 = cur_triangle.getV1();
      cur_triangle.setV1(cur_triangle.getV2());
      cur_triangle.setV2(v1);
    }

    //Add the newly created welded triangle to the list
    welded_triangles->append(new AMFTriangle(cur_triangle));
  }

  return true;
}
