/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009, Sean Cretella (sac76@cornell.edu)
               2010, Karl Gluck (kwg8@cornell.edu)

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

#include <QtGlobal>
#include "openglcamera.h"
#include "math.h"
#include <cmath>
#include <qgl.h>


#define PI 3.141592f

OpenGLCamera::OpenGLCamera() {
  aspect = 1.0f;
  this->eye.set(150.0f, 250.0f, 150.0f);
  this->target.set(150.0f, 70.0f, 0.0f);
  this->up.set(0.0f, 0.0f, 1.0f);
  this->right.set(0.0f, 0.0f, 0.0f);
}


void OpenGLCamera::orbit(float mouseDeltaX, float mouseDeltaY) {

  GLVector3 negGaze;
  negGaze.set(eye);
  negGaze.sub(target);
  float dist = negGaze.length();
  negGaze.normalize();

  float azimuth = (float)atan2(negGaze.x, negGaze.y);
  float elevation = (float)atan2(negGaze.z, sqrt(negGaze.x * negGaze.x + negGaze.y * negGaze.y));
  azimuth = fmodf((azimuth + mouseDeltaX), (float) (2 * PI));
  elevation = (float) qMax(-PI * 0.495f, qMin(PI * 0.495f, (float)(elevation - mouseDeltaY)));

  negGaze.set((float) (sin(azimuth) * cos(elevation)),
              (float) (cos(azimuth) * cos(elevation)),
              (float) sin(elevation));
  negGaze.normalize();

  eye.set(target);

  GLVector3 temp;
  temp.set(negGaze.x, negGaze.y, negGaze.z);
  temp.scale(dist);

  eye.add(temp);

  updateFrame();
}

void OpenGLCamera::zoom(float d) {

    d = -d;

    GLVector3 gaze;
    gaze.set(target.x, target.y, target.z);
    gaze.sub(eye);

    double dist = gaze.length();
    gaze.normalize();
    d *= 6;

    if (dist + d > 0.01) {

        GLVector3 temp(gaze);
        temp.scale(-d);
        eye.add(temp);
    }
}


void OpenGLCamera::glSetCamera() {
  view.glView(eye, target, up);

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixd(view.copy().mul(projection).array());
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


OpenGLCamera::GLVector3 OpenGLCamera::raycastFromScreen(int pixel_x, int pixel_y) const {
//  return raycastFromViewport((2.0f * pixel_x - screen_width) / float(screen_width),
//                             (2.0f * (screen_height - pixel_y - 1) - screen_height) / float(screen_height));
  return raycastFromViewport( ((2.0f * pixel_x) / screen_width  - 1),
                             -((2.0f * pixel_y) / screen_height - 1));
}

OpenGLCamera::GLVector3 OpenGLCamera::raycastFromViewport(float vx, float vy) const {

  // Build the vector of the pick ray in screen space
  GLVector3 v(vx / projection._11, vy / projection._22, -1.0f);

  // Convert to world-space
  // TODO: store a copy of the inverse view matrix when the camera is
  //       updated instead of recalculating it, because this calc is expensive
  GLMatrix4x4 m(view);
  m.invert();

  // The below calculation is an expansion of this commented code to take advantage
  // of known-value terms in the transformation.
  //m._41 = 0;  m._42 = 0;  m._43 = 0; // don't involve the camera's location
  //m.transformInPlace(&v);
  v.set(v.x*m._11 + v.y*m._21 + v.z*m._31,
        v.x*m._12 + v.y*m._22 + v.z*m._32,
        v.x*m._13 + v.y*m._23 + v.z*m._33);

  // Normalize the pick ray and return it
  v.normalize();
  return v;
}


void OpenGLCamera::projectToScreen(const GLVector3& coordinate, float* x, float* y) const {
  // todo: store view.mul(projection) as view_proj_matrix_
  // todo: this doesn't work.
  GLVector3 projected(view.copy().mul(projection).transform(coordinate));
  *x =   screen_width * (1 + projected.x) / 2.0f;
  *y =  screen_height * (1 + projected.y) / 2.0f;
}



void OpenGLCamera::updateFrame() {

  projection.glProjectionPerspective(PI/4.0f, aspect, 1.0f, 1000.0f);
  up.normalize();
  view.glView(eye, target, GLVector3::make(0.0f,0.0f,1.0f));

  GLVector3 negGaze;
  negGaze.set(eye.x, eye.y, eye.z);
  negGaze.sub(target);
  negGaze.normalize();
  up.normalize();

  GLVector3 temp;
  temp.set(up.x, up.y, up.z);
  temp.cross(negGaze);
  right.set(temp.x, temp.y, temp.z);
  right.normalize();
  //up.set(negGaze.x, negGaze.y, negGaze.z);
  //up.cross(right);

}

void OpenGLCamera::setEye(float x, float y, float z) {
  eye.set(x, y, z);
  updateFrame();
}

void OpenGLCamera::setTarget(float x, float y, float z) {
  target.set(x, y, z);
  updateFrame();
}

void OpenGLCamera::setUp(float x, float y, float z) {
  up.set(x, y, z);
  updateFrame();
}

OpenGLCamera::GLVector3 OpenGLCamera::getEye() {
  return eye;
}

OpenGLCamera::GLVector3 OpenGLCamera::getTarget() {
  return target;
}

OpenGLCamera::GLVector3 OpenGLCamera::getUp() {
  return up;
}

void OpenGLCamera::setScreenSize(int width, int height) {
  screen_width = width;
  screen_height = height;
  aspect = float(width) / float(height);
  updateFrame();
}
