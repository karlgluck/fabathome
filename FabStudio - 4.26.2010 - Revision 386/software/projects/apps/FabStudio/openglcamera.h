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
#pragma once
#ifndef OPENGLCAMERA_H
#define OPENGLCAMERA_H

#include "shared/fabathome-constants.h"

class OpenGLCamera {
public:
  typedef Math::Vector3 GLVector3;
  typedef Math::Matrix4x4 GLMatrix4x4;

public:
  OpenGLCamera();

  void orbit(float mouseDeltaX, float mouseDeltaY);
  void zoom(float d);
  void setEye(float x, float y, float z);
  void setTarget(float x, float y, float z);
  void setUp(float x, float y, float z);

  GLVector3 getEye();
  GLVector3 getTarget();
  GLVector3 getUp();

  void setScreenSize(int width, int height);

  /**
   * Initializes the view and projection matrices in the OGL device
   */
  void glSetCamera();

  /**
   * Returns the pick ray in 3d space that passes from the eye through
   * the given pixel on the screen.
   */
  GLVector3 raycastFromScreen(int pixel_x, int pixel_y) const;

  /**
   * Returns the vector in 3d space defined by casting a ray from this
   * camera through the given viewport coordinates.  Obtain vx & vy by
   * dividing the mouse's x & y coordinates by the width and height of
   * the screen, respectively.
   */
  GLVector3 raycastFromViewport(float vx, float vy) const;

  /**
   * Outputs the 2d pixel location of the given coordinate in 3d space
   */
  void projectToScreen(const GLVector3& coordinate, float* x, float* y) const;

protected:
  int screen_width, screen_height;
  float aspect;
  GLVector3 eye, target, up, right;

  GLMatrix4x4 projection, view;


private:

  void updateFrame();
};



#endif // OPENGLCAMERA_H
