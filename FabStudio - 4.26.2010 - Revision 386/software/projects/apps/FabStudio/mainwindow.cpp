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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openglwidget.h"
#include "openglcamera.h"

#include "toolbar.h"
#include "loadobjectspanel.h"
#include "positionobjectspanel.h"
#include "printableobject.h"
#include "loadobjectfilethread.h"
#include "printpanel.h"
#include "materialspanel.h"
#include "openglrenderprimitives.h" // used to render the ground grid

#include <QToolBar>
#include <QDebug>

#include <QDragEnterEvent>
#include <QSettings>

// for opening web browser
#include <QDesktopServices>
#include <QUrl>

#include "shared/slicer/slicer.h"
#include "arrangebuildtray.h"
#include "firstexecutiondialog.h"
#include "aboutdialog.h"

static const char kAdvancedModeSettingKey[] = "advanced_mode";
static const char kFirstExecutionSettingKey[] = "first_exec";
static const char kAlwaysShowStartupInformationSettingKey[] = "always_show_startup";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      opengl_widget_(0),
      load_objects_panel_(0),
      position_objects_panel_(0),
      print_panel_(0),
      materials_panel_(0),
      selected_object_(0),
      load_object_file_thread_(0)
{

  // Initialize the main window's settings
  ui->setupUi(this);
  setWindowTitle("Fab Studio");

  // Receive key press events
  setFocusPolicy(Qt::StrongFocus);

  { // Add the toolbar to the top of the window
    QToolBar* qtb = this->addToolBar("Toolbar");
    qtb->addWidget(toolbar_ = new Toolbar(this));
    qtb->setAllowedAreas(Qt::BottomToolBarArea | Qt::TopToolBarArea);
  }

  // Add all of the panels, then hide them until they're needed
  this->setDockOptions(QMainWindow::ForceTabbedDocks);
  QList<QDockWidget*> dock_widgets;
  dock_widgets.append(load_objects_panel_ = new LoadObjectsPanel());
  dock_widgets.append(position_objects_panel_ = new PositionObjectsPanel(this));
  dock_widgets.append(print_panel_ = new PrintPanel(this));
  dock_widgets.append(materials_panel_ = new MaterialsPanel(this));

  for (int i = 0; i < dock_widgets.size(); ++i) {
    QDockWidget* widget = dock_widgets.at(i);
    addDockWidget(Qt::RightDockWidgetArea, widget);

    // Add an action to the view menu that will display this widget
    QAction* action = ui->menuView->addAction(widget->windowTitle());
    QString key_sequence;
    QTextStream(&key_sequence) << "Ctrl+" << widget->windowTitle().at(0);
    action->setShortcut(QKeySequence(key_sequence));
    connect(action,SIGNAL(triggered()),widget,SLOT(show()));
    connect(action,SIGNAL(triggered()),widget,SLOT(raise()));

    todo("kwg8", "add the ability to detect when a dock widget is closed and update the screen!");

    if (i > 0) {
      tabifyDockWidget(dock_widgets.at(i-1), widget);
    }
  }

  // Set up the inter-panel communication channels
  connect(materials_panel_, SIGNAL(changedMaterials(QStringList)),
          print_panel_, SLOT(onChangedMaterials(QStringList)));
  connect(print_panel_, SIGNAL(glRepaint()),
          this, SLOT(glRepaint()), Qt::QueuedConnection);

  // Disable panels when printing
  connect(print_panel_, SIGNAL(printingStatus(bool)),
          position_objects_panel_, SLOT(setDisabled(bool)));
  connect(print_panel_, SIGNAL(printingStatus(bool)),
          load_objects_panel_, SLOT(setDisabled(bool)));
  connect(print_panel_, SIGNAL(printingStatus(bool)),
          materials_panel_, SLOT(setDisabled(bool)));
  connect(print_panel_, SIGNAL(printingStatus(bool)),
          toolbar_, SLOT(setDisabled(bool)));
  connect(print_panel_, SIGNAL(printingStatus(bool)),
          ui->menuView, SLOT(setDisabled(bool)));

  // Don't display these windows on startup
  print_panel_->close();
  materials_panel_->close();

  // Initialize the graphics view
  opengl_widget_ = new OpenGLWidget(this, ui->graphicsView, "OpenGLWidget");
  opengl_widget_->makeCurrent();
  opengl_widget_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  opengl_widget_->acceptDrops();
  connect(&gl_repaint_timer_, SIGNAL(timeout()), opengl_widget_, SLOT(repaint()));

  // Reset mouse information
  mouse_.object = 0;
  mouse_.handle = kNoHandles;
  mouse_.axis = kNoAxis;
  mouse_.down = false;

  // Make sure everything is switched to basic mode
  QSettings settings;
  bool advanced_mode = settings.value(kAdvancedModeSettingKey, false).toBool();
  ui->actionEnable_Advanced_Features->setChecked(advanced_mode);
  useAdvancedMode(advanced_mode);
  updateCheckMarks();

  setAlwaysShowStartupInformation(settings.value(kAlwaysShowStartupInformationSettingKey, false).toBool());
}



