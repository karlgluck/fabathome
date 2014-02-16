/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2010, Karl Gluck (kwg8@cornell.edu)
               2010, Nathan Lloyd (nsl6@cornell.edu)

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
#include "printableobject.h"
#include "shared/stl/stlfile.h"
#include "shared/amf/amffile.h"
#include "shared/amf/amfobject.h"
#include "shared/amf/amfregion.h"
#include "common/confirm.h"
#include <qgl.h>
#include "openglrenderprimitives.h"

#include "shared/slicer/slicer.h"
#include <math.h>

// Provides function for calculating the center of gravity of a triangle mesh
#include "centerofgravity.h"




float snapParameterValue(float f, float multiple, float range) {
  float m = fmod(f, multiple);
  if (m > multiple/2) m -= multiple;
  float closest_ratchet = f - m;
  return (fabs(closest_ratchet - f) < range) ? closest_ratchet : f;
}




void PrintableObject::Transform::identity() {
  translation.set(0, 0, 0);
  rotation.identity();
  scaling.set(1, 1, 1);
  transform.identity();
}

void PrintableObject::Transform::updateMatrix() {
  FAHMatrix4x4 r,t;
  r.rotationQuaternion(rotation);
  t.translation(translation);
  transform.scaling(scaling).mul(r).mul(t);
}


FAHVector3 PrintableObject::getRotationToPutFaceDown(BoundingCubeFace face) {
  static const float k90Degrees = (float)(3.1415926535 / 2.0);
  switch (face) {
  case kPositiveX: return FAHVector3::make(0.0f,  k90Degrees, 0.0f);
  case kNegativeX: return FAHVector3::make(0.0f, -k90Degrees, 0.0f);
  case kPositiveY: return FAHVector3::make(-k90Degrees, 0.0f, 0.0f);
  case kNegativeY: return FAHVector3::make(+k90Degrees, 0.0f, 0.0f);
  case kPositiveZ: return FAHVector3::make(2*k90Degrees, 0.0f, 0.0f);
  default:
    confirm(!"Unknown response from findBestOrientation");
  case kNegativeZ: return FAHVector3::make(0.0f, 0.0f, 0.0f);
  }
}


QStringList PrintableObject::loadableFileFilterExtensions() {
  return QStringList() << "*.stl" << "*.bstl" << "*.amf";
}

QStringList PrintableObject::loadableFileFilterStrings() {
  return QStringList() << "STL Files (*.stl)" << "Binary STL Files (*.bstl)" << "AMF Files (*.amf)";
}

PrintableObject* PrintableObject::fromFile(const QString& file) {
  todo("kwg8", "instead of basing this purely on extension, we could attempt some sort of file analysis");
  int last_period = file.lastIndexOf(QChar('.'), -1);
  if (last_period < 0) return 0; // can't find the extension
  QString extension = file.right(file.length() - last_period - 1);
  if (0 == QString::compare(extension, "stl", Qt::CaseInsensitive)) {
    return fromSTLFile(file);
  } else if (0 == QString::compare(extension, "bstl", Qt::CaseInsensitive)) {
    return fromSTLFile(file); // should be a binary STL
  } else if (0 == QString::compare(extension, "bmp", Qt::CaseInsensitive)) {
    return fromBlackAndWhiteImage(file);
  } else if (0 == QString::compare(extension, "amf", Qt::CaseInsensitive)) {
    return fromAMFFile(file);
  }

  // can't determine the file type
  return 0;
}

PrintableObject* PrintableObject::fromSTLFile(const QString& file) {
  STLFile stl;
  if (!stl.read(file)) return 0;
  AMFFile* amf = new AMFFile();
  stl.convertToAMF(amf);
  return new PrintableObject(amf);

}

PrintableObject* PrintableObject::fromAMFFile(const QString& file) {
  AMFFile* amf = new AMFFile();
  if (amf->readFromFile(file)) {
    return new PrintableObject(amf);
  } else {
    delete amf;
    return 0;
  }
}

PrintableObject* PrintableObject::fromBlackAndWhiteImage(const QString& file) {
  STLFile stl;
  if (!stl.ReadBMP(file)) return 0;
  AMFFile* amf = new AMFFile();
  stl.convertToAMF(amf);
  return new PrintableObject(amf);
}

void PrintableObject::destroy(PrintableObject* object) {
  delete object;
}

PrintableObject::PrintableObject(AMFFile* amf)
    : amf_(amf), printable_amf_(0) {
  current_.identity();
  manipulation_.identity();
  manipulating_ = false;

  display_list_ = -1;

  updateBoundingSphere();
  moveCenterToOrigin();
}

PrintableObject::~PrintableObject() {
  if (display_list_ != -1 && glIsList(display_list_)) {
    glDeleteLists(display_list_, 1);
    display_list_ = -1;
  }
  delete amf_;
  delete printable_amf_;
}

