/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009, Kevin Hobson (krh65@cornell.edu)
                     Sonica Li (syl32@cornell.edu)

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
#include "amfregion.h"
#include "edgekey.h"
#include <QDebug> // for qDebug()
#include "common/todo.h"
#include "amfmesh.h"



AMFRegion::AMFRegion(AMFMesh* parent_mesh) : parent_mesh_(parent_mesh) {
}

AMFRegion::AMFRegion(AMFMesh* parent_mesh, const AMFRegion* copy_source) : parent_mesh_(parent_mesh) {
  fillMaterialId = copy_source->fillMaterialId;
  foreach (const AMFTriangle* t, copy_source->triangles_) {
    triangles_.append(new AMFTriangle(t));
  }
}

AMFRegion::~AMFRegion() {
  for (int i = 0; i < triangles_.size(); i ++) {
    delete triangles_[i];
  }
}

void AMFRegion::readFromXml(QDomElement& regionElement) {
  // Read the material id
  QString matIdStr = regionElement.attribute("FillMaterialID");
  if (!matIdStr.isEmpty()) {
    fillMaterialId = matIdStr.toInt();
  }

  // Parse each triangle element
  QDomNodeList triangleNodes = regionElement.elementsByTagName("Triangle");
  for (unsigned int i = 0; i < triangleNodes.length(); i++) {
    AMFTriangle* newTriangle = new AMFTriangle();
    triangles_.append(newTriangle);
    QDomElement triangleElement = triangleNodes.at(i).toElement();
    newTriangle->ReadFromXml(triangleElement);
  }
}

void AMFRegion::writeXml(QDomElement& regionElement) {
  QDomDocument theDoc = regionElement.ownerDocument();

  // Write the fill material id
  regionElement.setAttribute("FillMaterialID", fillMaterialId);

  // Write the triangles
  for (int i = 0; i < triangles_.size(); i++) {
    QDomElement triangleElement = theDoc.createElement("Triangle");
    triangles_[i]->WriteXml(triangleElement);
    regionElement.appendChild(triangleElement);
  }
}

void AMFRegion::FillTriangles(QList<AMFTriangle*> & newTriangles)
{

   // Clear the list of triangles
   int i;
   for (i = 0; i < triangles_.size(); i ++)
   {
      delete(triangles_[i]);
   }
   triangles_.clear();

   // Add the new triangles
   for (i = 0; i < newTriangles.size(); i++)
   {
      triangles_.append(newTriangles.at(i));
   }
}

/* Everything below this line was written by
 * syl32. Please add a todo for me here if there
 * are memory leaks or seg faults so that I can
 * learn from my mistakes. Thanks.
 */


/* algorithm for validation:
 * for each triangle
 *   for each edge in triangle
 *     check if that edge or its partner has appeared before,
 *     failing if an edge appears more than once or its
 *     partner never appears.
 * runtime: O(number of triangles)
 */