MainWindow::~MainWindow() {
  {
    QMutexLocker mutex(&printable_objects_mutex_);

    // Erase all loaded objects
    foreach (PrintableObject* object, printable_objects_) {
      PrintableObject::destroy(object);
    }
    printable_objects_.clear();
  }

  { // Erase the main panels
    delete position_objects_panel_;
    delete load_objects_panel_;
    delete print_panel_;
    delete materials_panel_;
  }

  delete ui;
}


void MainWindow::workspaceDragEnterEvent(QDragEnterEvent* event) {

  if (load_objects_panel_->isSourceOfEvent(event)) {
    event->acceptProposedAction();
  } else if (materials_panel_->isSourceOfEvent(event)) {
    event->acceptProposedAction();
  }
}

void MainWindow::workspaceDropEvent(QDropEvent* event) {
  QString parameter;
  if (load_objects_panel_->getDraggedObjectFile(event, &parameter)) {
    // The user dragged a file from the load objects panel into the main window,
    // so they want to load it into the workspace.  This takes time, so spawn
    // a thread to do it.
    load_objects_panel_->setEnabled(false); // disable the panel so they can't drag another
    ui->statusBar->showMessage("Currently loading " + parameter);
    FAHVector3 location = screenCoordinatesToTrayLocation(event->pos());
    load_object_file_thread_ = new LoadObjectFileThread(location, this, parameter);
    connect(load_object_file_thread_, SIGNAL(finished()), this, SLOT(finishedLoadObjectFile()), Qt::QueuedConnection);
    connect(load_object_file_thread_, SIGNAL(finished()), ui->statusBar, SLOT(clearMessage()), Qt::QueuedConnection);
    connect(load_object_file_thread_, SIGNAL(finished()), this, SLOT(updateCheckMarks()), Qt::QueuedConnection);
    connect(load_object_file_thread_, SIGNAL(finished()), this, SLOT(glRepaint()), Qt::QueuedConnection);
    load_object_file_thread_->start();
  }  else if (materials_panel_->getDraggedMaterialName(event, &parameter)) {
    // User is dropping a material on to an object.  Let's find out which object!

    QPoint p = opengl_widget_->mapFromGlobal(QCursor::pos());
    OpenGLCamera* camera = opengl_widget_->camera();
    FAHVector3 gv = camera->raycastFromScreen(p.x(), p.y());
    FAHVector3 eye = camera->getEye();

    PrintableObject* found = 0;
    float found_radius = 0.0f;
    foreach (PrintableObject* object, printable_objects_) {
      if (object->boundingSphere().intersects(eye, gv) && (!found || found_radius > object->boundingSphere().radius)) {
        found_radius = object->boundingSphere().radius;
        found = object;
      }
    }

    if (found) {
      // assign this material to the object
      QSet<int> fill_material_ids = found->fillMaterialIDs();
      bool material_chosen = true;
      int fill_material_id = 0;
      if (fill_material_ids.size() == 1) {
        fill_material_id = fill_material_ids.values().first();
      } else {

        todo("kwg8","this has not actually been tested");

        // pop a right-click menu
        QMenu menu(this);
        QMap<QAction*, int> action_to_material;
        foreach (int material_id, fill_material_ids) {
          QString str;
          str.sprintf("Material ID %d", material_id);
          action_to_material.insert(menu.addAction(str), material_id);
        }
        QAction* action = menu.exec(QCursor::pos());
        if (action) {
          fill_material_id = action_to_material.find(action).value();
        } else {
          material_chosen = false;
        }
      }
      if (material_chosen) {
        found->setFillMaterial(fill_material_id, parameter);

        updateMaterialColorAssignments();

        updateCheckMarks();
      }
    }
  }
}


