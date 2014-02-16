/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009, Sean Cretella (sac76@cornell.edu)
               2010, Karl Gluck (kwg8@cornell.edu)
                     Nathan Lloyd (nsl6@cornell.edu)
                     Jimmy Liu (jl2222@cornell.edu)
                     Karina Sobhani (ks598@cornell.edu)

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

#include "shared/amf/amffile.h"
#include "shared/amf/amfobject.h"
#include "shared/amf/amfvertex.h"
#include "shared/amf/amftriangle.h"
#include "shared/amf/amfregion.h"

#include "STLFile.h"
#include "stlfacet.h"
#include <qtextstream.h>
#include <qfile.h>
#include <qstringlist.h>
#include <iostream>
#include <stdio.h>
#include "common/todo.h"
#include <QBitmap>
#include <QPixmap>
#include <QImageReader>
#include <QRgb>

/**
  Detects the STL type at path and creates a mesh from the STL file.
  @param path  The path and filename to an STL file.
  @return  A boolean that returns true upon successful read.
  **/
bool STLFile::read(QString path) {

  bool is_binary_file;

  QFile stl(path);

  if (!stl.open(QFile::ReadOnly)) {
    std::cout << "Could not open file." << std::endl;
  }
  QDataStream stream(&stl);
  QString line;

  if (stream.atEnd()) {
    // error
    std::cout << "There was an error reading the specified STL file.";
    return false;
  }

  // Detect ASCII vs. binary.
  // Read the last 20 characters of the file and
  // search for "endsolid" (indicates ASCII file).
  char buffer[20];
  stream.skipRawData(stl.size() - 19);
  stream.readRawData(buffer, 20);
  buffer[19] = '\0';
  line = buffer;

  stl.close();
  is_binary_file = !line.contains("endsolid", Qt::CaseInsensitive);

  return is_binary_file ? readBinary(path) : readASCII(path);
}

/**
  Processes an ASCII STL file and stores facet data in mesh.
  @param path  The path and filename to an ASCII STL file.
  @return  A boolean that returns true upon successful read.
  **/
bool STLFile::readASCII(QString path) {

  QFile file(path);

  file.open(QFile::ReadOnly);

  QTextStream stream(&file);
  QString line;

  STLFacet facet;
  // count: keeps track of # of float values read
  //        per line scan (if this is not 3, discard)
  int count;

  // facet normal x y z
  // outer loop
  // vertex x y z
  // vertex x y z
  // vertex x y z
  // endloop
  // endfacet
  // endsolid name_of_object

  line = stream.readLine();
  if (!line.contains("solid", Qt::CaseInsensitive)) {
      //error
      return false;
  }

  while (!stream.atEnd()) {
    line = stream.readLine().trimmed();
    if (line.toLower().contains("endsolid")) {
      // done
      file.close();
      break;
    }

    count = sscanf(line.toStdString().c_str(), "facet normal %lf %lf %lf",
        &facet.normal.x, &facet.normal.y, &facet.normal.z);
    if (count != 3 || stream.atEnd()) {
      return false;
    }
    
    line = stream.readLine().trimmed();
    if (0 != line.compare("outer loop", Qt::CaseInsensitive) || stream.atEnd()) {
      return false;
    }
    line = stream.readLine().trimmed();
    count = sscanf(line.toStdString().c_str(), "vertex %lf %lf %lf",
        &facet.triangle.v[0].x, &facet.triangle.v[0].y, &facet.triangle.v[0].z);
    if (count != 3 || stream.atEnd()) {
      return false;
    }

    line = stream.readLine().trimmed();
    count = sscanf(line.toStdString().c_str(), "vertex %lf %lf %lf",
        &facet.triangle.v[1].x, &facet.triangle.v[1].y, &facet.triangle.v[1].z);
    if (count != 3 || stream.atEnd()) {
      return false;
    }

    line = stream.readLine().trimmed();
    count = sscanf(line.toStdString().c_str(), "vertex %lf %lf %lf",
        &facet.triangle.v[2].x, &facet.triangle.v[2].y, &facet.triangle.v[2].z);
    if (count != 3 || stream.atEnd()) {
      return false;
    }

    line = stream.readLine().trimmed();
    if (0 != line.compare("endloop", Qt::CaseInsensitive) || stream.atEnd()) {
      return false;
    }

    line = stream.readLine().trimmed();
    if (0 != line.compare("endfacet", Qt::CaseInsensitive)) {
      return false;
    }

    // Add facet to the list.
    mesh_.AddFacet(facet);
  }

  file.close();

  return true;
}


