/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009, Kevin Hobson (krh65@cornell.edu)

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
#include "amftriangle.h"

AMFTriangle::AMFTriangle() {
  v1 = -1;
  v2 = -1;
  v3 = -1;
}

AMFTriangle::AMFTriangle(const AMFTriangle* copy_source) {
  v1 = copy_source->v1;
  v2 = copy_source->v2;
  v3 = copy_source->v3;
}

AMFTriangle::AMFTriangle(int v1, int v2, int v3) {
  this->v1 = v1;
  this->v2 = v2;
  this->v3 = v3;
}

void AMFTriangle::ReadFromXml(QDomElement& triangleElement) {
  // Grab the vertices out of the attributes of the element
  v1 = triangleElement.attribute("V1").toInt();
  v2 = triangleElement.attribute("V2").toInt();
  v3 = triangleElement.attribute("V3").toInt();
}

void AMFTriangle::WriteXml(QDomElement& triangleElement) {
  triangleElement.setAttribute("V1", v1);
  triangleElement.setAttribute("V2", v2);
  triangleElement.setAttribute("V3", v3);
}

bool AMFTriangle::isDegenerate() const {
  return v1 == v2 || v2 == v3 || v3 == v1;
}

void AMFTriangle::setV1(int v1) {
  this->v1 = v1;
}

void AMFTriangle::setV2(int v2) {
  this->v2 = v2;
}

void AMFTriangle::setV3(int v3) {
  this->v3 = v3;
}

int AMFTriangle::getV1() const {
  return v1;
}

int AMFTriangle::getV2() const {
  return v2;
}

int AMFTriangle::getV3() const {
  return v3;
}
