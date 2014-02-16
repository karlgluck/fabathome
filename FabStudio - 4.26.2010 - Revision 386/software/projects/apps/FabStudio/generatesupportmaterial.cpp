/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2010, Karl Gluck (kwg8@cornell.edu)
               2010, Jason Zhao (jlz27@cornell.edu)

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
#include "generatesupportmaterial.h"
#include "shared/amf/amfregion.h"
#include "shared/amf/amfmesh.h"
#include "shared/fabathome-constants.h"

#include <common/todo.h>

bool requiresSupportMaterial(const QList<AMFRegion*> printing_regions) {
    todo("jlz27", "implement an algorithm in the requiresSupportMaterial method");

    // possible algorithm: determine if the silhouette (defined by the
    // edges between upward-facing to downward-facing triangles) is contained
    // by the edges of the base of the object
    return true;
}


void generateSupportingMesh(const QList<AMFRegion*> printing_regions,
                            AMFMesh* output_mesh) {

    // find the overall bounding box
    //  NOTE: always set the bottom of the box to z=0
    // create the vertices for the bounding box
    // make a new region with indices of bounding box, add to output_mesh
    // append bounding box vertices to the output vertex list
    const int buffer = 5;
    double xMin, xMax, yMin, yMax, zMax = 0;

    QList<AMFVertex*> outVertices;

    /*Make sure there are no identical regions*/
    QSet<AMFMesh*> meshes;
    for (int i = 0; i < printing_regions.size(); ++i) {
      meshes.insert(printing_regions[i]->getParent());
    }
    QList<AMFMesh*> mesh = meshes.toList();
    QList<int> listSize;

    /*Initializing the x,y,z values with first value in mesh*/
    xMin = mesh[0]->GetVertices()->first()->getPoint().x;
    xMax = xMin;
    yMin = mesh[0]->GetVertices()->first()->getPoint().y;
    yMax = yMin;
    zMax = 0;

    /*Finding the xMin, xMax, yMin, yMax, zMax values and creating the new vertices list*/
    for (int i = 0; i < mesh.size(); ++i) {
        const QList<AMFVertex*> *vertex = mesh[i]->GetVertices();
        listSize.append(vertex->size());
        for (int j = 0; j < vertex->size(); ++j) {
            if (xMin > vertex->at(j)->getPoint().x)
                xMin = vertex->at(j)->getPoint().x;
            if (xMax < vertex->at(j)->getPoint().x)
                xMax = vertex->at(j)->getPoint().x;
            if (yMin > vertex->at(j)->getPoint().y)
                yMin = vertex->at(j)->getPoint().y;
            if (yMax < vertex->at(j)->getPoint().y)
                yMax = vertex->at(j)->getPoint().y;
            if (zMax < vertex->at(j)->getPoint().z)
                zMax = vertex->at(j)->getPoint().z;
            outVertices.append(new AMFVertex(vertex->at(j)->getPoint(), outVertices.size()));
        }
    }

    // Adjust the bounding area by the buffer size
    xMin = xMin - buffer;
    xMax = xMax + buffer;
    yMin = yMin - buffer;
    yMax = yMax + buffer;
    zMax = zMax + buffer;

    /*
      2 __________ 3
      |\            \
      | \            \
      |  \            \
      |   6 __________ 7
      0   |       1    |
       \  |            |
        \ |            |
         \4 __________ 5
      */
    const int kTriangles[] = { 0, 1, 2, 2, 1, 3, // back
                               0, 4, 6, 0, 6, 2, // left
                               1, 5, 7, 1, 7, 3, // right
                               4, 5, 6, 6, 5, 7};// front
    int start = outVertices.size();
    outVertices.append(new AMFVertex(FAHVector3::make(xMin, yMin, 0), outVertices.size()));     // 0
    outVertices.append(new AMFVertex(FAHVector3::make(xMax, yMin, 0), outVertices.size()));     // 1
    outVertices.append(new AMFVertex(FAHVector3::make(xMin, yMin, zMax), outVertices.size()));  // 2
    outVertices.append(new AMFVertex(FAHVector3::make(xMax, yMin, zMax), outVertices.size()));  // 3
    outVertices.append(new AMFVertex(FAHVector3::make(xMin, yMax, 0), outVertices.size()));     // 4
    outVertices.append(new AMFVertex(FAHVector3::make(xMax, yMax, 0), outVertices.size()));     // 5
    outVertices.append(new AMFVertex(FAHVector3::make(xMin, yMax, zMax), outVertices.size()));  // 6
    outVertices.append(new AMFVertex(FAHVector3::make(xMax, yMax, zMax), outVertices.size()));  // 7

    QList<AMFTriangle*> outTri;

    /*Generating the new regions list with the outside box and flip the normals of the existing regions*/
    for (int i = 0; i < mesh.size(); ++i) {
        const QList<AMFRegion*> *region = mesh[i]->GetRegions();
        QList<AMFTriangle*> tri;
        for (int j = 0; j < region->size(); ++j) {
            tri.append(region->at(j)->getTriangles());
        }
        int shiftVal = 0;
        for (int p = 0; p < i; ++p) {
            shiftVal = shiftVal + listSize [p];
        }
        for (int j = 0; j < tri.size(); ++j) {
            outTri.append(new AMFTriangle(tri.at(j)->getV1() + shiftVal,
                                          tri.at(j)->getV3() + shiftVal, tri.at(j)->getV2() + shiftVal));
        }
    }

    /*Adding the new external box that will enclose the region*/
    for (int i = 0; i < 8; ++i) {
      outTri.append(new AMFTriangle(start + kTriangles[i*3+0],
                                    start + kTriangles[i*3+2],
                                    start + kTriangles[i*3+1]));
    }

    /*Generating output mesh*/
    output_mesh->Fill(outTri, outVertices);

}

