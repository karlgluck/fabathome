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

#include <QList>
#include <QDomNode>

#include "amfmesh.h"

class AMFObject {
public:

  /**
   * Units enumeration
   * The types of units used by models
   */
  enum Unit {
    Unknown,     // Unknown units
    Milimeters,  // Milimeters
    Inches       // Inches
  };

  /**
   * Default constructor
   * Creates an empty object with unknown units
   */
  AMFObject();

  /**
   * Creates a deep-copy of the given source object
   */
  explicit AMFObject(const AMFObject* source);

  /**
   * Destructor
   * Frees all memory held by AMFObject
   */
  ~AMFObject();

  /**
   * Fills the AMF object with the data in the object XML element
   * @param objectElement the XML element containing the object data
   * @see WriteXml()
   */
  void ReadFromXml(QDomElement& objectElement);

  /**
   * Outputs the AMF object data to an XML element
   * @param objectElement the output XML element
   * @see ReadFromXml()
   */
  void WriteXml(QDomElement& objectElement);

  /**
   * Get the mesh that makes up this AMF object element
   * First asserts that there is only one mesh for this AMF object, as there would be for an STL model
   * @return the single mesh object that makes up this AMF object
   * @see GiveMesh()
   */
  AMFMesh* GetMesh(); // Return the internal mesh (assert that there's only one)
  const AMFMesh* GetMesh() const;

  /**
   * Erase all current meshes and add a new mesh to this AMF object
   * @param newMesh the mesh to add to this AMF object element
   * @see GetMesh()
   */
  void GiveMesh(AMFMesh* newMesh); // Erase all current meshes and add this new mesh

   // Do we want an AddMesh that would keep all current meshes and add a new one?

private:
  Unit units;
  QList<AMFMesh*> meshes;
};