void MainWindow::showLoadObjectsPanel() {
  load_objects_panel_->show();
  load_objects_panel_->raise();
}


void MainWindow::showPrintPanel() {
  print_panel_->show();
  print_panel_->raise();
}


void MainWindow::showMaterialsPanel() {
  materials_panel_->show();
  materials_panel_->raise();
}

void MainWindow::showPositionObjectsPanel() {
  position_objects_panel_->show();
  position_objects_panel_->raise();
}


FAHVector3 MainWindow::screenCoordinatesToTrayLocation(const QPoint& point) {
  OpenGLCamera* camera = opengl_widget_->camera();
  OpenGLCamera::GLVector3 gv = camera->raycastFromScreen(point.x(), point.y());
  OpenGLCamera::GLVector3 eye = camera->getEye();
  gv.scale(-eye.z/gv.z).add(eye); // intersect with z=0 plane
  FAHVector3 fv(gv.x, gv.y, gv.z);
  return fv;
}


void MainWindow::addObject(const QString& source_file, PrintableObject* object) {

  // Automatically reorient the object if the user is running in "basic" mode
  if (!ui->actionEnable_Advanced_Features->isChecked()) {
    object->setBestOrientation();
  }

  // Add the object to the scene
  QMutexLocker mutex(&printable_objects_mutex_);
  printable_objects_.append(object);
  object->snapToRestOnXYPlane();
}


void MainWindow::removeObject(PrintableObject* object) {
  confirm(object) else return;
  confirm(printable_objects_.removeOne(object));
  if (mouse_.object == object) {
    mouse_.object = 0;
  }
  if (selected_object_ == object) {
    selectObject(0);
    confirm(selected_object_ == 0);
  }
  PrintableObject::destroy(object);
  updateMaterialColorAssignments(); // calls glRepaint()
  updateCheckMarks();
}


void MainWindow::clearSelectedObjectFillMaterials() {
  if (!selected_object_) return;
  selected_object_->clearFillMaterials();
  updateMaterialColorAssignments();
  updateCheckMarks();
}

void MainWindow::clearAllObjectFillMaterials() {
  foreach (PrintableObject* object, printable_objects_)  {
    object->clearFillMaterials();
  }
  updateMaterialColorAssignments();
  updateCheckMarks();
}


void MainWindow::glRender() {
  if (!opengl_widget_) return;

  // Build area size in millimeters
  float print_area_width_ = 35.0f * 10.0f;
  float print_area_height_ = 28.0f * 10.0f;
  OpenGLPrimitives::renderGrid(print_area_width_, print_area_height_, 10.0f);

  QMutexLocker mutex(&printable_objects_mutex_);

  QPoint p = opengl_widget_->mapFromGlobal(QCursor::pos());

  OpenGLCamera* camera = opengl_widget_->camera();
  FAHVector3 gv = camera->raycastFromScreen(p.x(), p.y());
  FAHVector3 eye = camera->getEye();

  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

  // Draw the print-panel's preview BEFORE objects so that, if they are
  // rendered semi-transparently, the stuff inside can be seen
  print_panel_->glRender();

  // Draw each object in the scene
  foreach (PrintableObject* object, printable_objects_) {
    // Draw the object.  If a print is in progress, this object is
    // rendered semi-transparently.
    bool transparent = print_panel_->isPrintInProgress() ||
                       (object == selected_object_ && isOffOfBuildTray(object));
    object->glRender(transparent);
  }

  // Render the mouse-over effect for this object
  if (handleManipulationEnabled() && (mouse_.object != 0)) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    mouse_.object->glRenderHandles(mouse_.handle,
                                   mouse_.axis,
                                   mouse_.down,
                                   use_advanced_mode_ // only render Z axis in advanced mode
                                   );
    glPopMatrix();

    glDisable(GL_BLEND);
  }

  // Render the mouse-over effect for this object
  if (handleManipulationEnabled() && selected_object_ != 0) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    selected_object_->glRenderHandles(kNoHandles,
                                      kNoAxis,
                                      false,
                                      use_advanced_mode_);
    glPopMatrix();

    glDisable(GL_BLEND);
  }
}