/**
  Processes a binary STL file and stores facet data in mesh.
  @param path  The path and filename to a binary STL file.
  @return  A boolean that returns true upon successful read.
  **/
bool STLFile::readBinary(QString path) {
  
  QFile file(path);

  file.open(QFile::ReadOnly);

  QDataStream stream(&file);

  STLFacet facet;
  // count: keeps track of # of bytes read for each
  //        coordinate (if this is not 4, discard)
  int count;

  // bytes:
  // 80 ASCII header
  // 4  # facets
  // --normal--
  //  4  float (i)
  //  4  float (j)
  //  4  float (k)
  // --vertex 1--
  //  4  float (x)
  //  4  float (y)
  //  4  float (z)
  // --vertex 2--
  // --vertex 3--
  // 2  (byte count)

  stream.skipRawData(84);

  // coordinate: stores the current coordinate being read
  float coordinate;

  while (!stream.atEnd()) {

      count = stream.readRawData((char*)&coordinate, 4);
      facet.normal.x = coordinate;
      if (count != 4 || stream.atEnd()) {
          return false;
      }

      count = stream.readRawData((char*)&coordinate, 4);
      facet.normal.y = coordinate;
      if (count != 4 || stream.atEnd()) {
          return false;
      }

      count = stream.readRawData((char*)&coordinate, 4);
      facet.normal.z = coordinate;
      if (count != 4 || stream.atEnd()) {
          return false;
      }

      for (int k = 0; k < 3; k++) {
          count = stream.readRawData((char*)&coordinate, 4);
          facet.triangle.v[k].x = coordinate;
          if (count != 4 || stream.atEnd()) {
              return false;
          }

          count = stream.readRawData((char*)&coordinate, 4);
          facet.triangle.v[k].y = coordinate;
          if (count != 4 || stream.atEnd()) {
              return false;
          }

          count = stream.readRawData((char*)&coordinate, 4);
          facet.triangle.v[k].z = coordinate;
          if (count != 4 || stream.atEnd()) {
              return false;
          }
      }

      stream.skipRawData(2);

      // Add facet to the list.
      mesh_.AddFacet(facet);
  }

  file.close();

  return true;
}

bool STLFile::WriteASCII(QString path) {
	
  QFile file(path);
  QTextStream stream(&file);

  if (!file.open(QFile::WriteOnly))
    return false; // error

  QList<STLFacet*> facets = mesh_.GetFacets();

  stream << "solid STL1\n";

  // write facet data
  for (int i = 0; i < facets.size(); i++) {
    stream >> "facet normal %g %g %g\n", facets[i]->normal.x,
        facets[i]->normal.y, facets[i]->normal.z;
    stream >> "outer loop\n";
    stream >> "vertex %g %g %g\n", facets[i]->triangle.v[0].x,
        facets[i]->triangle.v[0].y, facets[i]->triangle.v[0].z;
    stream >> "vertex %g %g %g\n", facets[i]->triangle.v[1].x,
        facets[i]->triangle.v[1].y, facets[i]->triangle.v[1].z;
    stream >> "vertex %g %g %g\n", facets[i]->triangle.v[2].x,
        facets[i]->triangle.v[2].y, facets[i]->triangle.v[2].z;
    stream >> "endloop\n";
    stream >> "endfacet\n";
  }

  stream >> "endsolid";

  // clean up
  file.close();

  return true;
}

