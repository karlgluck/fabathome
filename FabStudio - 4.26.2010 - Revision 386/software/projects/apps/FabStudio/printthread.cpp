/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2010, Karl Gluck (kwg8@cornell.edu)

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
#include "printthread.h"
#include "mainwindow.h"
#include "shared/toolscript/toolscript.h"
#include "shared/slicer/slicerprogresscallback.h"
#include "shared/pather/patherprogresscallback.h"
#include "shared/toolscript/progresstsi.h"
#include "printthreadpatherprogresscallbacksignals.h"
#include "printthreadslicerprogresscallbacksignals.h"

#include "generatesupportmaterial.h"
#include "shared/amf/amfmesh.h"

PrintThread::PrintThread(MainWindow* main_window,
                         const ToolScript* tool_script,
                         PatherImplementation pather_implementation,
                         bool generate_support_material) {
  main_window_ = main_window;
  tool_script_ = tool_script;
  pather_implementation_ = pather_implementation;
  generate_support_material_ = generate_support_material;
}

PrintThread::~PrintThread() {
}

#include "shared/amf/amfvertex.h"
#include "shared/amf/amfregion.h"

void PrintThread::run() {
  PrintThreadSlicerProgressCallbackSignals slicer_callback;
  PrintThreadPatherProgressCallbackSignals pather_callback;
  ProgressTSI progress_tsi;

  connect(&progress_tsi, SIGNAL(setRange(int,int)), this, SLOT(emitSetMasterRange(int,int)), Qt::QueuedConnection);
  connect(&progress_tsi, SIGNAL(setProgress(int)), this, SLOT(emitSetMasterProgress(int)), Qt::QueuedConnection);
  connect(&progress_tsi, SIGNAL(outputLogEntry(QString)), this, SLOT(emitLogEntry(QString)), Qt::QueuedConnection);

  connect(&slicer_callback, SIGNAL(signalSetTaskRange(int,int)), this, SLOT(emitSetTaskRange(int,int)), Qt::QueuedConnection);
  connect(&slicer_callback, SIGNAL(signalSetTaskProgress(int)), this, SLOT(emitSetTaskProgress(int)), Qt::QueuedConnection);
  connect(&slicer_callback, SIGNAL(signalEncounteredIssue(QString)), this, SLOT(emitLogEntry(QString)), Qt::QueuedConnection);
  connect(&slicer_callback, SIGNAL(signalSetTaskTitle(QString)), this, SLOT(emitSetTaskTitle(QString)), Qt::QueuedConnection);
  connect(&slicer_callback, SIGNAL(signalSetProcessingPlane(float)), this, SLOT(emitSetProcessingPlane(float)), Qt::QueuedConnection);

  connect(&pather_callback, SIGNAL(signalSetTaskRange(int,int)), this, SLOT(emitSetTaskRange(int,int)), Qt::QueuedConnection);
  connect(&pather_callback, SIGNAL(signalSetTaskProgress(int)), this, SLOT(emitSetTaskProgress(int)), Qt::QueuedConnection);
  connect(&pather_callback, SIGNAL(signalEncounteredIssue(QString)), this, SLOT(emitLogEntry(QString)), Qt::QueuedConnection);
  connect(&pather_callback, SIGNAL(signalSetTaskTitle(QString)), this, SLOT(emitSetTaskTitle(QString)), Qt::QueuedConnection);
  connect(&pather_callback, SIGNAL(signalAddRenderablePath(int,Path*)), this, SLOT(emitAddRenderablePath(int,Path*)), Qt::QueuedConnection);

  // Get a set of <tool name, geometry> pairs
  QMultiMap<QString,AMFRegion*> meshes = main_window_->getMaterialNameToPrintableMeshMap();
  if (meshes.isEmpty()) return; // Make sure geometry exists!
  tool_script_->remapMaterialKeysToToolKeys(&meshes);



  // Automatically generate support material
  AMFMesh support_material_mesh;
  if (generate_support_material_) {
    QList<AMFRegion*> printing_regions = meshes.values();
    QString tool_name;
    if (tool_script_->getSupportMaterialTool(&tool_name) && // inexpensive; do it before requires...() check
        requiresSupportMaterial(printing_regions)) {

      // Create the supporting mesh for this object, and add its geometry
      // to the printing map
      generateSupportingMesh(printing_regions, &support_material_mesh);
      foreach (AMFRegion* region, *support_material_mesh.GetRegions()) {
        meshes.insertMulti(tool_name, region);
      }
    }
  }

  // create a new set of meshes
  QList<AMFMesh*> allocated_meshes;
  { // Regenerate all meshes
    QMap<QString,int> tool_materials;
    foreach (const QString& tool, meshes.keys()) {
      if (!tool_materials.contains(tool)) {
        int id = tool_materials.size();
        internal_material_id_to_color_.insert(id, QColor((Qt::GlobalColor)(int(Qt::red)+id)));
        tool_materials.insert(tool, id);
      }
    }

    // Get the set of meshes
    QSet<AMFMesh*> amf_meshes;
    QMultiMap<AMFMesh*,AMFRegion*> regions;
    QMap<AMFRegion*,QString> region_tools;
    for (QMultiMap<QString,AMFRegion*>::const_iterator i = meshes.begin(); i != meshes.end(); ++i) {
      AMFRegion* region = i.value();
      region_tools.insert(region, i.key());
      amf_meshes.insert(region->getParent());
      regions.insertMulti(region->getParent(), region);
    }
    meshes.clear();

    foreach (AMFMesh* mesh, amf_meshes) {

      QList<AMFVertex*> vertices;
      foreach (AMFVertex* vertex, *mesh->GetVertices()) {
        vertices.append(new AMFVertex(vertex->getPoint(), vertex->getVertexID()));
      }

      AMFMesh* new_mesh = new AMFMesh();
      foreach (AMFRegion* region, regions.values(mesh)) {
        QString string = region_tools.find(region).value();
        AMFRegion* new_region = new AMFRegion(new_mesh);
        QList<AMFTriangle*> triangles;
        foreach (AMFTriangle* triangle, region->getTriangles()) {
          triangles.append(new AMFTriangle(triangle));
        }
        new_region->setFillMaterialID(tool_materials.find(string).value());
        new_region->FillTriangles(triangles);
        meshes.insertMulti(string, new_region);
      }

      new_mesh->setVertices(vertices);
      allocated_meshes.append(new_mesh);
    }
  }

  QString fab_file;
  if (tool_script_->run(meshes,
                        &slicer_callback,
                        pather_implementation_,
                        &pather_callback,
                        &progress_tsi,
                        &fab_file)) {
    emit setTaskTitle("Finished!");
    emit finishedFabFile(fab_file);
  } else {
    emit setTaskTitle("Printing error; see output (advanced mode) for details");
  }

  for (int i = 0; i < allocated_meshes.size(); ++i) {
    delete allocated_meshes.at(i);
  }
}

void PrintThread::emitSetMasterRange(int a, int b) {
  emit setMasterRange(a,b);
}

void PrintThread::emitSetMasterProgress(int a) {
  emit setMasterProgress(a);
}

void PrintThread::emitLogEntry(const QString& text) {
  emit logEntry(text);
}

void PrintThread::emitSetTaskRange(int a, int b) {
  emit setTaskRange(a, b);
}

void PrintThread::emitSetTaskProgress(int a) {
  emit setTaskProgress(a);
}

void PrintThread::emitSetTaskTitle(const QString& text) {
  emit setTaskTitle(text);
}

void PrintThread::emitAddRenderablePath(int material_id, Path* path) {
  emit addRenderablePath(internal_material_id_to_color_.find(material_id).value(), path);
}

void PrintThread::emitSetProcessingPlane(float z) {
  emit setProcessingPlane(z);
}
