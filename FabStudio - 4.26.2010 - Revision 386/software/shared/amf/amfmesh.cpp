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
#include <QDomNode>
#include <QList>

#include "common/todo.h"
#include "amfmesh.h"
#include "amftriangle.h"
#include "amfvertex.h"
#include "amfregion.h"

AMFMesh::AMFMesh() { }

AMFMesh::AMFMesh(const AMFMesh* copy_source) {
  foreach (const AMFVertex* v, copy_source->vertices) {
    vertices.append(new AMFVertex(v->getPoint(), v->getVertexID()));
  }
  foreach (const AMFRegion* r, copy_source->regions) {
    regions.append(new AMFRegion(this, r));
  }
}

AMFMesh::~AMFMesh() {
  // Delete the vertices
  int i;
  for (i = 0; i < vertices.size(); i++) {
    delete vertices[i];
  }

  // Delete the regions
  for (i = 0; i < regions.size(); i++) {
    delete regions[i];
  }
}

void AMFMesh::ReadFromXml(QDomElement& meshElement) {
  // Read in the vertices
  QDomElement verticesElement = meshElement.firstChildElement("Vertices");
  QDomNodeList vertexNodeList = verticesElement.elementsByTagName("Vertex");

  int i;
  for (i = 0; i < vertexNodeList.size(); i++) {
    AMFVertex* newAmfVertex = new AMFVertex();
    vertices.append(newAmfVertex);
    QDomElement vertexElement = vertexNodeList.at(i).toElement();
    newAmfVertex->ReadFromXml(vertexElement);
  }

  // Now read the regions
  QDomNodeList regionNodeList = meshElement.elementsByTagName("Region");
  for (i = 0; i < regionNodeList.size(); i++) {
    AMFRegion* newAmfReg = new AMFRegion(this);
    regions.append(newAmfReg);
    QDomElement regionElement = regionNodeList.at(i).toElement();
    newAmfReg->readFromXml(regionElement);
  }

}

void AMFMesh::WriteXml(QDomElement& meshElement) {
  QDomDocument theDoc = meshElement.ownerDocument();

  // Create the vertices element
  QDomElement verticesElement = theDoc.createElement("Vertices");
  meshElement.appendChild(verticesElement);

  // Create the vertex elements
  for (int i = 0; i < vertices.size(); i++) {
    QDomElement vertexElement = theDoc.createElement("Vertex");
    vertices[i]->WriteXml(vertexElement);
    verticesElement.appendChild(vertexElement);
  }

  // Create the region elements
  for (int i = 0; i < regions.size(); i++) {
    QDomElement regionElement = theDoc.createElement("Region");
    regions[i]->writeXml(regionElement);
    meshElement.appendChild(regionElement);
  }
}

void AMFMesh::Fill(QList<AMFTriangle*> & newTriangles,
                   QList<AMFVertex*> & newVertices) {

  // Delete the old vertices and regions
  int i;
  for (i = 0; i < vertices.size(); i++) {
    delete(vertices[i]);
  }
  vertices.clear();

  for (i = 0; i < regions.size(); i++) {
    delete(regions[i]);
  }
  regions.clear();

  // Fill the list of vertices
  for (i = 0; i < newVertices.size(); i++) {
    vertices.append(newVertices.at(i));
  }

  // Fill a new region
  AMFRegion* newRegion = new AMFRegion(this);
  newRegion->FillTriangles(newTriangles);
  regions.append(newRegion);
}

void AMFMesh::setVertices(QList<AMFVertex*>& new_vertices) {

  // Delete the old vertices
  int i;
  for (i = 0; i < vertices.size(); i++) {
    delete(vertices[i]);
  }
  vertices.clear();

  vertices += new_vertices;
}

const QList<AMFVertex*>* AMFMesh::GetVertices() const {
  return &vertices;
}


bool AMFMesh::Validate() {
  for (int i = 0; i < regions.size(); i++) {
    const AMFRegion& region = *regions[i];
    const QList<AMFTriangle*>& tris = region.getTriangles();
    if (!(region.validate(tris))) {
      return false;
    }
  }
  return true;
}

QList<AMFRegion*>* AMFMesh::GetRegions() {
  return &regions;
}

const QList<AMFRegion*>* AMFMesh::GetRegions() const {
  return &regions;
}


void AMFMesh::getAllTriangles(QList<FAHTriangle>* output) {
  for (int i = 0; i < regions.size(); ++i) {
    const AMFRegion* region = regions[i];
    const QList<AMFTriangle*>& tris = region->getTriangles();
    for (int t = 0; t < tris.size(); ++t) {
      const AMFTriangle* triangle = tris[t];
      if (triangle->isDegenerate()) {
        continue;
      }
      FAHTriangle triangle_out(
        vertices.at(triangle->getV1())->getPoint(),
        vertices.at(triangle->getV2())->getPoint(),
        vertices.at(triangle->getV3())->getPoint());
      output->append(triangle_out);
    }
  }
}

void AMFMesh::scaleAndPutInQ1(float amount) {
  FAHVector3 min;

  // adjust all locations, and find the lowest coordinate
  for (int i = 0; i < vertices.size(); ++i) {
    vertices[i]->getPoint().scale(amount);
    if (i == 0) {
      min.set(vertices[i]->getPoint());
    } else {
      min.min(vertices[i]->getPoint());
    }
  }

  // translate to positive space
  for (int i = 0; i < vertices.size(); ++i) {
    vertices[i]->getPoint().sub(min);
  }
}


void AMFMesh::moveToPositiveZ() {
  FAHVector3 min;

  // find the lowest coordinate
  for (int i = 0; i < vertices.size(); ++i) {
    if (i == 0) {
      min.set(vertices[i]->getPoint());
    } else {
      min.min(vertices[i]->getPoint());
    }
  }

  // translate to positive space
  for (int i = 0; i < vertices.size(); ++i) {
    vertices[i]->getPoint().z -= min.z;
  }
}

void AMFMesh::createTransformedCopy(const FAHMatrix4x4& matrix, AMFMesh* destination) {
  todo("kwg8", "clear out destination mesh");
  foreach (AMFVertex* v, vertices) {
    FAHVector3 point = v->getPoint();
    matrix.transformInPlace(&point);
    destination->vertices.append(new AMFVertex(point, v->getVertexID()));
  }
  foreach (AMFRegion* r, regions) {
    // duplicate each region with its copy constructor
    destination->regions.append(new AMFRegion(destination, r));
  }
}