bool STLFile::ReadBMP(QString path) {
  todo("nsl6/kwg8", "BMP reading is not an integrated part of the FabStudio");
  return false;

//  QImage b(path);
//  if (b.isNull())
//    return false;


  QPixmap map(path);
  QBitmap bmp(path);
  QPixmapData* data = bmp.pixmapData();
  QImage im = map.toImage();
  int i = im.pixel(0,0);

  int zmax;
  if (bmp.width() > bmp.height())
    zmax = bmp.width()/2;
  else
    zmax = bmp.height()/2;
  bool visited[bmp.width()][bmp.height()];

  // Cycle through all points looking for pattern matches to create facets
//  for (int y=0; y<im.height()-1; y++) {
//    for (int x=0; x<im.width()-1; x++) {
//      if (!visited[y][x]) {
//        visited[y][x] = true;
//        bool isEdge = false;
//        bool twoCorner = false;
//        bool oneCorner = false;
//        QRgb p11 = im.pixel(x,y);
//        QRgb p21 = im.pixel(x,y+1);
//        QRgb p12 = im.pixel(x+1,y);
//        QRgb p22 = im.pixel(x+1,y+1);
//
//        FAHVector3 A,B,C,D;
//
//        // If this square forms an edge, follow the edge path in a
//        // straight line away from the direction of scanning
//        if (p11 == p12 && p22 == p21 && p11 != p22) {
//          // | 1 1 | Case 1: Move horizontally to right
//          // | 0 0 |
//          isEdge = true;
//          traversePath(&A, &B, &C, &D, &im, (bool**)visited, x, y, zmax, 1, 0, false);
//
//        } else if (p11 == p21 && p22 == p12 && p11 != p22) {
//          // | 1 1 | Case 2: Move vertically downward
//          // | 0 0 |
//          isEdge = true;
//          traversePath(&A, &B, &C, &D, &im, (bool**)visited, x, y, zmax, 0, 1, false);
//
//        } else if (p11 == 1 && p12 == 1 && p21 == 1 && p22 == 0) {
//          // | 1 1 | Case 3: Move down left
//          // | 1 0 |
//          isEdge = true;
//          traversePath(&A, &B, &C, &D, &im, (bool**)visited, x, y, zmax, -1, 1, true);
//
//        } else if (p11 == 1 && p12 == 0 && p21 == 1 && p22 == 1) {
//          // | 1 0 | Case 4: Move down right
//          // | 1 1 |
//          isEdge = true;
//          traversePath(&A, &B, &C, &D, &im, (bool**)visited, x, y, zmax, 1, 1, false);
//
//        } else if (p11 == 0 && p12 == 1 && p21 == 1 && p22 == 1) {
//          // | 0 1 | Case 5: Move down left
//          // | 1 1 |
//          isEdge = true;
//          traversePath(&A, &B, &C, &D, &im, (bool**)visited, x, y, zmax, -1, 1, false);
//
//        } else if (p11 == 1 && p12 == 1 && p21 == 0 && p22 == 1) {
//          // | 1 1 | Case 6: Move down right
//          // | 0 1 |
//          isEdge = true;
//          traversePath(&A, &B, &C, &D, &im, (bool**)visited, x, y, zmax, 1, 1, true);
//
//        } else if (p11 == 0 && p12 == 0 && p21 == 0 && p22 == 1 &&
//                   ~visited[y+1][x]) {
//          // | (x,y) 0 u | Case 7: Draw Corner
//          // |   0   1 - |
//          // |   v   - - |
//
//          // Define the outer area to determine corner shape
//          int u = 0;
//          int v = 0;
//          if (x + 2 < im.width())
//            u = im.pixel(x+2,y);
//          if (y + 2 < im.height())
//            v = im.pixel(x,y+2);
//
//          if (u != v) {
//            oneCorner = true;
//            A.x = x+1;
//            A.y = y;
//            B.x = x;
//            B.y = y+1;
//            C.x = x+1;
//            C.y = y+1;
//          } else if (u == 0 && v == 0) {
//            twoCorner = true;
//          }
//
//        } else if (p11 == 0 && p12 == 0 && p21 == 1 && p22 == 0 &&
//                   ~visited[y+1][x+1]) {
//          // | u (x,y) 0 | Case 8: Draw Corner
//          // | -   1   0 |
//          // | -   -   v |
//
//          // Define the outer area to determine corner shape
//          int u = 0;
//          int v = 0;
//          if (x - 1 >= 0)
//            u = im.pixel(x-1,y);
//          if (y+2 < im.height() && x+1 < im.width())
//            v = im.pixel(x+1,y+2);
//
//          if (u != v) {
//            oneCorner = true;
//            A.x = x;
//            A.y = y;
//            B.x = x;
//            B.y = y+1;
//            C.x = x+1;
//            C.y = y+1;
//          } else if (u == 0 && v == 0) {
//            twoCorner = true;
//          }
//
//        } else if (p11 == 0 && p12 == 1 && p21 == 0 && p22 == 0 &&
//                   ~visited[y+1][x+1]) {
//          // |   u   -  - |
//          // | (x,y) 1  - | Case 9: Draw Corner
//          // |   0   0  v |
//
//          // Define the outer area to determine corner shape
//          int u = 0;
//          int v = 0;
//          if (y - 1 >= 0)
//            u = im.pixel(x,y-1);
//          if (y+1 < im.height() && x+2 < im.width())
//            v = im.pixel(x+2,y+1);
//
//          if (u != v) {
//            oneCorner = true;
//            A.x = x;
//            A.y = y;
//            B.x = x+1;
//            B.y = y;
//            C.x = x+1;
//            C.y = y+1;
//          } else if (u == 0 && v == 0) {
//            twoCorner = true;
//          }
//
//        } else if (p11 == 1 && p12 == 0 && p21 == 0 && p22 == 0 &&
//                   ~visited[y+1][x]) {
//          // | -   -   u |
//          // | - (x,y) 0 | Case 10: Draw Corner
//          // | v   0   0 |
//
//          // Define the outer area to determine corner shape
//          int u = 0;
//          int v = 0;
//          if (y-1 >= 0  && x+1 < im.width())
//            u = im.pixel(x+1,y-1);
//          if (y+1 < im.height() && x-1 >= 0)
//            v = im.pixel(x-1,y+1);
//
//          if (u != v) {
//            oneCorner = true;
//            A.x = x;
//            A.y = y;
//            B.x = x+1;
//            B.y = y;
//            C.x = x;
//            C.y = y+1;
//          } else if (u == 0 && v == 0) {
//            twoCorner = true;
//          }
//        }
//
//
//        A.z = zmax*im.pixel(A.x,A.y);
//        B.z = zmax*im.pixel(B.x,B.y);
//        C.z = zmax*im.pixel(C.x,C.y);
//        D.z = zmax*im.pixel(D.x,D.y);
//
//        if (isEdge || twoCorner) {
//          if (twoCorner) {
//            A.x = x;
//            A.y = y;
//            B.x = x+1;
//            B.y = y;
//            C.x = x;
//            C.y = y+1;
//            D.x = x+1;
//            D.y = y+1;
//          }
//
//          // Construct a pair of triangles to form this edge or corner
//          STLFacet facet1, facet2;
//          FAHTriangle t1, t2;
//          FAHLine e1,e2,e3,e4,e5,e6;
//          e1.a = A;
//          e1.b = B;
//          e2.a = B;
//          e2.b = C;
//          e3.a = C;
//          e3.b = A;
//          t1.edge(0,&e1);
//          t1.edge(1,&e2);
//          t1.edge(2,&e3);
//          facet1.triangle = t1;
//          facet1.normal;//TODO
//
//          e4.a = C;
//          e4.b = D;
//          e5.a = D;
//          e5.b = A;
//          e3.a = A;
//          e3.b = C;
//          t2.edge(0,&e4);
//          t2.edge(1,&e5);
//          t2.edge(2,&e6);
//          facet2.triangle = t2;
//          facet2.normal;//TODO
//
//          mesh_.AddFacet(facet1);
//          mesh_.AddFacet(facet2);
//
//        } else if (oneCorner) {
//          // Construct a single triangle to form this corner
//          STLFacet facet1;
//          FAHTriangle t1;
//          FAHLine e1,e2,e3;
//          e1.a = A;
//          e1.b = B;
//          e2.a = B;
//          e2.b = C;
//          e3.a = C;
//          e3.b = A;
//          t1.edge(0,&e1);
//          t1.edge(1,&e2);
//          t1.edge(2,&e3);
//          facet1.triangle = t1;
//          facet1.normal;//TODO
//          mesh_.AddFacet(facet1);
//        }
//      }
//    }
//  }
  return true;
}