void PrintableObject::setUniformScale(float multiple) {
  current_.scaling.set(multiple, multiple, multiple);
  updateBoundingSphere();
  changed();
}

void PrintableObject::setUnitScale(float millimeters) {
  const QList<AMFVertex*>* vertices = amf_->GetObjects().at(0)->GetMesh()->GetVertices();
  FAHVector3 min(vertices->at(0)->getPoint());
  FAHVector3 max(min);
  foreach (AMFVertex* v, *vertices) {
    FAHVector3& point = v->getPoint();
    min.min(point);
    max.max(point);
  }
  setUniformScale(FAHFloat(millimeters) / max.copy().sub(min).magnitude());
}

void PrintableObject::translateAbsolute(const FAHVector3& translation) {
  current_.translation.set(translation);
  updateBoundingSphere();
  changed();
}

void PrintableObject::rotateEuler(const FAHVector3& rotation) {
  current_.rotation.setEuler(rotation);
  updateBoundingSphere();
  changed();
}

void PrintableObject::scale(const FAHVector3& scaling) {
  current_.scaling.set(scaling);
  updateBoundingSphere();
  changed();
}


FAHVector3 PrintableObject::translation() const {
  return manipulating_ ? current_.translation.copy().add(manipulation_.translation) : current_.translation;
}

FAHVector3 PrintableObject::rotation() const {
  return manipulating_ ? current_.rotation.copy().mul(manipulation_.rotation).eulerAngles() : current_.rotation.eulerAngles();
}

FAHVector3 PrintableObject::scaling() const {
  return manipulating_ ? current_.scaling.copy().scalePairwise(manipulation_.scaling) : current_.scaling;
}


void PrintableObject::updateBoundingSphere() {
  FAHVector3 min, max;
  findMinMaxOfScaledRotatedMesh(&min, &max);

  // find the furthest from the origin
  FAHFloat rmin = min.magnitude(), rmax = max.magnitude();
  FAHVector3 translation(current_.translation);
  if (manipulating_) {
    translation.add(manipulation_.translation);
  }
  bounding_sphere_.set(translation, rmin > rmax ? rmin : rmax);
}

const FAHSphere& PrintableObject::boundingSphere() const {
  return bounding_sphere_;
}

void PrintableObject::getTransform(FAHMatrix4x4* transform) {

  // Make sure the base matrix is set up
  current_.updateMatrix();

  // Set up the manipulation matrix
  Transform t;
  t.rotation.set(current_.rotation).mul(manipulation_.rotation).normalize();
  t.scaling.set(current_.scaling).scalePairwise(manipulation_.scaling);
  t.translation.set(current_.translation).add(manipulation_.translation);
  t.updateMatrix();
  manipulation_.transform.set(t.transform);

  // Select the matrix to return based on whether manipulation is active
  transform->set(manipulating_ ? &manipulation_.transform : &current_.transform);
}

void PrintableObject::snapToRestOnXYPlane() {
  FAHVector3 min;
  findMinMaxOfScaledRotatedMesh(&min, 0);
  manipulating_ = false;
  current_.translation.z = -min.z;
}




QSet<int> PrintableObject::fillMaterialIDs() const {
  QSet<int> fill_material_ids;
  foreach (const AMFObject* object, amf_->GetObjects()) {
    foreach (const AMFRegion* region, *(object->GetMesh()->GetRegions())) {
      fill_material_ids.insert(region->getFillMaterialId());
    }
  }
  return fill_material_ids;
}



void PrintableObject::setFillMaterial(int fill_material_id, const QString& material_name) {
  fill_material_id_to_material_name_.insert(fill_material_id, material_name);
}

void PrintableObject::clearFillMaterials() {
  fill_material_id_to_material_name_.clear();
  fill_material_id_to_color_.clear();
  changed();
}



QSet<QString> PrintableObject::collectMaterialNames() const {
  return fill_material_id_to_material_name_.values().toSet();
}

QList<AMFRegion*> PrintableObject::printableRegionsAssignedMaterial(const QString& material_name) {
  QMap<int, QString>::const_iterator i = fill_material_id_to_material_name_.constBegin();
  QMap<int, QString>::const_iterator end = fill_material_id_to_material_name_.constEnd();
  QList<AMFRegion*> regions;
  while (i != end) {
    if (QString::compare(i.value(), material_name, Qt::CaseSensitive) == 0) {
      // add all regions with this material ID
      regions += printableRegionsAssignedMaterial(i.key());
    }
    ++i;
  }
  return regions;
}

