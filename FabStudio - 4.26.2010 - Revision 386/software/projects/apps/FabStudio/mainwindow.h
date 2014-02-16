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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMutex> // used in asynchronous loading
#include <QTimer> // for gl repaint timer
#include <QMultiMap>

#include "shared/fabathome-constants.h" // for FAHVector3

namespace Ui
{
    class MainWindow;
}

class QDockWindow;

class OpenGLWidget;
class LoadObjectsPanel;
class PositionObjectsPanel;
class PrintableObject;
#include "printableobjecthandle.h"
class PrintPanel;
class SlicerProgressCallback;
class MaterialsPanel;
class ToolScript;
class DisplayToolbar;
class AMFRegion;
class Toolbar;
class LoadObjectFileThread;

/**
 *
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

  /**
   * Invoked by the OpenGLWidget when its dragEnterEvent method is called
   */
  void workspaceDragEnterEvent(QDragEnterEvent* event);

  /**
   * Invoked by the OpenGLWidget when its dropEvent method is called
   */
  void workspaceDropEvent(QDropEvent* event);

  /**
   * The Toolbar calls this method to bring up the corresponding tool-panels
   */
  void showLoadObjectsPanel();

  /**
   * The Toolbar calls this method to bring up the print scene tool-panel
   */
  void showPrintPanel();

  /**
   * The Toolbar calls this method to bring up the material selection dock widget
   */
  void showMaterialsPanel();

  /**
   * Invoked to display the position objects panel
   */
  void showPositionObjectsPanel();

  /**
   * Returns the 3d point on the printer tray that is beneath the given
   * screen-point.
   */
  FAHVector3 screenCoordinatesToTrayLocation(const QPoint& point);

  /**
   * Adds a loaded PrintableObject to the main workspace.  This method is
   * called by the asynchronous loading thread.
   */
  void addObject(const QString& source_file, PrintableObject* object);

  /**
   * Erases an object from the scene, making sure to remove all internal
   * references to the object.
   */
  void removeObject(PrintableObject* object);

  /**
   * Removes all material assignments from the currently selected object
   * and updates the state of the materials panel, check boxes and screen
   */
  void clearSelectedObjectFillMaterials();

  /**
   * Remove all material assignments.  Used when changing which printer is
   * selected.
   */
  void clearAllObjectFillMaterials();

  /**
   * This method is called by OpenGLWidget each time the scene needs to
   * be rendered.
   */
  void glRender();

  /**
   * Looks up and returns the given tool-script from the materials panel
   */
  ToolScript* getToolScriptByName(const QString& name);


  void mouseMoveEvent(QMouseEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseDoubleClickEvent(QMouseEvent* event);
  void keyPressEvent(QKeyEvent* event);

  /**
   * Returns true if, when the mouse moves over the scene, object handles
   * appear.  This is only the cause when (1) the position objects tab
   * is active and (2) the display objects checkbox is set.
   */
  bool handleManipulationEnabled() const;

  /**
   * Returns a set of pairs ("material name", region to print with material) based on the
   * materials assigned to objects.
   */
  QMultiMap<QString,AMFRegion*> getMaterialNameToPrintableMeshMap();

  /**
   * Turns on or off all "advanced" features of the program.  This is used to turn FabStudio
   * from a simple, general and fail-proof tool into a fully-featured 3d printing program
   * or back again.
   */
  void useAdvancedMode(bool enable);

  /**
   * Determines whether or not materials have been mapped for every object.
   * There must be at least 1 object for this function to return true.
   */
  bool allObjectsHaveMaterials() const;

  /**
   * Checks persistent application settings and determines if this is the first
   * time that the program has been run.  If it is, this method will pop up the
   * first execution dialog and reset the first-execution flag.
   */
  void checkFirstExecution();

  /**
   * Returns 'true' if the given object is completely beyond the build tray.
   * When this occurs, the object should be deleted.
   */
  bool isOffOfBuildTray(const PrintableObject* object) const;

  /**
   * Returns true if the given location is beyond the borders of the tray
   */
  bool isOffOfBuildTray(const FAHVector3& location) const;

  bool isInAdvancedMode() const;

public slots:

  /**
   * Forces the window to redraw the OpenGL scene
   */
  void glRepaint();

  /**
   * Synchronizes the checkmarks on buttons in the toolbar with the state of the
   * print scene.
   */
  void updateCheckMarks();

  /**
   * Sets up the build tray so that it all objects have their rotations and
   * positions set automatically.
   */
  void arrangeBuildTray();


private:
  Ui::MainWindow *ui;

  /**
   * Invoked by the Qt event loop when the window's size changes
   */
  void resizeEvent(QResizeEvent* event);

  /**
   * Returns the list of the names of all materials that are currently
   * assigned to objects in the scene
   */
  QSet<QString> collectAssignedMaterialNames();

 /**
  * Selects a new object, or deselects an old object if an object is already
  * selected.  Passing 'null' causes the current object to be selected and
  * no new object to be selected.
  */
  void selectObject(PrintableObject* object);

  /**
   * Changes the value of the check-box in the "options" menu and the
   * corresponding value in QSettings
   */
  void setAlwaysShowStartupInformation(bool show);

  /**
   * Synchronize the set of materials assigned to objects with those in
   * the materials panel, and rederive colors from that panel to give
   * to the objects.
   */
  void updateMaterialColorAssignments();


private: // Section for Fab@Home-specific members

  /// This object controls the main OpenGL renderer and all visual 3d effects
  OpenGLWidget* opengl_widget_;

  /// Provides ways of bringing objects into the scene
  LoadObjectsPanel* load_objects_panel_;

  /// Allows objects to be reoriented
  PositionObjectsPanel* position_objects_panel_;

  /// Handles slicing/pathing/toolscript
  PrintPanel* print_panel_;

  /// Allows objects to be given printing materials
  MaterialsPanel* materials_panel_;

  /// Each object in the scene
  QList<PrintableObject*> printable_objects_;

  /// Object currently selected for editing (it has the handles!)
  PrintableObject* selected_object_;

  /// Printable objects are loaded asynchronously, so access must use a mutex.  It is
  /// very rare that there is contention so this shouldn't affect performance.
  QMutex printable_objects_mutex_;

  /// When the mouse moves it changes these values.  They are used for manipulation.
  struct {
    PrintableObjectHandleType handle;
    PrintableObjectHandleAxis axis;
    PrintableObject* object; /// mouse over object
    bool down;
    FAHVector3 down_ray;  /// ray from camera where mouse was clicked
  } mouse_;

  /// Provides a breadcrumb of buttons
  Toolbar* toolbar_;

  /// Whether program is running in basic or advanced mode
  bool use_advanced_mode_;

  /// This timer is used to prevent calls to glRepaint from occuring more frequently
  /// than necessary.
  QTimer gl_repaint_timer_;

  /// Current thread loading an object file
  LoadObjectFileThread* load_object_file_thread_;

private slots:
  void on_actionShow_Information_at_Startup_triggered();
  void on_actionExit_triggered();
  void on_actionAbout_FabStudio_triggered();
  void on_actionVisit_FabAtHome_org_triggered();
  void on_actionEnable_Advanced_Features_triggered();
  void graphicsViewUpdateSize();
  void finishedLoadObjectFile();
};

#endif // MAINWINDOW_H
