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
#include "fabathomemodel2fabwritertsi.h"
#include "shared/pather/pathslice.h"
#include "shared/pather/pathslicematerial.h"
#include "shared/pather/pathsliceregion.h"
#include "shared/pather/pathstack.h"

#include <common/todo.h>
#include <QScriptClassPropertyIterator>

#include "shared/toolscript/amfregiontsi.h"
#include "shared/amf/amfregion.h"


FabAtHomeModel2FabWriterTSI::FabAtHomeModel2FabWriterTSI(QScriptEngine& engine,
                                                         QDomDocument& fab_document)
  : ToolScriptInterface(engine),
    fab_document_(fab_document),
    print_acceleration_(100),     // default acceleration, valid for many fab files
    print_bottom_up_(true)        // usually building things
{
}

FabAtHomeModel2FabWriterTSI::~FabAtHomeModel2FabWriterTSI() {
  foreach (MaterialProperties* material, materials_list_) {
    delete material;
  }
}

void FabAtHomeModel2FabWriterTSI::addMeshes(QString material_name,
                                            QScriptValue material_calibration,
                                            QScriptValue meshes_array) {
  MaterialProperties* material_properties = new MaterialProperties();

  { // iterate over material calibration properties
    QScriptValueIterator iterator(material_calibration);
    while (iterator.hasNext()) {
      iterator.next();
      material_properties->insert(iterator.name(),
                                  iterator.value().toString());
    }

    material_properties->insert("name", material_name);

    materials_list_.append(material_properties);
  }

  { // save the meshes' stacks into the main list of stacks, paired
    // with this material calibration
    QScriptValueIterator iterator(meshes_array);

    while (iterator.hasNext()) {
      iterator.next();
      QScriptValue value = iterator.value();
      if (!value.isQObject()) continue;
      QObject* qo = value.toQObject();
      AMFRegionTSI* region_tsi =
          (AMFRegionTSI*)(0 == QString::compare("AMFRegionTSI",
                                                qo->metaObject()->className()) ? qo : NULL);
      PrintableRegionPair p(material_properties,
                            region_tsi);
      regions_.append(p);
    }
  }
}


void FabAtHomeModel2FabWriterTSI::sortBottomUp() {
  print_bottom_up_ = true;
}


void FabAtHomeModel2FabWriterTSI::sortTopDown() {
  print_bottom_up_ = false;
}

void FabAtHomeModel2FabWriterTSI::setPrintAcceleration(int acceleration) {
  print_acceleration_ = acceleration;
}


struct PrintablePaths {
  float z;
  QString tool_name;
  PathSliceRegion* region;
};

bool operator < (const PrintablePaths& lhs,
                 const PrintablePaths& rhs) {
  return lhs.z < rhs.z;
}


void FabAtHomeModel2FabWriterTSI::print() {

  QDomElement root = fab_document_.createElement("fabAtHomePrinter");
  fab_document_.appendChild(root);

  { // write out the print-acceleration node
    QString print_acceleration_text;
    QTextStream (&print_acceleration_text) << print_acceleration_;
    QDomElement element = fab_document_.createElement("printAcceleration");
    element.appendChild(fab_document_.createTextNode(print_acceleration_text));
    root.appendChild(element);
  }

  for (int i = 0; i < materials_list_.size(); ++i) {
    MaterialProperties* material = materials_list_.at(i);

    { // add the material calibration settings to the document
      QDomElement material_calibration = fab_document_.createElement("materialCalibration");
      for (MaterialProperties::iterator i = material->begin(); i != material->end(); ++i) {
        QDomElement calibration_element
            = fab_document_.createElement(i.key());
        calibration_element.appendChild(
            fab_document_.createTextNode(i.value()));
        material_calibration.appendChild(calibration_element);
      }

      root.appendChild(material_calibration);
    }
  }

  // The set of all paths that need to be printed.  This will be sorted by
  // slice height after construction so that all of the paths are printed
  // in the correct order.
  QList<PrintablePaths> collected_printable_paths_bottom_up;

  foreach (const PrintableRegionPair& printable_region_pair, regions_) {
    MaterialProperties* material_properties = printable_region_pair.first;
    MaterialProperties::iterator i = material_properties->find("name");
    confirm (i != material_properties->end()) else continue;
    QString tool_name = i.value();

    AMFRegionTSI* region = printable_region_pair.second;
    PathStack* paths = region->paths();
    foreach (PathSlice* slice, paths->getPathSlices()) {
      foreach (PathSliceMaterial* material, slice->getPathSliceMaterials()) {
        foreach (PathSliceRegion* region, material->getRegions()) {
          PrintablePaths printable_paths;
          printable_paths.z = slice->getZ();
          printable_paths.tool_name = tool_name;
          printable_paths.region = region;
          collected_printable_paths_bottom_up.append(printable_paths);
        }
      }
    }
  }

  qSort(collected_printable_paths_bottom_up);

  for (int i = 0; i < collected_printable_paths_bottom_up.size(); ++i) {

    // Get the paths based on the direction of the print
    const PrintablePaths& p
        = print_bottom_up_ ? collected_printable_paths_bottom_up.at(i)
          : collected_printable_paths_bottom_up.at(collected_printable_paths_bottom_up.size()-1-i);

    foreach (Path* path, p.region->getPaths()) {

      QDomElement pathDomElement = fab_document_.createElement("path");
      { // add the name of the material that's being used for this path
        QDomElement material_calibration_name = fab_document_.createElement("materialCalibrationName");
        material_calibration_name.appendChild(fab_document_.createTextNode(p.tool_name));
        pathDomElement.appendChild(material_calibration_name);
      }

      const QVector<FAHVector3>& points = path->points();
      for (int point = 0; point < points.size(); ++point) {
        const FAHVector3& xyz = points.at(point);
        QDomElement pointDomElement = fab_document_.createElement("point");
        QString v;

        // Generate the x/y/z location properties
        QDomElement x = fab_document_.createElement("x");
        x.appendChild(fab_document_.createTextNode(v.sprintf("%f",xyz.x)));
        pointDomElement.appendChild(x);
        QDomElement y = fab_document_.createElement("y");
        y.appendChild(fab_document_.createTextNode(v.sprintf("%f",xyz.y)));
        pointDomElement.appendChild(y);
        QDomElement z = fab_document_.createElement("z");
        pointDomElement.appendChild(z);
        z.appendChild(fab_document_.createTextNode(v.sprintf("%f",xyz.z)));

        // Add this point to the current path
        pathDomElement.appendChild(pointDomElement);
      }

      root.appendChild(pathDomElement);
    }
  }
}
