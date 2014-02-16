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
#ifndef PRINTABLEOBJECT_H
#define PRINTABLEOBJECT_H

#include <QString>
#include <QColor>
#include <QMultiMap>
#include "shared/fabathome-constants.h"
#include "printableobjecthandle.h"
#include "shared/math/quaternion.h"

class AMFFile;
class AMFRegion;
class Slicer;
class SlicerProgressCallback;
class SliceStack;

/**
 * A PrintableObject is a self-contained unit that holds all of the geometry,
 * orientation, material, slicing and pathing information necessary to print
 * this object.
 * It controls its own manipulation with the mouse via the exposed handles
 * interface, and is in charge of its own rendering.
 */
class PrintableObject {

  struct Transform {
    FAHVector3 translation, scaling;
    Math::Quaternion rotation;
    FAHMatrix4x4 transform;
    void identity();
    void updateMatrix();
  };

  enum BoundingCubeFace {
    kPositiveX,
    kNegativeX,
    kPositiveY,
    kNegativeY,
    kPositiveZ,
    kNegativeZ
  };

  /**
   * Returns the x/y/z rotation vector needed to direct the given face to
   * the negative Z direction.
   */
  static FAHVector3 getRotationToPutFaceDown(BoundingCubeFace face);

public:

  /**
   * Returns a list like "*.xyz" for each file type 'xyz' that can be loaded by
   * calling 'fromFile'.  This is used by the automatic scan-for-objects thread
   * and by any other object file filter.
   */
  static QStringList loadableFileFilterExtensions();

  /**
   * Returns a list like "STL File (*.stl)" for each object type that can be
   * loaded
   */
  static QStringList loadableFileFilterStrings();

  /**
   * Loads some type of object file into memory, and returns a new PrintableObject
   * if it could be loaded.  Use PrintableObject::destroy to deallocate the memory
   * for this object once it is no longer needed.
   * @return null if loading fails, otherwise a PrintableObject containing 'file'
   */
  static PrintableObject* fromFile(const QString& file);

  /**
   * Loads a printable object representation of either a binary or Ascii STL file.
   * This method will call the appropriate sub-function automatically.
   */
  static PrintableObject* fromSTLFile(const QString& file);

  /**
   * Loads a printable object representation of an AMF file
   * @return null if loading fails, otherwise a PrintableObject containing 'file'
   */
  static PrintableObject* fromAMFFile(const QString& file);

  /**
   * Scans a black-and-white image and uses the data to create a heightmap-style
   * PrintableObject.
   * @author Nathan
   */
  static PrintableObject* fromBlackAndWhiteImage(const QString& file);

  /**
   * Frees memory for the given object
   */
  static void destroy(PrintableObject* object);

public:
  virtual ~PrintableObject();

  /**
   * Sets the exact multiple scaling of this object
   */
  void setUniformScale(float multiple);

  /**
   * Uniformly scales the object so that the largest dimension is the given amount
   */
  void setUnitScale(float millimeters);

  /**
   * Sets the translation component of this object's transformation matrix
   */
  void translateAbsolute(const FAHVector3& translation);

  FAHVector3 translation() const;
  FAHVector3 rotation() const;
  FAHVector3 scaling() const;

  void rotateEuler(const FAHVector3& rotation);

  void scale(const FAHVector3& scaling);

  /**
   * Draws this mesh using OpenGL
   * @param transparent Set this flag to make the mesh semi-transparent.  This
   *                    is used while printing so the paths show up nicely.
   */
  void glRender(bool transparent);

  /**
   *
   */
  void glRenderHandles(PrintableObjectHandleType active_handles,
                       PrintableObjectHandleAxis mouse_axis,
                       bool dragging,
                       bool display_z_translate);

  /**
   * Finds the handle selected by the given pick ray.  Returns true if the handle/axis
   * parameters are filled with selected data.
   * @param pick_z_translate Whether or not to allow the user to grab the Z-axis handle
   */
  bool pickHandle(const FAHVector3& source,
                  const FAHVector3& pick_ray,
                  bool pick_z_translate,
                  PrintableObjectHandleType* handle,
                  PrintableObjectHandleAxis* axis);