void PrintableObject::buildPrintableMesh() {
  delete printable_amf_;
  printable_amf_ = new AMFFile();
  current_.updateMatrix();
  foreach (AMFObject* object, amf_->GetObjects()) {
    AMFMesh* new_mesh = new AMFMesh();
    object->GetMesh()->createTransformedCopy(current_.transform, new_mesh);
    AMFObject* new_object = new AMFObject();
    new_object->GiveMesh(new_mesh);
    printable_amf_->addObject(new_object);
  }
}

QMultiMap<QString,AMFRegion*> PrintableObject::getMaterialNameToPrintableMeshMap() {
  QMultiMap<QString,AMFRegion*> map;
  foreach (QString material, fill_material_id_to_material_name_.values()) {
    foreach (AMFRegion* region, printableRegionsAssignedMaterial(material)) {
      map.insertMulti(material, region);
    }
  }
  return map;
}


void PrintableObject::generateSupportingMesh() {
}


void PrintableObject::setBestOrientation() {
  manipulating_ = false;

  // Set the best object rotation
  current_.rotation.setEuler(getRotationToPutFaceDown(findBestOrientation()));

  // Put the object back on the XY plane
  snapToRestOnXYPlane();
  changed();
}


void PrintableObject::setRenderColor(int fill_material_id, const QColor& color) {
  fill_material_id_to_color_.insert(fill_material_id, color);
  changed();
}


void PrintableObject::setRenderColor(const QString& material_name, const QColor& color) {
  QMap<int,QString>::iterator i = fill_material_id_to_material_name_.begin();
  while (i != fill_material_id_to_material_name_.end()) {
    if (QString::compare(i.value(), material_name, Qt::CaseSensitive) == 0) {
      setRenderColor(i.key(), color);
    }
    ++i;
  }
  changed();
}


bool PrintableObject::getRenderColor(int fill_material_id, QColor* color) {
  QMap<int,QColor>::const_iterator i = fill_material_id_to_color_.find(fill_material_id);
  if (i == fill_material_id_to_color_.end()) {
    return false;
  } else {
    *color = i.value();
    return true;
  }
}

bool PrintableObject::hasCompleteMaterialAssignment() const {
  bool all = true;
  foreach (int id, fillMaterialIDs()) {
    all = all && fill_material_id_to_material_name_.contains(id);
  }
  return all;
}

bool PrintableObject::intersectedByRay(const FAHVector3& origin, const FAHVector3& ray) const {

  // Do a coarse check
  //if (!boundingSphere().intersects(origin, ray)) return false;
  return boundingSphere().intersects(origin, ray);

  todo("kwg8", "use an octree acceleration structure to speed this up");

  // Check all of the triangles in the mesh
  foreach (AMFObject* object, amf_->GetObjects()) {
    const AMFMesh* mesh = object->GetMesh();
    const QList<AMFVertex*>* vertices = mesh->GetVertices();
    foreach (const AMFRegion* region, *mesh->GetRegions()) {
      foreach (const AMFTriangle* amf_triangle, region->getTriangles()) {
        const FAHVector3& v0 = vertices->at(amf_triangle->getV1())->getPoint();
        const FAHVector3& v1 = vertices->at(amf_triangle->getV2())->getPoint();
        const FAHVector3& v2 = vertices->at(amf_triangle->getV3())->getPoint();
        FAHTriangle triangle(v0, v1, v2);
        if (triangle.intersectedByRay(origin, ray)) return true;
      }
    }
  }

  return false;
}



void PrintableObject::glRender(bool transparent) {
  FAHMatrix4x4 transform;
  getTransform(&transform);

  // Make sure the bounding sphere is synced
  updateBoundingSphere();


  if (transparent) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  } else {
    float ambient[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    float position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

  }

  // Render the mesh itself
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd(transform.array());

  if (display_list_ != -1 && glIsList(display_list_)) {

    glCallList(display_list_);

  } else {
    static const QColor kDefaultColor(128, 128, 128, 0);
    static const float kAlpha = 0.1f;

    // Rebuild the display list
    display_list_ = glGenLists(1);
    glNewList(display_list_, GL_COMPILE_AND_EXECUTE);


    foreach (AMFObject* object, amf_->GetObjects()) {
      const AMFMesh* mesh = object->GetMesh();
      const QList<AMFVertex*>* vertices = mesh->GetVertices();
      foreach (const AMFRegion* region, *mesh->GetRegions()) {
        QColor fill_material_color;
        const QColor& color = getRenderColor(region->getFillMaterialId(),
                                             &fill_material_color)
                                ? fill_material_color
                                : kDefaultColor;
        float rgba[] = {color.red()   / 255.0f,
                        color.green() / 255.0f,
                        color.blue()  / 255.0f,
                        1.0f};
        glColor4f(rgba[0], rgba[1], rgba[2], kAlpha);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, rgba);
        glBegin(GL_TRIANGLES);

        foreach (const AMFTriangle* amf_triangle, region->getTriangles()) {

          const FAHVector3& v0 = vertices->at(amf_triangle->getV1())->getPoint();
          const FAHVector3& v1 = vertices->at(amf_triangle->getV2())->getPoint();
          const FAHVector3& v2 = vertices->at(amf_triangle->getV3())->getPoint();
          FAHTriangle triangle(v0, v1, v2);

          FAHVector3 normal = triangle.normal();

          glNormal3f(normal.x, normal.y, normal.z);

          glVertex3f(triangle.v[0][0], triangle.v[0][1], triangle.v[0][2]);
          glVertex3f(triangle.v[1][0], triangle.v[1][1], triangle.v[1][2]);
          glVertex3f(triangle.v[2][0], triangle.v[2][1], triangle.v[2][2]);
        }

        glEnd();
      }
    }

    glEndList();
  }

  if (transparent) {
    glDisable(GL_BLEND);
  } else {
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_NORMALIZE);
  }

  glPopMatrix();
}