ToolScript* MainWindow::getToolScriptByName(const QString& name) {
  return materials_panel_->getToolScriptByName(name);
}




void MainWindow::resizeEvent(QResizeEvent* event) {
  graphicsViewUpdateSize();
  QWidget::resizeEvent(event);
}


void MainWindow::mouseMoveEvent(QMouseEvent* event) {
  QPoint p = opengl_widget_->mapFromGlobal(QCursor::pos());

  // Get camera parameters
  OpenGLCamera* camera = opengl_widget_->camera();
  FAHVector3 gv = camera->raycastFromScreen(p.x(), p.y());
  FAHVector3 eye = camera->getEye();

  if (handleManipulationEnabled()) {

    if (mouse_.down) {

      if (mouse_.handle != kNoHandles &&
          mouse_.axis != kNoAxis) {

        // The mouse is being dragged and is adjusting some handle for the object
        confirm(mouse_.object) else {
          mouse_.handle = kNoHandles;
          mouse_.axis = kNoAxis;
          return;
        }

        // Pass the mouse's current coordinate to the object so it can be manipulated
        selectObject(mouse_.object);
        selected_object_->handleManipulate(mouse_.handle,
                                           mouse_.axis,
                                           eye,
                                           mouse_.down_ray,
                                           gv);

        // Update the position objects panel
        if (!position_objects_panel_->isTopLevel()) showPositionObjectsPanel();
        position_objects_panel_->manipulatedObject(selected_object_);
        position_objects_panel_->showDeletingObjectLabel(isOffOfBuildTray(selected_object_));

        switch (mouse_.handle) {
        default:
          confirm (false && "Handle should always be valid");
        case kScalingHandles:     position_objects_panel_->showScaling(); break;
        case kRotationHandles:    position_objects_panel_->showRotation(); break;
        case kTranslationHandles:
        case kTranslationHandles_NoZAxis: position_objects_panel_->showTranslation(); break;
        }
      }

    } else {

      // Find a handle on some object to highlight when the mouse runs over it

      // Reset the currently selected axis
      mouse_.axis = kNoAxis;

      if (mouse_.object) {

        // Only test the active object
        if (!mouse_.object->pickHandle(eye,
                                       gv,
                                       use_advanced_mode_, // only pick Z translate axis in advanced mode
                                       &mouse_.handle,
                                       &mouse_.axis)) {
          // Reset the active object if the mouse strays too far from the bounding sphere
          FAHSphere big_sphere(mouse_.object->boundingSphere());
          big_sphere.radius *= 2.0f;
          if (!big_sphere.intersects(eye, gv)) {
            mouse_.object = 0;
          }
        }
      }

      // Check each object in the scene.  Pick the handle of the smallest object that it is
      // over.  This allows objects within other objects to still be selected.
      foreach (PrintableObject* object, printable_objects_) {
        if (object->pickHandle(eye,
                               gv,
                               use_advanced_mode_, // z translate axis in advanced mode only
                               &mouse_.handle,
                               &mouse_.axis)) {
          if (!mouse_.object ||
               object->boundingSphere().radius < mouse_.object->boundingSphere().radius) {
            mouse_.object = object;
          }
        }
      }

      // If no axis was found for any object, remove the selection display
      if (mouse_.axis == kNoAxis) {
        mouse_.handle = kNoHandles;
      }
    }

  }

  // Make sure the scene is redrawn
  glRepaint();

  // Pick a shape for the cursor based on which handle, if any, is active
  Qt::CursorShape shape = Qt::ArrowCursor;
  if (!print_panel_->isPrintInProgress() &&
      (mouse_.axis != kNoAxis)) {
    switch (mouse_.handle) {
    default:
      confirm (false && "Handle should always be valid if there is no axis");
    case kScalingHandles:
    case kRotationHandles:    shape = mouse_.down ? Qt::ClosedHandCursor : Qt::OpenHandCursor; break;
    case kTranslationHandles: shape = (mouse_.axis == kZOnly) ? Qt::SizeVerCursor : Qt::SizeAllCursor; break;
    case kTranslationHandles_NoZAxis: shape = Qt::SizeAllCursor; break;
    }
  }
  opengl_widget_->setCursor(QCursor(shape));
  QMainWindow::mouseMoveEvent(event);
}