bool AMFRegion::validate(const QList<AMFTriangle*> &tris) const {
  typedef QMap<EdgeKey, int> EdgeMap;
  EdgeMap all_edges;

  for (int i = 0; i < tris.size(); ++i) {
    const AMFTriangle *tri = tris[i];
    EdgeKey ek;
    for (int j = 0; j < 3; ++j) {
      switch (j) {
      case 0:
        ek.v1 = tri->getV1();
        ek.v2 = tri->getV2();
        break;
      case 1:
        ek.v1 = tri->getV2();
        ek.v2 = tri->getV3();
        break;
      case 2:
        ek.v1 = tri->getV3();
        ek.v2 = tri->getV1();
        break;
      }/*
    for (int j = 0; j < 3; ++j) {
      switch (j) {
      case 0:
        ek.v1 = tri->getV1();
        ek.v2 = tri->getV2();
        break;
      case 1:
        ek.v1 = tri->getV2();
        ek.v2 = tri->getV3();
        break;
      case 2:
        ek.v1 = tri->getV3();
        ek.v2 = tri->getV1();
        break;
      }*/
      if (all_edges.contains(ek)) {
        all_edges.insert(ek, 1 + all_edges.take(ek));
      } else {
        all_edges.insert(ek, 1);
      }/*
      if (all_edges.contains(ek)) {
        all_edges.insert(ek, 1 + all_edges.take(ek));
      } else {
        all_edges.insert(ek, 1);
      }*/

      EdgeKey ek_reverse(ek.v2, ek.v1);
      if (all_edges.contains(ek_reverse)) {
        all_edges.insert(ek_reverse, 1 + all_edges.take(ek_reverse));
      } else {
        all_edges.insert(ek_reverse, 1);
      }/*
      EdgeKey ek_reverse(ek.v2, ek.v1);
      if (all_edges.contains(ek_reverse)) {
        all_edges.insert(ek_reverse, 1 + all_edges.take(ek_reverse));
      } else {
        all_edges.insert(ek_reverse, 1);
      }*/
    }
  }

//  for (EdgeMap::const_iterator j = all_edges.begin(); j != all_edges.end(); ++j) {
//    if (j.value() != 2) {
//      qDebug() << j.value() << ": " << j.key().v1 << " " << j.key().v2;
//    }
//  }

  return true;
}
/*
  QMap<EdgeKey, bool> visitedEdges, doneEdges;

  // test each triangle
  for (int i = 0; i < tris.size(); ++i) {
    const AMFTriangle *tri = tris[i];

    // each edge
    if (!validateEdge(&visitedEdges, &doneEdges, tri->getV1(), tri->getV2()) ||
        !validateEdge(&visitedEdges, &doneEdges, tri->getV2(), tri->getV3()) ||
        !validateEdge(&visitedEdges, &doneEdges, tri->getV3(), tri->getV1())) {
      return false;
    }
  }

  // check for lone edges
  if (!(visitedEdges.isEmpty())) {
    qDebug() << "loner " << visitedEdges.size();
    return false;
  }

  return true;
}*/


QList<AMFTriangle*>& AMFRegion::getTriangles() {
  return triangles_;
}

void AMFRegion::getTriangles(QList<FAHTriangle>* triangles_out) const {
  const QList<AMFVertex*>* vertices = parent_mesh_->GetVertices();

  for (int t = 0; t < triangles_.size(); ++t) {
    const AMFTriangle* triangle = triangles_[t];
    if (triangle->isDegenerate()) {
      continue;
    }
    FAHTriangle triangle_out(
      vertices->at(triangle->getV1())->getPoint(),
      vertices->at(triangle->getV2())->getPoint(),
      vertices->at(triangle->getV3())->getPoint());
    triangles_out->append(triangle_out);
  }
}

const QList<AMFTriangle*>& AMFRegion::getTriangles() const {
  return triangles_;
}


bool AMFRegion::validateEdge(QMap<EdgeKey, bool> *visitedEdges,
                             QMap<EdgeKey, bool> *doneEdges,
                             int v1, int v2) const {

  EdgeKey edgeKey(v1, v2);
  EdgeKey revEdgeKey(v2, v1);

  // edge already exists as a loner - FAIL
  if (visitedEdges->contains(edgeKey)) {
    //qDebug() << "edge already exists " << v1 << " " << v2;
    return false;
  }

  if (visitedEdges->contains(revEdgeKey)) {

    // edge and partner have been seen previously - FAIL
    if (doneEdges->contains(edgeKey)) {
      //qDebug() << "pair already exists " << v1 << " " << v2;
      return false;
    }

    // record having visited edge and partner
    doneEdges->insert(edgeKey, true);
    doneEdges->insert(revEdgeKey, true);
    visitedEdges->remove(edgeKey);
    visitedEdges->remove(revEdgeKey);
    //qDebug() << "recording pair " << v1 << " " << v2;
  }
  else {
    visitedEdges->insert(edgeKey, true);
    //qDebug() << "recording edge " << v1 << " " << v2;
  }

  return true;
}

void AMFRegion::setFillMaterialID(int material_id) {
  fillMaterialId = material_id;
}

int AMFRegion::getFillMaterialId() const {
  return fillMaterialId;
}


AMFMesh* AMFRegion::getParent() {
  return parent_mesh_;
}

const AMFMesh* AMFRegion::getParent() const {
  return parent_mesh_;
}
