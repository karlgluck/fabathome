/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009, Kevin Hobson (krh65@cornell.edu)

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

#include <QList>

#include "amftriangle.h"
#include "shared/fabathome-constants.h"

class AMFVertex;
class AMFRegion;

class AMFMesh {
public:

  /**
   * Default constructor
   * Creates an empty AMF mesh
   */
  AMFMesh();

  /**
   * Creates a deep-copy of the given mesh
   */
  explicit AMFMesh(const AMFMesh* copy_source);

  /**
   * Destructor
   * Frees all memory held by this AMF mesh
   */
  ~AMFMesh();

  /**
   * Fills the AMF mesh with the data in the object XML element
   * @param meshElement the XML element containing the mesh data
   * @see WriteXml()
   */
  void ReadFromXml(QDomElement& meshElement);

  /**
   * Outputs the AMF mesh data to an XML element
   * @param meshElement the output XML element
   * @see ReadFromXml()
   */
  void WriteXml(QDomElement& meshElement);

  /**
   * Determines whether or not this is a valid AMF mesh
   * If all AMF regions contained by this AMF mesh are closed regions, then this is a valid AMF mesh
   * @return true if this is a valid AMF mesh, false otherwise
   */
  bool Validate();

  /**
   * Sets this AMF mesh to have the given triangles and vertices
   * Deletes any current triangles and vertices that make up this mesh, and replaces them with newTriangles and newVertices
   * @param newTriangles the new AMF triangles for this mesh
   * @param newVertices the new AMF vertices for this mesh
   * @see GetVertices()
   */
  void Fill(QList<AMFTriangle*>& newTriangles, QList<AMFVertex*>& newVertices);

  void setVertices(QList<AMFVertex*>& new_vertices);

  /**
   * Gets the AMF vertices that make up this AMF mesh
   * @return the list of AMF vertices in this AMF mesh
   * @see Fill()
   */
  const QList<AMFVertex*>* GetVertices() const;

  /**
   * Gets the AMF regions that make up this AMF mesh
   * @return the list of AMF regions in this AMF mesh
   * @see Fill()
   * @todo MAKE THIS RETURN A REFERENCE & INSTEAD OF A POINTER
   */
  QList<AMFRegion*>* GetRegions();
  const QList<AMFRegion*>* GetRegions() const;

  /**
    * Fills the output list with all of the triangles in the mesh.
    * @param output the output list of triangles in this mesh
    * @see GetVertices(), GetRegions()
    */
  void getAllTriangles(QList<FAHTriangle>* output);

  /**
    * Multiplies each vertex's position by this amount, then translates the mesh to be
    * entirely in the first quadrant.
    * @param amount the amout to scale each vertex's position
    */
  void scaleAndPutInQ1(float amount);

  /**
    * Translates the mesh to be entirely in positive Z
    */
  void moveToPositiveZ();

   /**
    * Applies the transformation matrix to each vertex in this mesh, and fills
    * the given destination with the resulting geometry.  The destination will
    * be identical to this one in terms of material/object structure.
    */
  void createTransformedCopy(const FAHMatrix4x4& matrix, AMFMesh* destination);

private:
   QList<AMFVertex*> vertices;
   QList<AMFRegion*> regions;

   // implicit copy constructors are illegal
   AMFMesh(const AMFMesh& other);
};