void MainWindow::mousePressEvent(QMouseEvent* event) {
  QPoint p = opengl_widget_->mapFromGlobal(QCursor::pos());
  OpenGLCamera* camera = opengl_widget_->camera();
  FAHVector3 down_ray = camera->raycastFromScreen(p.x(), p.y());

  if (handleManipulationEnabled()) {
    bool left = event->buttons().testFlag(Qt::LeftButton);
    bool right = event->buttons().testFlag(Qt::RightButton);
    if (right && left && mouse_.object) {
      // cancel manipulating
      mouse_.object->handleFinishManipulation(false);
      mouse_.object = 0;
      mouse_.handle = kNoHandles;
      mouse_.axis = kNoAxis;
    } else if (left && mouse_.object) {
      selectObject(mouse_.object);
      mouse_.down = true;
      mouse_.down_ray = down_ray;
    }
  }

  QMainWindow::mousePressEvent(event);
}


void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
  OpenGLCamera* camera = opengl_widget_->camera();
  QPoint p = opengl_widget_->mapFromGlobal(QCursor::pos());
  FAHVector3 ray = camera->raycastFromScreen(p.x(), p.y());

  if (handleManipulationEnabled() &&
      !event->buttons().testFlag(Qt::LeftButton)) {
    mouse_.down = false;

    // Apply transformations to the object
    if (mouse_.object) {
      mouse_.object->handleFinishManipulation(true);

      // Did they drop the object outside of the build area?
      if (isOffOfBuildTray(mouse_.object)) {
        removeObject(mouse_.object);
      } else {
        // In basic mode, automatically reposition the object after a rotation
        // so that it rests on the build tray.
        if ((mouse_.handle == kRotationHandles ||
             mouse_.handle == kScalingHandles) && !use_advanced_mode_) {
          mouse_.object->snapToRestOnXYPlane();
        }
      }
    }

    // Reset the mouse information
    mouse_.handle = kNoHandles;
    mouse_.axis = kNoAxis;

    // Deselect the object if the mouse was released off of it
    if (mouse_.object != selected_object_ &&
        selected_object_ &&
        !selected_object_->intersectedByRay(
            camera->getEye(),
            ray)) {
      selectObject(0);
      glRepaint();
    }
  }
  QMainWindow::mouseReleaseEvent(event);
}


void MainWindow::mouseDoubleClickEvent(QMouseEvent* event) {

  // Snap to XY plane if double-click Z translate axis.  This is only
  // visible in advanced mode, so we don't have to check for it otherwise.
  if (use_advanced_mode_ && handleManipulationEnabled()) {

    // Get camera parameters
    OpenGLCamera* camera = opengl_widget_->camera();
    QPoint p = opengl_widget_->mapFromGlobal(QCursor::pos());
    FAHVector3 ray = camera->raycastFromScreen(p.x(), p.y());
    FAHVector3 eye = camera->getEye();

    PrintableObjectHandleType handle;
    PrintableObjectHandleAxis axis;
    if (mouse_.object && mouse_.object->pickHandle(eye, ray, true, &handle, &axis)) {
      if (handle == kTranslationHandles && axis == kZOnly) {
        // auto-snap to place on tray
        mouse_.object->snapToRestOnXYPlane();
      }
    }
  }

  glRepaint();

  QMainWindow::mouseDoubleClickEvent(event);
}


void MainWindow::keyPressEvent(QKeyEvent* event) {
  bool shift = event->modifiers().testFlag(Qt::ShiftModifier);
  switch (event->key()) {
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
      if (selected_object_) removeObject(selected_object_);
      break;

    case Qt::Key_Left:
      if (shift) opengl_widget_->rotate(-0.15); else opengl_widget_->nudge(-10.0f, 0.0f);
      break;

    case Qt::Key_Right:
      if (shift) opengl_widget_->rotate(+0.15); else opengl_widget_->nudge(+10.0f, 0.0f);
      break;

    case Qt::Key_Up:
      if (shift) opengl_widget_->addZoom(+0.1); else opengl_widget_->nudge(0.0f, -10.0f);
      break;

    case Qt::Key_Down:
      if (shift) opengl_widget_->addZoom(-0.1); else opengl_widget_->nudge(0.0f, +10.0f);
      break;
  }
}



bool MainWindow::handleManipulationEnabled() const {
  return !print_panel_->isPrintInProgress();
}