// This method is not used anywhere - kwg8

///**
// * Returns the list of all edges that are on the silhouette of an object when looking down from the top
// */
//QList<FAHLine> findSilhouetteEdges(const AMFRegion* region) {
//  const QList<AMFVertex*>* vertices = region->getParent()->GetVertices();
//  const QList<AMFTriangle*>& triangles = region->getTriangles();
//  QList<FAHLine> silhouette_edges;
//  for (int i = 0; i < triangles.size(); ++i) {
//    const AMFTriangle* i_tri = triangles.at(i);
//    int i_v[] = {i_tri->getV1(), i_tri->getV2(), i_tri->getV3()};
//    for (int j = 0; j < triangles.size(); ++j) {
//      const AMFTriangle* j_tri = triangles.at(i);
//      int j_v[] = {j_tri->getV1(), j_tri->getV2(), j_tri->getV3()};
//
//      // Do these share an edge?
//      const bool share[] = {
//        // ways to share [0,1]                // 0 1 2
//        i_v[0] == j_v[0] && i_v[1] == j_v[1], // 0 1
//        i_v[0] == j_v[1] && i_v[1] == j_v[0], // 1 0
//        i_v[0] == j_v[1] && i_v[1] == j_v[2], // 1 2
//        i_v[0] == j_v[2] && i_v[1] == j_v[1], // 2 1
//        i_v[0] == j_v[2] && i_v[1] == j_v[0], // 2 0
//        i_v[0] == j_v[0] && i_v[1] == j_v[2], // 0 2
//
//        // ways to share [1,2]
//        i_v[1] == j_v[0] && i_v[2] == j_v[1], //   0 1
//        i_v[1] == j_v[1] && i_v[2] == j_v[0], //   1 0
//        i_v[1] == j_v[1] && i_v[2] == j_v[2], //   1 2
//        i_v[1] == j_v[2] && i_v[2] == j_v[1], //   2 1
//        i_v[1] == j_v[2] && i_v[2] == j_v[0], //   2 0
//        i_v[1] == j_v[0] && i_v[2] == j_v[2], //   0 2
//
//        // ways to share [2,0]
//        i_v[2] == j_v[0] && i_v[0] == j_v[1], // 1   0
//        i_v[2] == j_v[1] && i_v[0] == j_v[0], // 0   1
//        i_v[2] == j_v[1] && i_v[0] == j_v[2], // 2   1
//        i_v[2] == j_v[2] && i_v[0] == j_v[1], // 1   2
//        i_v[2] == j_v[2] && i_v[0] == j_v[0], // 0   2
//        i_v[2] == j_v[0] && i_v[0] == j_v[2], // 2   0
//      };
//
//      bool share01 = share[0] || share[1] || share[2] || share[3] || share[4];
//      bool share12 = share[5] || share[6] || share[7] || share[8] || share[9];
//      bool share20 = share[10] || share[11] || share[12] || share[13] || share[14];
//
//      if (share01 || share12 || share20) {
//        FAHTriangle i_t(vertices->at(i_v[0])->getPoint(),
//                        vertices->at(i_v[1])->getPoint(),
//                        vertices->at(i_v[2])->getPoint());
//        FAHTriangle j_t(vertices->at(j_v[0])->getPoint(),
//                        vertices->at(j_v[1])->getPoint(),
//                        vertices->at(j_v[2])->getPoint());
//        FAHVector3 z_positive(0,0,1);
//        if (i_t.normal().dot(z_positive) * j_t.normal().dot(z_positive) > 0) {
//          // ignore, not on the silhouette
//          continue;
//        }
//      }
//
//      if (share01) {
//        FAHLine l(vertices->at(i_v[0])->getPoint(), vertices->at(i_v[1])->getPoint());
//        silhouette_edges.append(l);
//      } else if (share12) {
//        FAHLine l(vertices->at(i_v[1])->getPoint(), vertices->at(i_v[2])->getPoint());
//        silhouette_edges.append(l);
//      } else if (share20) {
//        FAHLine l(vertices->at(i_v[2])->getPoint(), vertices->at(i_v[0])->getPoint());
//        silhouette_edges.append(l);
//      }
//    }
//  }
//  return silhouette_edges;
//}