void PrintableObject::glRenderHandles(PrintableObjectHandleType active_handles,
                                      PrintableObjectHandleAxis mouse_axis,
                                      bool dragging,
                                      bool display_z_translate) {
  glRenderArrowHandles((active_handles == kTranslationHandles ||
                        active_handles == kTranslationHandles_NoZAxis) ? mouse_axis : kNoAxis, dragging, display_z_translate);
  glRenderRotateXYZHandles(active_handles == kRotationHandles ? mouse_axis : kNoAxis, dragging);
  glRenderCubeHandles(active_handles == kScalingHandles ? mouse_axis : kNoAxis, dragging);
}

bool PrintableObject::pickHandle(const FAHVector3& source,
                                 const FAHVector3& pick_ray,
                                 bool pick_z_translate,
                                 PrintableObjectHandleType* handle,
                                 PrintableObjectHandleAxis* axis) {
  PrintableObjectHandleAxis picked_axis;
  const PrintableObjectHandleType translate_handle
      = pick_z_translate ? kTranslationHandles : kTranslationHandles_NoZAxis;
  if (kNoAxis != (picked_axis = pickHandleAxis(translate_handle, source, pick_ray))) {
    *handle = translate_handle;
    *axis = picked_axis;
    return true;
  }
  if (kNoAxis != (picked_axis = pickHandleAxis(kRotationHandles, source, pick_ray))) {
    *handle = kRotationHandles;
    *axis = picked_axis;
    return true;
  }
  if (kNoAxis != (picked_axis = pickHandleAxis(kScalingHandles, source, pick_ray))) {
    *handle = kScalingHandles;
    *axis = picked_axis;
    return true;
  }
  return false;
}

PrintableObjectHandleAxis PrintableObject::pickHandleAxis(PrintableObjectHandleType type,
                                                          const FAHVector3& source,
                                                          const FAHVector3& pick_ray) {
  FAHLine pick(source, source.copy().add(pick_ray));
  FAHVector3 point;

  switch (type) {
  default:
    return kNoAxis;
  case kRotationHandles:
    if (intersectRotationHandlePoint(pick, FAHVector3::make(1,0,0), &point)) return kXOnly;
    if (intersectRotationHandlePoint(pick, FAHVector3::make(0,1,0), &point)) return kYOnly;
    if (intersectRotationHandlePoint(pick, FAHVector3::make(0,0,1), &point)) return kZOnly;
    //if (bounding_sphere_.intersects(source, pick_ray)) return kAllAxes;
    break;
  case kTranslationHandles:
    if (intersectTranslationHandlePoint(source, pick_ray, FAHVector3::make(0,0,+1), &point)) return kZOnly;
    if (intersectTranslationHandlePoint(source, pick_ray, FAHVector3::make(0,0,-1), &point)) return kZOnly;
  case kTranslationHandles_NoZAxis:
    if (intersectTranslationHandlePoint(source, pick_ray, FAHVector3::make(+1,0,0), &point)) return kXOnly;
    if (intersectTranslationHandlePoint(source, pick_ray, FAHVector3::make(-1,0,0), &point)) return kXOnly;
    if (intersectTranslationHandlePoint(source, pick_ray, FAHVector3::make(0,+1,0), &point)) return kYOnly;
    if (intersectTranslationHandlePoint(source, pick_ray, FAHVector3::make(0,-1,0), &point)) return kYOnly;
    break;
  case kScalingHandles:
    if (intersectScalingHandlePoint(source, pick_ray, FAHVector3::make(+1,+1,+1), &point) ||
        intersectScalingHandlePoint(source, pick_ray, FAHVector3::make(+1,+1,-1), &point) ||
        intersectScalingHandlePoint(source, pick_ray, FAHVector3::make(+1,-1,+1), &point) ||
        intersectScalingHandlePoint(source, pick_ray, FAHVector3::make(+1,-1,-1), &point) ||
        intersectScalingHandlePoint(source, pick_ray, FAHVector3::make(-1,+1,+1), &point) ||
        intersectScalingHandlePoint(source, pick_ray, FAHVector3::make(-1,+1,-1), &point) ||
        intersectScalingHandlePoint(source, pick_ray, FAHVector3::make(-1,-1,+1), &point) ||
        intersectScalingHandlePoint(source, pick_ray, FAHVector3::make(-1,-1,-1), &point)) return kAllAxes;
    break;
  }
  return kNoAxis;
}