void MainWindow::glRepaint() {
  if (gl_repaint_timer_.isActive()) {
    return;
  }
  opengl_widget_->repaint();
  gl_repaint_timer_.setSingleShot(true);
  gl_repaint_timer_.start(1000/60);
}


void MainWindow::updateCheckMarks() {
  bool load_objects = !printable_objects_.isEmpty();
  bool assign_materials = allObjectsHaveMaterials();
  bool send_to_printer = false;
  toolbar_->setCheckMarks(load_objects,
                          assign_materials,
                          send_to_printer);


  // Only enable panels once their prereqs have been fulfilled.
  // Disabling is IMPORTANT because even though the panels are
  // closed, the user can right-click the toolbar to bring
  // them back...and there's not much we can do to stop that.

  // need objects before materials / position objects
  bool object_editing_panels_enabled = load_objects;
  if (!object_editing_panels_enabled) {
    position_objects_panel_->close();
    materials_panel_->close();
  }
  position_objects_panel_->setEnabled(object_editing_panels_enabled);
  materials_panel_->setEnabled(object_editing_panels_enabled);

  // need objects w/ materials before printing
  bool print_panel_enabled = load_objects && assign_materials;
  if (!print_panel_enabled) print_panel_->close();
  print_panel_->setEnabled(print_panel_enabled);

  // This disables or enables menu items based on the scene
  // Search by an index or something, rather than having to do a
  // big for... loop
  todo("kwg8","Find items in the View menu less hackishly");
  foreach (QAction* action, ui->menuView->actions()) {
    if (action->text() == position_objects_panel_->windowTitle()) {
      action->setEnabled(object_editing_panels_enabled);
    }
    if (action->text() == materials_panel_->windowTitle()) {
      action->setEnabled(object_editing_panels_enabled);
    }
    if (action->text() == print_panel_->windowTitle()) {
      action->setEnabled(print_panel_enabled);
    }
  }
}


void MainWindow::arrangeBuildTray() {

  { // Re-orient each object
    QMutexLocker locker(&printable_objects_mutex_);
    foreach (PrintableObject* object, printable_objects_) {
      object->setBestOrientation();
    }
  }

  // Find the footprints of the objects
  QList<FAHLoopInXYPlane> footprints;
  {
    todo("kwg8", "find better footprints; for now just use the bounding sphere");
    QMutexLocker locker(&printable_objects_mutex_);
    foreach (PrintableObject* object, printable_objects_) {
      const FAHSphere& sphere = object->boundingSphere();
      const float r = sphere.radius;
      FAHLoopInXYPlane footprint;
      footprint.add(sphere.center.copy().add(FAHVector3::make(-r,-r,0)));
      footprint.add(sphere.center.copy().add(FAHVector3::make(-r,+r,0)));
      footprint.add(sphere.center.copy().add(FAHVector3::make(+r,+r,0)));
      footprint.add(sphere.center.copy().add(FAHVector3::make(+r,-r,0)));
      footprints.append(footprint);
    }
  }

  // Get the build tray layout
  todo("kwg8", "Set the correct build area width based on printer setting");
  float build_area_width_ = 250.0f;
  QList<PositionAndOrientation> layout = ::arrangeBuildTray(footprints, build_area_width_);

  // Assign the layout
  for (int i = 0; i < layout.size(); ++i) {
    const PositionAndOrientation& item = layout.at(i);
    PrintableObject* object = printable_objects_[i];
    object->translateAbsolute(FAHVector3::make(item.x, item.y, 0.0f));
    todo("kwg8", "set the rotation of the object");
    object->snapToRestOnXYPlane();
  }

  if (selected_object_) {
    position_objects_panel_->manipulatedObject(selected_object_);
  }

  glRepaint();
}


QMultiMap<QString,AMFRegion*> MainWindow::getMaterialNameToPrintableMeshMap() {
  QMutexLocker locker(&printable_objects_mutex_);
  QMultiMap<QString,AMFRegion*> map;
  foreach (PrintableObject* object, printable_objects_) {

    // Make sure the mesh has been constructed
    object->buildPrintableMesh();

    // Add the mapping to our output set
    map.unite(object->getMaterialNameToPrintableMeshMap());
  }
  return map;
}


