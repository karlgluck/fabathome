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
#include "amfvertex.h"

AMFVertex::AMFVertex() {
  vertexId = -1;
}

AMFVertex::AMFVertex(const FAHVector3& point, int vertexId) : point(point) {
  this->vertexId = vertexId;
}

AMFVertex::~AMFVertex() {
}

void AMFVertex::ReadFromXml(QDomElement& vertexElement) {
  vertexId = vertexElement.attribute("VertexID", "").toInt();

  QDomElement vertexLocationElement = vertexElement.firstChildElement();
  float x = vertexLocationElement.attribute("X").toFloat();
  float y = vertexLocationElement.attribute("Y").toFloat();
  float z = vertexLocationElement.attribute("Z").toFloat();

  // Write into the vector3.
  point.set(x, y, z);
}

void AMFVertex::WriteXml(QDomElement& vertexElement) {
  QDomDocument theDoc = vertexElement.ownerDocument();

  // Write the vertex id
  vertexElement.setAttribute("VertexID", vertexId);

  QDomElement vertexLocationElement = theDoc.createElement("VertexLocation");
  vertexLocationElement.setAttribute("X", point.x);
  vertexLocationElement.setAttribute("Y", point.y);
  vertexLocationElement.setAttribute("Z", point.z);
  vertexElement.appendChild(vertexLocationElement);
}

FAHVector3& AMFVertex::getPoint() {
  return point;
}

const FAHVector3& AMFVertex::getPoint() const {
  return point;
}

int AMFVertex::getVertexID() const {
  return vertexId;
}
