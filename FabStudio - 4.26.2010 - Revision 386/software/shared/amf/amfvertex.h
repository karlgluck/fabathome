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
#pragma once

#include <QDomNode>

#include "shared/fabathome-constants.h"
//typedef int Vector3;

class AMFVertex {
public:

  /**
  * Default constructor
  * Creates an AMF vertex with point (0,0,0) and vertexID -1
  */
  AMFVertex();

  /**
  * Constructor
  * Creates an AMF vertex with the specified point and vertex ID
  * @param point the location of the vertex
  * @param vertexID the id of this vertex
  */
  AMFVertex(const FAHVector3& point, int vertexID);

  /**
  * Destructor
  * Frees all memory held by this AMF vertex
  */
  ~AMFVertex();

  /**
  * Fills the AMF vertex with the data in the vertex XML element
  * @param vertexElement the XML element containing the vertex data
  * @see WriteXml()
  */
  void ReadFromXml(QDomElement& vertexElement);

  /**
  * Outputs the AMF vertex data to an XML element
  * @param vertexElement the output XML element
  * @see ReadFromXml()
  */
  void WriteXml(QDomElement& vertexElement);

  /**
  * Get the vector3 point of this vertex
  * @return the point at which this vertex exists
  */
  FAHVector3& getPoint();

  /**
   * Get the vector3 point of this vertex
   * @return the point at which this vertex exists
   */
  const FAHVector3& getPoint() const;

  /**
  * Get this vertex's vertex id
  * @return this vertex's vertex id
  */
  int getVertexID() const;

private:
  FAHVector3 point;
  int vertexId;
};