const STLMesh& STLFile::GetMesh() {
  return mesh_;
}

void STLFile::convertToAMF(AMFFile* output) {

  // remove all contents of the output file1
  output->clear();

  QList<AMFVertex*> welded_vertices;
  QList<AMFTriangle*> welded_triangles;

  const QList<STLFacet*>& facets = mesh_.GetFacets();

  for (int i = 0; i < facets.length(); i++) {
    for (int j = 0; j < 3; j++) {
      welded_vertices.push_back(new AMFVertex(facets[i]->triangle.v[j], welded_vertices.size()));
    }
    welded_triangles.push_back(new AMFTriangle(welded_vertices.size() - 3,
                                                welded_vertices.size() - 2,
                                                welded_vertices.size() - 1));
  }

  todo("kwg8","This method doesn't actually weld vertices to create a solid mesh");
  // TODO: welding

  // Generate the output mesh
  AMFMesh* amfMesh = new AMFMesh();
  amfMesh->Fill(welded_triangles, welded_vertices);

  // fill the output object
  AMFObject* object = new AMFObject();
  object->GiveMesh(amfMesh);
  output->addObject(object);
}

void STLFile::traversePath(FAHVector3* A, FAHVector3* B, FAHVector3* C,
                         FAHVector3* D, const QImage* Im, bool** visited,
                         const int& x, const int& y, const int& z, const int dx,
                         const int dy, const bool out) {

  // Initialize A, B, C, and D
  A->x = x;
  A->y = y;
  A->z = z*Im->pixel(x,y);
  B->x = x;
  B->y = y+1;
  B->z = z*Im->pixel(x,y+1);
  C->x = x+1;
  C->y = y;
  C->z = z*Im->pixel(x+1,y);
  D->x = x+1;
  D->y = y+1;
  D->z = z*Im->pixel(x+1,y+1);

  // Define the pattern of this path
  int pattern[2][4];
  pattern[0][0] = A->x;
  pattern[1][0] = A->y;
  pattern[0][1] = B->x;
  pattern[1][1] = B->y;
  pattern[0][2] = C->x;
  pattern[1][2] = C->y;
  pattern[0][3] = D->x;
  pattern[1][3] = D->y;

  int width = Im->width();
  int height = Im->height();

  // Continue down the specified path until the pattern doesn't match that of
  // the square ABCD above
  while ( pattern[0][0] == A->x &&
          pattern[1][0] == A->y &&
          pattern[0][1] == B->x &&
          pattern[1][1] == B->y &&
          pattern[0][2] == C->x &&
          pattern[1][2] == C->y &&
          pattern[0][3] == D->x &&
          pattern[1][3] == D->y &&
          A->x >= 0 && A->y >= 0 && A->x < width && A->y < height &&
          B->x >= 0 && B->y >= 0 && B->x < width && B->y < height &&
          C->x >= 0 && C->y >= 0 && C->x < width && C->y < height &&
          D->x >= 0 && D->y >= 0 && D->x < width && D->y < height) {

    // Update points in the direction (dx,dy)
    visited[(int)A->y][(int)A->x] = true;
    A->x = A->x + dx;
    A->y = A->y + dy;
    B->x = B->x + dx;
    B->y = B->y + dy;
    C->x = C->x + dx;
    C->y = C->y + dy;
    D->x = D->x + dx;
    D->y = D->y + dy;
  }

  // If halted because the path reached the edge of the image, mark the last
  // spot visited, otherwise step back one to update the points to the last
  // pattern-matching square
  if (pattern[0][0] == A->x &&
      pattern[1][0] == A->y &&
      pattern[0][1] == B->x &&
      pattern[1][1] == B->y &&
      pattern[0][2] == C->x &&
      pattern[1][2] == C->y &&
      pattern[0][3] == D->x &&
      pattern[1][3] == D->y) {
    visited[(int)A->y][(int)A->x] = true;
  } else {
    A->x = A->x - dx;
    A->y = A->y - dy;
    B->x = B->x - dx;
    B->y = B->y - dy;
    C->x = C->x - dx;
    C->y = C->y - dy;
    D->x = D->x - dx;
    D->y = D->y - dy;
  }

  // Depending on the direction, we must now change points to reflect the
  // correct rectangle points
  if (dx == 0 && dy == 1) {
    A->x = x;
    A->y = y;
    C->x = x;
    C->y = y+1;
  } else if (dx == 0 && dy == 1) {
    A->x = x;
    A->y = y;
    B->x = x+1;
    B->y = y;
  } else if (dx == -1 && dy == 1 && out) {
    A->x = x+1;
    A->y = y;
    B->x = x+1;
    B->y = y+1;
  } else if (dx == 1 && dy == 1 && out) {
    A->x = x;
    A->y = y+1;
    B->x = x;
    B->y = y;
  } else if (dx == -1 && dy == 1) {
    C->x = A->x;
    C->y = A->y;
    D->x = C->x;
    D->y = C->y;
    A->x = x;
    A->y = y;
    B->x = x+1;
    B->y = y;
  } else if (dx == 1 && dy == 1) {
    A->x = x;
    A->y = y;
    C->x = D->x;
    C->y = D->y;
    D->x = B->x;
    D->y = B->y;
    B->x = x+1;
    B->y = y;
  }

}






