  /**
   * Returns the handle axis of the object selected by the given ray.  This is used by
   * the main window to allow the user to select and manipulate object handles.
   */
  PrintableObjectHandleAxis pickHandleAxis(PrintableObjectHandleType type,
                                           const FAHVector3& source,
                                           const FAHVector3& pick_ray);

  /**
   * Given the camera's location, this allows the class to modify the transformation
   * values for this printable object.  The transformations are not combined with
   * the object's defined transformation values until the manipulation is finished.
   */
  void handleManipulate(PrintableObjectHandleType type,
                        PrintableObjectHandleAxis axis,
                        const FAHVector3& source,
                        const FAHVector3& start_ray,
                        const FAHVector3& end_ray);

  /**
   * When the user finishes manipulating an object with the handles, the changes can
   * be confirmed, in which case they are merged into the object, or cancelled.
   */
  void handleFinishManipulation(bool keep_changes);

  /**
   * Synchronizes the size and location of the bounding sphere with the
   * object's current transformation.
   */
  void updateBoundingSphere();

  /**
   * Obtains the sphere that contains all of the object's geometry
   */
  const FAHSphere& boundingSphere() const;

  /**
   * Returns the transformation matrix that has been applied to this object.  This
   * updates the internally stored matrices.
   */
  void getTransform(FAHMatrix4x4* transform);

  /**
   * Adjusts the vertical translation to place the lowest vertex of the transformed mesh
   * exactly at z=0.
   */
  void snapToRestOnXYPlane();

  /**
   * Returns a list of all of the fill material IDs referenced by one or more regions in
   * the printable object.
   */
  QSet<int> fillMaterialIDs() const;

  /**
   * Pairs the designated region index with the provided material name
   */
  void setFillMaterial(int fill_material_id, const QString& material_name);

  /**
   * Removes any material assignments from this object
   */
  void clearFillMaterials();

  /**
   * Returns a set of all of the material names that are assigned to regions in this
   * object.
   */
  QSet<QString> collectMaterialNames() const;

  /**
   * Obtains a pointer to all of the regions that have the fill material associated with
   * the given material name.
   */
  QList<AMFRegion*> printableRegionsAssignedMaterial(const QString& material_name);

  /**
   * Duplicates the internal mesh, transforms each vertex in this mesh and saves the pointer
   * internall.  This MUST be called before printing.
   */
  void buildPrintableMesh();

  /**
   * Returns a map of material names to the mesh structure that should be printed with a
   * tool using that material.
   */
  QMultiMap<QString,AMFRegion*> getMaterialNameToPrintableMeshMap();

  /**
   * Creates an internal triangle mesh that, when sliced and pathed, will support this mesh
   */
  void generateSupportingMesh();

  /**
   * Discovers the best orientation for this object, and rotates it to have that orientation
   */
  void setBestOrientation();

  /**
   * Defines the color used to draw a given material.  This is used to match draw colors to
   * the color of the material in the assignment panel.
   */
  void setRenderColor(int fill_material_id, const QColor& color);

  /**
   * Changes the color used to render all of the subsets using the given material
   */
  void setRenderColor(const QString& material_name, const QColor& color);

  /**
   * Obtains the color used to draw the given material id
   */
  bool getRenderColor(int fill_material_id, QColor* color);

  /**
   * Returns true if every fill material has an associated material name
   */
  bool hasCompleteMaterialAssignment() const;

  /**
   * Returns true if the given ray intersects a triangle in this mesh
   */
  bool intersectedByRay(const FAHVector3& origin, const FAHVector3& ray) const;

private:
  PrintableObject(AMFFile* amf);
  PrintableObject(PrintableObject&);
  void operator =(PrintableObject&);

  /**
   * Adjusts the position of each vertex in the AMF to put the middle of the bounding
   * cube at the origin.  This method is called in the constructor.
   */
  void moveCenterToOrigin();