void MainWindow::useAdvancedMode(bool enable) {
  QSettings settings;
  settings.setValue(kAdvancedModeSettingKey, enable);
  if (use_advanced_mode_ && !enable) {
    // set everything to rest on the build tray
    foreach (PrintableObject* object, printable_objects_) {
      object->snapToRestOnXYPlane();
    }
    glRepaint();
  }
  use_advanced_mode_ = enable;
  load_objects_panel_->useAdvancedMode(enable);
  position_objects_panel_->useAdvancedMode(enable);
  print_panel_->useAdvancedMode(enable);
  materials_panel_->useAdvancedMode(enable);
}


bool MainWindow::allObjectsHaveMaterials() const {
  bool all = !printable_objects_.isEmpty();
  foreach (PrintableObject* po, printable_objects_) {
    all = all && po->hasCompleteMaterialAssignment();
  }
  return all;
}


void MainWindow::checkFirstExecution() {
  QSettings settings;
  if (settings.value(kFirstExecutionSettingKey, true).toBool() ||
      settings.value(kAlwaysShowStartupInformationSettingKey, false).toBool()) {

    // run the first execution dialog
    FirstExecutionDialog dialog;
    dialog.exec();
  }
  settings.setValue(kFirstExecutionSettingKey, false);
}


bool MainWindow::isOffOfBuildTray(const PrintableObject* object) const {
  const FAHSphere& sphere = object->boundingSphere();
  return sphere.radius + sphere.center.x < 0 ||
         sphere.radius + sphere.center.y < 0;
}


bool MainWindow::isOffOfBuildTray(const FAHVector3& location) const {
  return location.x < 0 || location.y < 0;
}


bool MainWindow::isInAdvancedMode() const {
  return use_advanced_mode_;
}


void MainWindow::graphicsViewUpdateSize() {
  opengl_widget_->resize(ui->graphicsView->size());
  glRepaint();
}


QSet<QString> MainWindow::collectAssignedMaterialNames() {
  QSet<QString> material_names;
  QMutexLocker locker(&printable_objects_mutex_);
  foreach (PrintableObject* object, printable_objects_) {
    material_names += object->collectMaterialNames();
  }
  return material_names;
}

void MainWindow::selectObject(PrintableObject* object) {
  if (selected_object_ != object) {
    if (!object) {
      selected_object_ = 0;
    } else {
      selected_object_ = object;
//      showPositionObjectsPanel();
    }
  }

  position_objects_panel_->setSelectedObject(selected_object_);
  materials_panel_->setSelectedObject(selected_object_);
}

void MainWindow::setAlwaysShowStartupInformation(bool show) {
  QSettings settings;
  settings.setValue(kAlwaysShowStartupInformationSettingKey, show);
  ui->actionShow_Information_at_Startup->setChecked(show);
}

void MainWindow::updateMaterialColorAssignments() {
  // Sync the materials panel
  QSet<QString> assigned_materials = collectAssignedMaterialNames();
  const QMap<QString, QColor>& color_assignments
      = materials_panel_->updateAssignedMaterials(assigned_materials);

  // Based on assigned colors, set this object's subsets' colors
  foreach (PrintableObject* object, printable_objects_) {
    for (QMap<QString,QColor>::const_iterator i = color_assignments.begin();
                                              i != color_assignments.end();
                                            ++i) {
      object->setRenderColor(i.key(), i.value());
    }
  }

  // Redraw the screen since colors may have changed
  glRepaint();
}


void MainWindow::on_actionEnable_Advanced_Features_triggered() {
  useAdvancedMode(ui->actionEnable_Advanced_Features->isChecked());
}

void MainWindow::finishedLoadObjectFile() {
  load_objects_panel_->setEnabled(true);
  delete load_object_file_thread_;
  load_object_file_thread_ = 0;
  showMaterialsPanel();
}

void MainWindow::on_actionVisit_FabAtHome_org_triggered() {
  todo("kwg8", "do something (pop failure msg) if we can't open the website");
  QDesktopServices::openUrl(QUrl("http://www.fabathome.org/"));
}

void MainWindow::on_actionAbout_FabStudio_triggered() {
  AboutDialog dialog;
  dialog.exec();
}

void MainWindow::on_actionExit_triggered() {
  close();
}

void MainWindow::on_actionShow_Information_at_Startup_triggered() {
  setAlwaysShowStartupInformation(ui->actionShow_Information_at_Startup->isChecked());
}
