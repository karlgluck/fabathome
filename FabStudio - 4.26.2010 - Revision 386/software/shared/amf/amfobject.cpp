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
#include "amfobject.h"

AMFObject::AMFObject() {
  units = Unknown;
}

AMFObject::AMFObject(const AMFObject* copy_source) {
  units = copy_source->units;
  foreach(const AMFMesh* source_mesh, copy_source->meshes) {
    meshes.append(new AMFMesh(source_mesh));
  }
}

AMFObject::~AMFObject() {
  // Delete the meshes
  for (int i = 0; i < meshes.size(); i++) {
    delete(meshes[i]);
  }
}

void AMFObject::ReadFromXml(QDomElement& objectElement) {
  // Set the units
  if (objectElement.attribute("Units").toLower() == "mm")
    units = Milimeters;
  else if (objectElement.attribute("Units").toLower() == "in")
    units = Inches;

  // Get the mesh elements
  QDomNodeList meshNodes = objectElement.elementsByTagName("Mesh");

  meshes.clear();
  for (unsigned int i = 0; i < meshNodes.length(); i++) {
    AMFMesh* newAmfMesh = new AMFMesh();
    meshes.append(newAmfMesh);
    QDomElement meshElement = meshNodes.at(i).toElement();
    newAmfMesh->ReadFromXml(meshElement);
  }
}

void AMFObject::WriteXml(QDomElement& objectElement) {
  QDomDocument theDoc = objectElement.ownerDocument();

  // Write the units
  if (units == Milimeters)
    objectElement.setAttribute("Units", "mm");
  else if (units == Inches)
    objectElement.setAttribute("Units", "in");

  // Create the mesh elements
  for (int i = 0; i < meshes.size(); i++) {
    QDomElement meshElement = theDoc.createElement("Mesh");
    meshes[i]->WriteXml(meshElement);
    objectElement.appendChild(meshElement);
  }
}

AMFMesh* AMFObject::GetMesh() {
  // Assert that there is exactly one mesh

  confirm(meshes.size() == 1) then {
    return meshes.at(0);
  }

  // If there are multiple meshes, return the first one
  if (meshes.size() > 0)
    return meshes.at(0);

  // If there are no meshes, return a null pointer
  return NULL;
}

const AMFMesh* AMFObject::GetMesh() const {
  // Assert that there is exactly one mesh

  confirm(meshes.size() == 1) then {
    return meshes.at(0);
  }

  // If there are multiple meshes, return the first one
  if (meshes.size() > 0)
    return meshes.at(0);

  // If there are no meshes, return a null pointer
  return NULL;
}

void AMFObject::GiveMesh(AMFMesh* newMesh) {
  // Delete the old meshes
  int i;
  for (i = 0; i < meshes.size(); i++) {
    delete(meshes[i]);
  }
  meshes.clear();

  // Add the new mesh
  meshes.append(newMesh);
}