  /**
   * Returns 'true' and fills 'point' if the pick line intersects the axial plane at
   * a point that is within the bounding circle drawing area.  These circles represent
   * the rotation handles.
   */
  bool intersectRotationHandlePoint(const FAHLine& pick_line,
                                    const FAHVector3& plane_normal,
                                    FAHVector3* point);

  /**
   * Returns 'true' and fills 'point' if the pick line intersects the translation
   * handle (arrow).  The arrow's intersection primitive is a small sphere around the
   * head of the arrow.
   */
  bool intersectTranslationHandlePoint(const FAHVector3& origin,
                                       const FAHVector3& ray,
                                       const FAHVector3& handle_offset,
                                       FAHVector3* point);

  /**
   * Returns true and fills 'point' if the pick line intersects the scaling handle
   * at the given offset.  The handle is modeled as a small sphere.
   */
  bool intersectScalingHandlePoint(const FAHVector3& origin,
                                   const FAHVector3& ray,
                                   const FAHVector3& handle_offset,
                                   FAHVector3* point);

  void glRenderRotateXYZHandles(PrintableObjectHandleAxis mouse_axis, bool dragging);

  /**
   * Cube handles are used for scaling the object
   */
  void glRenderCubeHandles(PrintableObjectHandleAxis mouse_axis, bool dragging);

  /**
   * Arrow handles allow the object to be translated
   */
  void glRenderArrowHandles(PrintableObjectHandleAxis mouse_axis, bool dragging, bool display_z);

  /**
   * Implements the dragging of a rotation handle by the mouse.  The 'start' and 'end'
   * parameters are the lines in 3d space formed by the mouse down/current mouse pick rays.
   */
  void handleManipulateRotation(PrintableObjectHandleAxis axis,
                                const FAHLine& start,
                                const FAHLine& end);

  /**
   * Implements the dragging of a translation handle by the mouse.  The 'start' and 'end'
   * parameters are the lines in 3d space formed by the mouse down/current mouse pick rays.
   */
  void handleManipulateTranslation(PrintableObjectHandleAxis axis,
                                   const FAHLine& start,
                                   const FAHLine& end);

  /**
   * Implements the dragging of a scaling handle by the mouse.  The 'start' and 'end'
   * parameters are the lines in 3d space formed by the mouse down/current mouse pick rays.
   */
  void handleManipulateScaling(PrintableObjectHandleAxis axis,
                               const FAHVector3& sphere_ray,
                               const FAHVector3& start_ray,
                               const FAHVector3& end_ray);

  /**
   * Determines the minimum and maximum per-axis values of the transformed AMF without the
   * translation component.
   */
  void findMinMaxOfScaledRotatedMesh(FAHVector3* min, FAHVector3* max);

  /**
   * Gets the list of all regions that are assigned the given material ID number
   */
  QList<AMFRegion*> printableRegionsAssignedMaterial(int fill_material_id);

private:

  /**
   * Returns the bounding cube face on which this object should rest for it to have the best
   * printing orientation
   * @author Chris Hogan
   */
  BoundingCubeFace findBestOrientation();

  /**
   * If the mesh was modified in some way that changes how it renders, call this method
   * so that the display list can be rebuilt.
   */
  void changed();

private: // local data
  AMFFile* amf_;
  AMFFile* printable_amf_;  // transformed version of the source mesh

  /// Cache for the OpenGL rendering instructions.  Set to -1 if invalid.
  int display_list_;

  /// Transformation parameters assigned to this object to change its location/size/rotation
  Transform current_;

  /// Additional transformation extending the 'current' transform when the user is using the
  /// mouse to adjust the object using handles.
  Transform manipulation_;
  bool manipulating_;

  /// Sphere containing all transformed object geometry
  FAHSphere bounding_sphere_;

  /// Maps a material ID number (in each AMFRegion) to the name of the material that will be
  /// used to print that material
  QMap<int, QString> fill_material_id_to_material_name_;

  QMap<int, QColor> fill_material_id_to_color_;

};

#endif // PRINTABLEOBJECT_H