void PrintableObject::handleManipulate(PrintableObjectHandleType type,
                                       PrintableObjectHandleAxis axis,
                                       const FAHVector3& source,
                                       const FAHVector3& start_ray,
                                       const FAHVector3& end_ray) {

  FAHLine start(source, source.copy().add(start_ray));
  FAHLine end(source, source.copy().add(end_ray));
  manipulating_ = true;
  switch (type) {
  case kRotationHandles:  handleManipulateRotation(axis, start, end); break;
  case kTranslationHandles:
  case kTranslationHandles_NoZAxis:
    handleManipulateTranslation(axis, start, end); break;
  case kScalingHandles: handleManipulateScaling(axis, bounding_sphere_.center.copy().sub(source).normalize(), start_ray, end_ray); break;
  }
  changed();
}

void PrintableObject::handleFinishManipulation(bool keep_changes) {
  if (!manipulating_) return;
  manipulating_ = false;
  if (keep_changes) {

    // Snap the rotation angles
    current_.rotation.mul(manipulation_.rotation).normalize();
    current_.scaling.scalePairwise(manipulation_.scaling);
    current_.translation.add(manipulation_.translation);
    current_.updateMatrix();
  }
  manipulation_.identity();
  changed();
}

void PrintableObject::moveCenterToOrigin() {
  // adjust vertices to put center of the bounding cube at the origin
  const QList<AMFVertex*>* vertices = amf_->GetObjects().at(0)->GetMesh()->GetVertices();
  FAHVector3 min(vertices->at(0)->getPoint());
  FAHVector3 max(min);
  foreach (AMFVertex* v, *vertices) {
    FAHVector3& point = v->getPoint();
    min.min(point);
    max.max(point);
  }
  FAHVector3 offset;
  offset.set(max).add(min).scale(-0.5f);
  foreach (AMFVertex* v, *vertices) {
    FAHVector3& point = v->getPoint();
    point.add(offset);
  }
  changed();
}


bool PrintableObject::intersectRotationHandlePoint(const FAHLine& pick_line,
                                                   const FAHVector3& plane_normal,
                                                   FAHVector3* point) {
  FAHPlane plane(bounding_sphere_.center, plane_normal);
  if (plane.intersectLine(pick_line, point)) {
    // is this point of intersection on the rotation handle?
    float distance = point->distanceTo(bounding_sphere_.center);
    return (distance >= bounding_sphere_.radius && distance <= 1.1f*bounding_sphere_.radius);
  }
  return false;
}


bool PrintableObject::intersectTranslationHandlePoint(const FAHVector3& origin,
                                                      const FAHVector3& ray,
                                                      const FAHVector3& handle_offset,
                                                      FAHVector3* point) {
  FAHSphere handle(handle_offset.copy().scale(bounding_sphere_.radius*1.5).add(bounding_sphere_.center), 0.2*bounding_sphere_.radius);
  return handle.intersectNearLocation(origin, ray, point);
}



bool PrintableObject::intersectScalingHandlePoint(const FAHVector3& origin,
                                                  const FAHVector3& ray,
                                                  const FAHVector3& handle_offset,
                                                  FAHVector3* point) {
  FAHSphere handle(handle_offset.copy().scale(bounding_sphere_.radius).add(bounding_sphere_.center), 0.2*bounding_sphere_.radius);
  return handle.intersectNearLocation(origin, ray, point);
}

