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
#include <common/confirm.h>
#include <common/todo.h>


class AMFTriangle {
public:

  /**
  * Default constructor
  * Creates an AMF triangle with vertex indices -1, -1, -1
  */
  AMFTriangle();

  /**
   * Copy the input
   */
  explicit AMFTriangle(const AMFTriangle* copy_source);

  /**
  * Constructor
  * Creates an AMF triangle with the specified vertex indices
  * @param v1 index for v1
  * @param v2 index for v2
  * @param v3 index for v3
  */
  AMFTriangle(int v1, int v2, int v3);


  /**
  * Fills the AMF triangle with the data in the triangle XML element
  * @param triangleElement the XML element containing the triangle data
  * @see WriteXml()
  */
  void ReadFromXml(QDomElement& triangleElement);

  /**
  * Outputs the AMF triangle data to an XML element
  * @param triangleElement the output XML element
  * @see ReadFromXml()
  */
  void WriteXml(QDomElement& triangleElement);


  /**
  * Determines whether or not this AMF triangle is degenerate
  * An AMF triangle is degenerate if its vertices do not determine a triangle.
  * That is, if any two of the triangle's vertex indices are equal the triangle is degenerate.
  * @return true if this AMF triangle is degenerate, false otherwise
  */
  bool isDegenerate() const;

  /**
  * Set the vertex index v1
  * @param v1 the new value for v1
  * @see getV1(), setV2(), setV3()
  */
  void setV1(int v1);

  /**
  * Set the vertex index v2
  * @param v2 the new value for v2
  * @see getV2(), setV1(), setV3()
  */
  void setV2(int v2);

  /**
  * Set the vertex index v3
  * @param v3 the new value for v3
  * @see getV3(), setV1(), setV2()
  */
  void setV3(int v3);

  /**
  * Get the vertex index v1
  * @return the value of vertex index v1
  * @see setV1(), getV2(), getV3()
  */
  int getV1() const;

  /**
  * Get the vertex index v2
  * @return the value of vertex index v2
  * @see setV2(), getV1(), getV3()
  */
  int getV2() const;

   /**
   * Get the vertex index v3
   * @return the value of vertex index v3
   * @see setV3(), getV1(), getV2()
   */
  int getV3() const;

private:
  int v1;
  int v2;
  int v3;
};
