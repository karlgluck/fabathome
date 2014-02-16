/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009, Kevin Hobson (krh65@cornell.edu)
               2010, Karl Gluck (kwg8@cornell.edu)

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

#include <QSet>
#include <QDomNode>
#include <QMap>

#include "AMFTriangle.h"
#include "AMFVertex.h"
#include "EdgeKey.h"

class AMFMesh;

class AMFRegion {
public:

  /**
   * Default constructor
   * Creates an empty AMF region
   */
  explicit AMFRegion(AMFMesh* parent_mesh);

  /**
   * Duplicates the given AMF region
   */
  explicit AMFRegion(AMFMesh* parent_mesh, const AMFRegion* copy_source);

  /**
   * Destructor
   * Frees all memory held by this AMF region
   */
  ~AMFRegion();

  /**
   * Fills the AMF region with the data in the region XML element
   * @param regionElement the XML element containing the region data
   * @see WriteXml()
   */
  void readFromXml(QDomElement& regionElement);

  /**
   * Outputs the AMF region data to an XML element
   * @param regionElement the output XML element
   * @see ReadFromXml()
   */
  void writeXml(QDomElement& regionElement);

  /**
   * Determines whether or not this is a valid AMF region
   * An AMF region is valid if it is closed
   * @param tris the triangles that form the region to be validated -- krh shouldn't this just use the region's internal triangles?
   * @return true if this is a valid AMF region, false otherwise
   */
  bool validate(const QList<AMFTriangle*>& tris) const;

  /**
   * Replaces this AMF region's triangles with a new list of triangles
   * Deletes this AMF region's current triangles and sets it to have the new list of triangles
   * @param newTriangles the new list of triangles for this region
   * @see getTriangles()
   */
  void FillTriangles(QList<AMFTriangle*> & newTriangles);

  /**
   * Get this AMF region's list of triangles
   * @return this AMF region's list of triangles
   * @see FillTriangles()
   */
  QList<AMFTriangle*>& getTriangles();

  /**
   * Get this AMF region's list of triangles (const)
   * @return this AMF region's list of triangles (const)
   * @see FillTriangles()
   */
  const QList<AMFTriangle*>& getTriangles() const;

  void setFillMaterialID(int material_id);

  /**
  * Get this AMF region's fill material id
  * @return this regions's fill material id
  */
  int getFillMaterialId() const;

  /**
   * Retrieves the mesh that owns the vertices referenced by this region
   */
  AMFMesh* getParent();
  const AMFMesh* getParent() const;

  /**
   * Fills the output list with all of the triangle positions in this region
   */
  void getTriangles(QList<FAHTriangle>* triangles_out) const;

private:

  /// Reference to the mesh that has the vertices referenced by this region
  AMFMesh* parent_mesh_;

  int fillMaterialId;
  QList<AMFTriangle*> triangles_;
  bool validateEdge(QMap<EdgeKey, bool>* visitedEdges,
                    QMap<EdgeKey, bool>* doneEdges,
                    int v1,
                    int v2) const;

  AMFRegion(const AMFRegion&);
  void operator =(const AMFRegion& );
};
