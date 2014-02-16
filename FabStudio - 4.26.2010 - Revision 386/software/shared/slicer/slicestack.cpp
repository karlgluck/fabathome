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

#include "slicestack.h"
#include <QFile>
#include <QTextStream>

SliceStack::SliceStack() {
}

SliceStack::SliceStack(const SliceStack& other) {
  foreach (const Slice& s, other.slices_) {
    slices_.append(s);
  }
}

SliceStack::SliceStack(SliceArray slices) {
  slices_ += slices;
}

void SliceStack::clear() {
  slices_.clear();
}

const SliceArray& SliceStack::getSlices() const{
  return slices_;
}

void SliceStack::addSlice(Slice slice){
  slices_.append(slice);
}


bool SliceStack::writeToVisualizerFile(
    const QString& file_name,
    const QList<FAHTriangle>* source_triangles) const {
  QFile file(file_name);
  QTextStream out(&file);
  if (!file.open(QFile::WriteOnly)) return false;

  out << slices_.size() << " slices\n";
  for (int slice_index = 0; slice_index < slices_.size(); ++slice_index) {
    out << "slice " << slice_index << "\n";
    const Slice& slice = slices_.at(slice_index);
    out << "\tz = " << slice.getZ() << "\n";
    const SliceMaterialArray& materials = slice.getMaterials();
      out << materials.size() << " materials\n";
    for (int material_index = 0; material_index < materials.size(); ++material_index) {
      const SliceMaterial& material = materials.at(material_index);
      out << "\tmaterial " << material_index << "\n";
      const SliceMaterialRegionArray& regions = material.getRegions();
      out << "\t" << regions.size() << " regions\n";
      for (int region_index = 0; region_index < regions.size(); ++region_index) {
        out << "\t\tregion " << region_index << "\n";
        const SliceMaterialRegion& region = regions.at(region_index);
        const QVector<FAHVector3>& outer_points = region.getOuterBoundary().points;
        out << "\t\t" << outer_points.size() << " points on outer loop\n";
        for (int pt = 0; pt < outer_points.size(); ++pt) {
          const FAHVector3& pt_vec = outer_points.at(pt);
          QString pt_str;
          out << pt_str.sprintf("\t\t\t%f %f %f\n",
                                pt_vec.x,
                                pt_vec.y,
                                pt_vec.z);
        }
        const QVector<FAHLoopInXYPlane>& inner_boundaries = region.getInnerBoundaries();
        out << "\t\t" << inner_boundaries.size() << " inner boundaries\n";
        for (int inner_boundary_index = 0;
             inner_boundary_index < inner_boundaries.size();
             ++inner_boundary_index) {
          const QVector<FAHVector3>& inner_points = inner_boundaries.at(inner_boundary_index).points;
          out << "\t\t\tinner boundary " << inner_boundary_index << " has " << inner_points.size() << " points\n";
          for (int pt = 0; pt < inner_points.size(); ++pt) {
            const FAHVector3& pt_vec = inner_points.at(pt);
            QString pt_str;
            out << pt_str.sprintf("\t\t\t%f %f %f\n",
                                  pt_vec.x,
                                  pt_vec.y,
                                  pt_vec.z);
          }
        }
      }
    }
  }

  if (source_triangles != NULL) {
    // write out all of the source triangle information
    out << "triangles = " << source_triangles->size() << "\n";
    for (int i = 0; i < source_triangles->size(); ++i) {
      const FAHTriangle& triangle = source_triangles->at(i);
      QString str;
      out << str.sprintf("\t%f %f %f : %f %f %f : %f %f %f\n",
                         triangle.v[0].x,
                         triangle.v[0].y,
                         triangle.v[0].z,
                         triangle.v[1].x,
                         triangle.v[1].y,
                         triangle.v[1].z,
                         triangle.v[2].x,
                         triangle.v[2].y,
                         triangle.v[2].z);
    }
  }

  file.close();

  return true;
}