void PrintableObject::glRenderRotateXYZHandles(PrintableObjectHandleAxis mouse_axis, bool dragging) {
  bool highlight[3] = {
    mouse_axis == kXOnly || mouse_axis == kAllAxes,
    mouse_axis == kYOnly || mouse_axis == kAllAxes,
    mouse_axis == kZOnly || mouse_axis == kAllAxes,
  };

  glBegin(GL_TRIANGLES);
  static const float PI = 3.1415926535;
  static const float inc = 0.2; // number of radians per quad; increase to reduce triangle count
  static const float full_intensity = dragging ? 1.0f : 0.5f;
  for (int axis = 0; axis < 3; ++axis) {
    FAHVector3 pt;
    float rgba[] = {0, 0, 0, 0};

    switch (axis) {
    case 0: // X axis
      if (dragging && !highlight[0]) continue;
      rgba[0] = 1.0f;
      rgba[3] = highlight[0] ? full_intensity : 0.1f;
      pt.set(0.0, bounding_sphere_.radius, 0.0);
      break;
    case 1: // Y axis
      if (dragging && !highlight[1]) continue;
      rgba[1] = 1.0f;
      rgba[3] = highlight[1] ? full_intensity : 0.1f;
      pt.set(bounding_sphere_.radius, 0.0f, 0.0);
      break;
    case 2: // Z axis
      if (dragging && !highlight[2]) continue;
      rgba[2] = 1.0f;
      rgba[3] = highlight[2] ? full_intensity : 0.1f;
      pt.set(bounding_sphere_.radius, 0.0, 0.0);
      break;
    }

    glColor4f(rgba[0],
              rgba[1],
              rgba[2],
              rgba[3]);

    float theta = 2*PI+inc;
    while (theta >= 0.0f) {
      FAHVector3 a(pt), c(pt);
      switch (axis) {
      case 0: // X axis
        a.rotateX(theta);
        c.rotateX(theta - inc);
        break;
      case 1: // Y axis
        a.rotateY(theta);
        c.rotateY(theta - inc);
        break;
      case 2: // Z axis
        a.rotateZ(theta);
        c.rotateZ(theta - inc);
        break;
      }
      FAHVector3 b(a), d(c);
      b.scale(1.1f);
      d.scale(1.1f);
      a.add(bounding_sphere_.center);
      b.add(bounding_sphere_.center);
      c.add(bounding_sphere_.center);
      d.add(bounding_sphere_.center);

      glVertex3f(a.x, a.y, a.z);
      glVertex3f(b.x, b.y, b.z);
      glVertex3f(d.x, d.y, d.z);
      glVertex3f(a.x, a.y, a.z);
      glVertex3f(d.x, d.y, d.z);
      glVertex3f(c.x, c.y, c.z);

      theta -= inc;
    }
  }
  glEnd();
}

void PrintableObject::glRenderCubeHandles(PrintableObjectHandleAxis mouse_axis,
                                          bool dragging) {

  bool highlight = mouse_axis == kAllAxes;
  if (!highlight && dragging) return;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glPushAttrib(GL_COLOR);
  glColor4f(1.0f, 0.5f, 0.0f, highlight ? 1.0f : 0.5f);

  FAHMatrix4x4 s, t;

  const float uniform = bounding_sphere_.radius * 0.2;

  for (int z = -1; z <= 1; z += 2) {
    for (int y = -1; y <= 1; y += 2) {
      for (int x = -1; x <= 1; x += 2) {

        s.scaling(x*uniform, y*uniform, z*uniform);
        t.translation(bounding_sphere_.center.copy().add(FAHVector3::make(x,y,z).scale(bounding_sphere_.radius)));
        glLoadMatrixd(s.mul(t).array());

        OpenGLPrimitives::renderScalingCorner();
      }
    }
  }

  glPopAttrib();
  glPopMatrix();
}


void PrintableObject::glRenderArrowHandles(PrintableObjectHandleAxis mouse_axis,
                                           bool dragging,
                                           bool display_z) {
  bool highlight[3] = {
    mouse_axis == kXOnly || mouse_axis == kAllAxes,
    mouse_axis == kYOnly || mouse_axis == kAllAxes,
    mouse_axis == kZOnly || mouse_axis == kAllAxes,
  };

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glPushAttrib(GL_COLOR);

  FAHMatrix4x4 s,r,t;

  const float sr = bounding_sphere_.radius;
  s.scaling(sr,sr,sr);
  t.translation(bounding_sphere_.center);

  const float offset = 1.0;

  if (!dragging || highlight[0]) {

    // +X
    glColor4f(1.0f, 0.0f, 0.0f, highlight[0] ? 1.0f : 0.1f);
    r.rotationZ(Math::toRadians(-90));
    glLoadMatrixd(s.copy().mul(r).mul(t).array());
    OpenGLPrimitives::renderArrow(offset);

    // -X
    r.rotationZ(Math::toRadians(90));
    glLoadMatrixd(s.copy().mul(r).mul(t).array());
    OpenGLPrimitives::renderArrow(offset);
  }

  if (!dragging || highlight[1]) {
    // +Y
    glColor4f(0.0f, 1.0f, 0.0f, highlight[1] ? 1.0f : 0.1f);
    glLoadMatrixd(s.copy().mul(t).array());
    OpenGLPrimitives::renderArrow(offset);

    // -Y
    r.rotationX(Math::toRadians(180));
    glLoadMatrixd(s.copy().mul(r).mul(t).array());
    OpenGLPrimitives::renderArrow(offset);
  }

  if (display_z && (!dragging || highlight[2])) {
    // +Z
    glColor4f(0.0f, 0.0f, 1.0f, highlight[2] ? 1.0f : 0.1f);
    r.rotationX(Math::toRadians(90));
    glLoadMatrixd(s.copy().mul(r).mul(t).array());
    OpenGLPrimitives::renderArrow(offset);

    // -Z
    r.rotationX(Math::toRadians(-90));
    glLoadMatrixd(s.copy().mul(r).mul(t).array());
    OpenGLPrimitives::renderArrow(offset);
  }

  glPopAttrib();
  glPopMatrix();
}

