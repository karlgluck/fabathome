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
#include "openglrenderprimitives.h"
#include <qgl.h>
#include <math.h> // cos, sin

namespace OpenGLPrimitives {

void renderArrow(float offset) {
  const double PI = 3.1415926535;
  const int arrowDivs = 32;
  const double arrowTailRadius = (double)0.05;
  const double arrowHeadRadius = (double)0.11;

  // tail coney
  double theta = 0;
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d((double)0, (double)offset, (double)0);
  for (double i = 0; i <= arrowDivs; ++i) {
    theta = (i / arrowDivs) * PI * 2;
    glVertex3d(cos(theta) * arrowTailRadius, offset+0.5, sin(theta) * arrowTailRadius);
  }
  glEnd();

  // neck ring
  glBegin(GL_QUAD_STRIP);
  for (double i = 0; i <= arrowDivs; ++i) {
    theta = (i / arrowDivs) * PI * 2;
    glVertex3d(cos(theta) * arrowTailRadius, offset+0.5, sin(theta) * arrowTailRadius);
    glVertex3d(cos(theta) * arrowHeadRadius, offset+0.53, sin(theta) * arrowHeadRadius);
  }
  glEnd();

  // head cone
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d((double)0, (double)offset+0.7, (double)0);
  for (double i = 0; i <= arrowDivs; ++i) {
    theta = (i / arrowDivs) * PI * 2;
    glVertex3d(cos(theta) * arrowHeadRadius, offset+0.53, sin(theta) * arrowHeadRadius);
  }
  glEnd();
}


void renderScalingCorner() {
  glBegin(GL_QUADS);
  // XY
  glVertex3f( 0.0f,  0.0f,  0.0f);
  glVertex3f( 1.0f,  0.0f,  0.0f);
  glVertex3f( 1.0f,  0.2f,  0.0f);
  glVertex3f( 0.0f,  0.2f,  0.0f);
  glVertex3f( 0.0f,  0.2f,  0.0f);
  glVertex3f( 0.2f,  0.2f,  0.0f);
  glVertex3f( 0.2f,  1.0f,  0.0f);
  glVertex3f( 0.0f,  1.0f,  0.0f);
  // XZ
  glVertex3f( 0.0f,  0.0f,  0.0f);
  glVertex3f( 1.0f,  0.0f,  0.0f);
  glVertex3f( 1.0f,  0.0f,  0.2f);
  glVertex3f( 0.0f,  0.0f,  0.2f);
  glVertex3f( 0.0f,  0.0f,  0.2f);
  glVertex3f( 0.2f,  0.0f,  0.2f);
  glVertex3f( 0.2f,  0.0f,  1.0f);
  glVertex3f( 0.0f,  0.0f,  1.0f);
  // YZ
  glVertex3f(  0.0f, 0.0f,  0.0f);
  glVertex3f(  0.0f, 1.0f,  0.0f);
  glVertex3f(  0.0f, 1.0f,  0.2f);
  glVertex3f(  0.0f, 0.0f,  0.2f);
  glVertex3f(  0.0f, 0.0f,  0.2f);
  glVertex3f(  0.0f, 0.2f,  0.2f);
  glVertex3f(  0.0f, 0.2f,  1.0f);
  glVertex3f(  0.0f, 0.0f,  1.0f);
  glEnd();
  glBegin(GL_TRIANGLES);
  glEnd();
}


void renderGrid(float width, float height, float spacing) {
  glPushMatrix();
  glLoadIdentity();
  glPushAttrib(GL_LIGHTING);
  glDisable(GL_LIGHTING);
  glColor3f(0.4f, 0.4f, 0.4f);
  glBegin(GL_LINES);

  for (int i = width / spacing; i >= 0; --i) {
    float x = spacing * i;
    glVertex3f((float)x, (float)0.0f, 0.0f);
    glVertex3f((float)x, (float)height, 0.0f);
  }

  for (int i = height / spacing; i >= 0; --i) {
    float y = spacing * i;
    glVertex3f((float)0.0f,  (float)y, 0.0f);
    glVertex3f((float)width, (float)y, 0.0f);
  }

  glVertex3f(width, 0.0f, 0.0f);
  glVertex3f(width, height, 0.0f);

  glVertex3f(0.0f, height, 0.0f);
  glVertex3f(width, height, 0.0f);

  glEnd();
  glPopAttrib();
  glPopMatrix();
}

}
