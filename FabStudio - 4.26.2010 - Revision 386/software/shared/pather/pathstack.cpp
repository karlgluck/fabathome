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

#include "pathstack.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>



//----[  ~PathStack  ]---------------------------------------------------------
PathStack::~PathStack() {
  for (int i = 0; i < slices_.size(); ++i) {
    delete slices_[i];
  }
}


//----[  getPathSlices  ]------------------------------------------------------
const PathSliceArray& PathStack::getPathSlices() const {
  return slices_;
}



//----[  writeToXMLFile  ]-----------------------------------------------------
bool PathStack::writeToXMLFile(const QString& file_name) const {
  QDomDocument doc;
  QDomElement root = doc.createElement("PathStack");
  doc.appendChild(root);

  // Write each object to the xml
  for (int i = 0; i < slices_.size(); i++) {
    QDomElement slice = doc.createElement("Slice");
    //objects.at(i)->WriteXml(obj);
    root.appendChild(slice);
  }

  QFile file(file_name);
  QTextStream outStream(&file);

  if (!file.open(QFile::WriteOnly)) {
    return false; //error
  }

  outStream << "<!DOCTYPE PATHSTACK>\n";
  outStream << doc.toString(3);

  file.close();

  return true;
}


bool PathStack::writeToVisualizerFile(const QString& file_name) const {
  QFile file(file_name);
  QTextStream out(&file);
  if (!file.open(QFile::WriteOnly)) return false;

  out << "slices = " << slices_.size() << "\n";
  for (int slice = 0; slice < slices_.size(); ++slice) {
    const PathSlice* slice_ptr = slices_.at(slice);
    out << "slice " << slice << "\n";
    {
      QString zlocation;
      zlocation.sprintf("%f", slice_ptr->getZ());
      out << "\tz = " << zlocation << "\n";
    }
    const PathSliceMaterialsArray& materials = slice_ptr->getPathSliceMaterials();
    out << "\tmaterials = " << materials.size() << "\n";
    for (int material = 0; material < materials.size(); ++material) {
      out << "\tmaterial " << material << "\n";
      const PathSliceRegionArray& regions = materials[material]->getRegions();
      out << "\t\tregions = " << regions.size() << "\n";
      for (int region = 0; region < regions.size(); ++region) {
        out << "\t\tregion " << region << "\n";
        const PathArray& paths = regions[region]->getPaths();
        out << "\t\t\tpaths = " << paths.size() << "\n";
        for (int path = 0; path < paths.size(); ++path) {
          out << "\t\t\tpath " << path << "\n";
          const QVector<FAHVector3>& points = paths[path]->points();
          out << "\t\t\t\tpoints = " << points.size() << "\n";
          for (int point = 0; point < points.size(); ++point) {
            const FAHVector3& pt = points[point];
            out << "\t\t\t\tpoint " << point;
            QString position;
            position.sprintf("%f %f %f", pt.x, pt.y, pt.z);
            out << " = " << position << "\n";
          }
        }
      }
    }
  }

  file.close();
  return true;
}




//----[  addSlice  ]-----------------------------------------------------------
void PathStack::addSlice(PathSlice* slice) {
  slices_.push_back(slice);
}