void PrintableObject::handleManipulateRotation(PrintableObjectHandleAxis axis,
                                               const FAHLine& start,
                                               const FAHLine& end) {

  // Snap the x and y rotations to multiples of 90 degrees
  static const Math::Float RATCHET_RADS = Math::kPi/2;
  static const Math::Float kMultiplier = 1.0f;

  switch (axis) {
  case kXOnly: {
    FAHPlane yz(bounding_sphere_.center, FAHVector3::make(1,0,0));
    FAHVector3 start_point(yz.intersectLine(start).sub(bounding_sphere_.center));
    FAHVector3 end_point(yz.intersectLine(end).sub(bounding_sphere_.center));

    // Find the degree of rotation
    FAHFloat start_angle = atan2(start_point.y, start_point.z);
    FAHFloat end_angle = atan2(end_point.y, end_point.z);

    // Manipulate by the delta
    manipulation_.rotation.set(FAHVector3::make(1,0,0), snapParameterValue(kMultiplier*(end_angle - start_angle), RATCHET_RADS, RATCHET_RADS));

  } break;

  case kYOnly: {
    // same code as for X-only with appropriate replacements
    FAHPlane xz(bounding_sphere_.center, FAHVector3::make(0,1,0));
    FAHVector3 start_point(xz.intersectLine(start).sub(bounding_sphere_.center));
    FAHVector3 end_point(xz.intersectLine(end).sub(bounding_sphere_.center));

    FAHFloat start_angle = atan2(start_point.z, start_point.x);
    FAHFloat end_angle = atan2(end_point.z, end_point.x);

    manipulation_.rotation.set(FAHVector3::make(0,1,0), snapParameterValue(kMultiplier*(end_angle - start_angle), RATCHET_RADS, RATCHET_RADS));

  } break;

  case kZOnly: {
    // same code as for X-only with appropriate replacements
    FAHPlane xy(bounding_sphere_.center, FAHVector3::make(0,0,1));
    FAHVector3 start_point(xy.intersectLine(start).sub(bounding_sphere_.center));
    FAHVector3 end_point(xy.intersectLine(end).sub(bounding_sphere_.center));

    FAHFloat start_angle = atan2(start_point.y, start_point.x);
    FAHFloat end_angle = atan2(end_point.y, end_point.x);

    manipulation_.rotation.set(FAHVector3::make(0,0,1), snapParameterValue(-kMultiplier*(end_angle - start_angle), RATCHET_RADS, RATCHET_RADS));
    //manipulation_.rotation.set(FAHVector3::make(0,0,1), -(end_angle - start_angle));

  } break;
  }
}



void PrintableObject::handleManipulateTranslation(PrintableObjectHandleAxis axis,
                                                  const FAHLine& start,
                                                  const FAHLine& end) {

  // Manipulation works as one would expect: when an axis is active, dragging along the
  // plane containing that axis causes the object to move only along that axis.
  // However, there is one exception.  Only having the horizontal-plane (x, y) handles
  // move in exactly their axes makes moving things around on the tray difficult.  So,
  // if the location of the mouse gets far from the straight axis, we "break" into
  // biaxial translation mode that allows the user to drag the object around on the xy
  // plane even though they are holding only either the x or y axes.

  const FAHFloat kBreakoutThreshold = 1;

  switch (axis) {
  case kXOnly:
  case kYOnly: {
    FAHPlane xy(bounding_sphere_.center, FAHVector3::make(0,0,1));
    FAHVector3 start_point(xy.intersectLine(start));
    FAHVector3 end_point(xy.intersectLine(end));
    manipulation_.translation.set(end_point).sub(start_point);
    manipulation_.translation.z = 0; // don't modify the Z-coordinate

    // Don't allow the other coordinate to be modified, unless the mouse has broken out
    if (axis == kXOnly) {
      FAHLine x(start_point, start_point.copy().add(FAHVector3::make(1,0,0)));
      if (x.lineDistanceTo2DXY(end_point) < kBreakoutThreshold) {
        manipulation_.translation.y = 0;
      }
    } else { // axis == kYOnly
      FAHLine y(start_point, start_point.copy().add(FAHVector3::make(0,1,0)));
      if (y.lineDistanceTo2DXY(end_point) < kBreakoutThreshold) {
        manipulation_.translation.x = 0;
      }
    }
  } break;

  case kZOnly: {
    // use a plane xz or yz depending on which is more perpendicular to the pick rays
    FAHPlane plane(bounding_sphere_.center,
                   fabs(start.ray().x)>0.5
                     ? FAHVector3::make(1,0,0)
                     : FAHVector3::make(0,1,0));
    FAHVector3 start_point(plane.intersectLine(start));
    FAHVector3 end_point(plane.intersectLine(end));
    manipulation_.translation.z = end_point.z - start_point.z;
  } break;
  }
}




