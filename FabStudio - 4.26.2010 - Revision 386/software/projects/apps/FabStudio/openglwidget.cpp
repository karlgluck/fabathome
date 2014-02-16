/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009, Sean Cretella (sac78@cornell.edu)
               2010, Karl Gluck (kwg8@cornell.edu)

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

#include "openglwidget.h"
#include "math.h"
#include <cmath>
#include <QMouseEvent>

#include "openglcamera.h"

#include "mainwindow.h" // For passing through drag-and-drop messages to the main app window

#include <QTime>

#define PI 3.141592f
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

OpenGLWidget::OpenGLWidget(MainWindow *main_window,
                           QWidget *parent,
                           const char *name) : QGLWidget(parent, 0, 0), main_window_(main_window) {
  setMouseTracking(true);
  setAcceptDrops(true);
  setFocusPolicy(Qt::StrongFocus);

  zoom = 2.0f;
  z_angle = 0.0f;

  // initialize camera
  camera_ = new OpenGLCamera();
  next_target_ = camera_->getTarget();
  FAHVector3 eye_dest;
  eye_dest.set(next_target_).add(FAHVector3::make(150*zoom*sin(z_angle),150*zoom*cos(z_angle),150*zoom));
  camera_->setEye(eye_dest.x, eye_dest.y, eye_dest.z);


  hasClicked = false;
  wireframeMode = true;

  connect(&advance_camera_timer_, SIGNAL(timeout()), this, SLOT(advanceCameraToTarget()));

  setFocus();
}

void OpenGLWidget::dragEnterEvent(QDragEnterEvent* event) {
  main_window_->workspaceDragEnterEvent(event);
}


void OpenGLWidget::dropEvent(QDropEvent* event) {
  main_window_->workspaceDropEvent(event);
}


OpenGLCamera* OpenGLWidget::camera() {
  return camera_;
}

void OpenGLWidget::nudge(float right_left, float fwd_bwd) {
  Math::Vector3 fwd(sin(z_angle), cos(z_angle), 0.0);
  Math::Vector3 right(sin(z_angle + Math::kPi / 2), cos(z_angle + Math::kPi / 2), 0.0);
  fwd.scale(fwd_bwd);
  right.scale(right_left);
  next_target_.add(fwd).add(right);
  advance_camera_timer_.start(16);
}

void OpenGLWidget::addZoom(float delta) {
  zoom -= delta;
  advance_camera_timer_.start(16);
}

void OpenGLWidget::rotate(float right_left) {
  z_angle += right_left;
  advance_camera_timer_.start(16);
}



void OpenGLWidget::paintGL() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // render in wireframe mode or fill mode
  if (wireframeMode)
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  else
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

  camera_->glSetCamera();

  // Draw the scene
  main_window_->glRender();
}


void OpenGLWidget::initializeGL() {
  glShadeModel(GL_SMOOTH);
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f); //Black background

  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  resize(parentWidget()->size());
}

void OpenGLWidget::resizeGL(int width, int height) {
  if (height == 0) { //Prevent a divide-by-zero error
    height = 1;
  }

  glViewport(0, 0, (GLint)width, (GLint)height);

  camera_->setScreenSize(width, height);
  camera_->glSetCamera();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *e) {
  // pass to the main window
  main_window_->keyPressEvent(e);
}


/*
 *   Mouse support
 */

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {

    currentMouseX = this->mapFromGlobal(event->globalPos()).x();
    currentMouseY = this->mapFromGlobal(event->globalPos()).y();

    // convert from window to viewport
    int w = this->width();
    int h = this->height();
    currentMouseX = (2 * currentMouseX - w) / w;
    currentMouseY = (2 * (h - currentMouseY - 1) - h) / h;

    if (clickType == kClickZoom) {
      z_angle += (currentMouseX - lastMouseX) * 2.0f;
      float zoom_factor = (currentMouseY - lastMouseY) * 2.0f;
      zoom -= zoom_factor;
      if (zoom <= 0.1f) zoom = 0.1f;
      if (zoom >= 4.0f) zoom = 4.0f;
      if (!advance_camera_timer_.isActive()) advance_camera_timer_.start(16);
    }

    lastMouseX = currentMouseX;
    lastMouseY = currentMouseY;

    //update();

    main_window_->mouseMoveEvent(event);
}


void OpenGLWidget::mousePressEvent(QMouseEvent* event) {
  main_window_->mousePressEvent(event);

  Qt::MouseButtons buttons = event->buttons();
  bool left = buttons.testFlag(Qt::LeftButton);
  bool right = buttons.testFlag(Qt::RightButton);
  if (left && right) {
    clickType = kClickRotate;
  } else if (right) {
    clickType = kClickZoom;
  } else {
    return;
  }

  lastMouseX = this->mapFromGlobal(QCursor::pos()).x();
  lastMouseY = this->mapFromGlobal(QCursor::pos()).y();

  if (right) {
    rightDownX = lastMouseX;
    rightDownY = lastMouseY;
  }

  // convert from window to viewport
  int w = this->width();
  int h = this->height();
  lastMouseX = (2 * lastMouseX - w) / w;
  lastMouseY = (2 * (h - lastMouseY - 1) - h) / h;

}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::RightButton) {
    // was this a click or a drag?
    int x = this->mapFromGlobal(QCursor::pos()).x();
    int y = this->mapFromGlobal(QCursor::pos()).y();
    int dx = x - rightDownX;
    int dy = y - rightDownY;
    if (dx*dx+dy*dy < (10*10)) {
      // it was a click! re-target the camera
      FAHVector3 tgt = main_window_->screenCoordinatesToTrayLocation(QPoint(x,y));
      if (!main_window_->isOffOfBuildTray(tgt)) {
        next_target_ = tgt;
        advance_camera_timer_.start(16);
      }
    }
  }
  clickType = kClickNone;
  main_window_->mouseReleaseEvent(event);
}

void OpenGLWidget::advanceCameraToTarget() {

  // smooth to next target
  FAHVector3 target = camera_->getTarget();
  target.add(next_target_.copy().sub(target).scale(0.1f));
  camera_->setTarget(target.x, target.y, target.z);

  // smooth to next eye
  FAHVector3 eye = camera_->getEye();
  FAHVector3 eye_dest;
  eye_dest.set(target).add(FAHVector3::make(150*zoom*sin(z_angle),150*zoom*cos(z_angle),150*zoom));
  eye.add(eye_dest.sub(eye).scale(0.05f));
  if (eye_dest.distanceToSq(eye) < 4.0f) {
    eye.set(eye_dest);
    advance_camera_timer_.stop();
  }
  camera_->setEye(eye.x, eye.y, eye.z);

  main_window_->glRepaint();
}

void OpenGLWidget::mouseDoubleClickEvent(QMouseEvent* event) {
  main_window_->mouseDoubleClickEvent(event);
}