void PrintableObject::handleManipulateScaling(PrintableObjectHandleAxis axis,
                                              const FAHVector3& sphere_ray,
                                              const FAHVector3& start_ray,
                                              const FAHVector3& end_ray) {
  float start_angle = sphere_ray.angleTo(start_ray);
  float end_angle = sphere_ray.angleTo(end_ray);
  float ratio = end_angle/start_angle;
  manipulation_.scaling.set(ratio, ratio, ratio);
}


void PrintableObject::findMinMaxOfScaledRotatedMesh(FAHVector3* min, FAHVector3* max) {

  // Let 'sr' be the transformation matrix with no translation component
  FAHMatrix4x4 sr;
  if (manipulating_) {
    FAHMatrix4x4 m;
    sr.scaling(current_.scaling.copy().scalePairwise(manipulation_.scaling));
    m.rotationQuaternion(current_.rotation.copy().mul(manipulation_.rotation).normalize());
  } else {
    FAHMatrix4x4 m;
    sr.scaling(current_.scaling);
    m.rotationQuaternion(current_.rotation);
    sr.mul(m);
  }

  const QList<AMFVertex*>* vertices = amf_->GetObjects().at(0)->GetMesh()->GetVertices();
  FAHVector3 local_min(sr.transform(vertices->at(0)->getPoint()));
  FAHVector3 local_max(local_min);

  foreach (AMFVertex* v, *vertices) {
    FAHVector3& point = v->getPoint();
    local_min.min(sr.transform(point));
    local_max.max(sr.transform(point));
  }

  if (min != 0) min->set(local_min);
  if (max != 0) max->set(local_max);
}


QList<AMFRegion*> PrintableObject::printableRegionsAssignedMaterial(int fill_material_id) {
  QList<AMFRegion*> regions;
  confirm (printable_amf_) then {
    foreach (const AMFObject* object, printable_amf_->GetObjects()) {
      foreach (AMFRegion* region, *(object->GetMesh()->GetRegions())) {
        if (region->getFillMaterialId() == fill_material_id) {
          regions.append(region);
        }
      }
    }
  }
  return regions;
}


PrintableObject::BoundingCubeFace PrintableObject::findBestOrientation() {
  BoundingCubeFace lowest_face;
  float lowest_height;
  for (int i = 0; i < 6; ++i) {
    BoundingCubeFace face_to_check = (BoundingCubeFace)i;
    FAHMatrix4x4 transform;
    transform.rotationEuler(getRotationToPutFaceDown(face_to_check));

    // Obtain a list of every single triangle in this object
    QList<FAHTriangle> triangles;
    foreach (const AMFObject* object, amf_->GetObjects()) {
      foreach (AMFRegion* region, *(object->GetMesh()->GetRegions())) {
        QList<FAHTriangle> region_triangles;
        region->getTriangles(&region_triangles);
        triangles += region_triangles;
      }
    }

    // Transform the set of triangles
    for (int t = 0; t < triangles.size(); ++t) {
      FAHTriangle& triangle = triangles[t];
      transform.transformInPlace(&triangle.v[0]);
      transform.transformInPlace(&triangle.v[1]);
      transform.transformInPlace(&triangle.v[2]);
    }

    // Find the lowest point of the mesh
    float min_z = triangles[0].v[0].z;
    for (int t = 0; t < triangles.size(); ++t) {
      FAHTriangle& triangle = triangles[t];
      min_z = min_z > triangle.v[0].z ? triangle.v[0].z : min_z;
      min_z = min_z > triangle.v[1].z ? triangle.v[1].z : min_z;
      min_z = min_z > triangle.v[2].z ? triangle.v[2].z : min_z;
    }

    // Use a Monte Carlo method to discover the center of gravity
    FAHVector3 center_of_gravity
        = calculateCenterOfGravity(triangles);

    // Determine if this center of gravity is better than others
    // (We have to adjust the center of gravity by the minimum
    //  z coordinate in order to compensate for rotation)
    float height = center_of_gravity.z - min_z;
    if (i == 0 || lowest_height > height) {
      lowest_height = height;
      lowest_face = face_to_check;
    }
  }
  return lowest_face;
}

void PrintableObject::changed() {
  if (display_list_ != -1 && glIsList(display_list_)) glDeleteLists(display_list_, 1);
  display_list_ = -1;
}
