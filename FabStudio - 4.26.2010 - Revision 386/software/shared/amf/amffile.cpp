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
#include "amffile.h"
#include "amfobject.h"

#include "shared/fabathome-constants.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>

AMFFile::AMFFile() { }

AMFFile::AMFFile(const AMFFile* copy_source) {
  foreach (const AMFObject* object, copy_source->objects) {
    objects.append(new AMFObject(object));
  }
}

AMFFile::~AMFFile() {
  clear();
}


bool AMFFile::readFromFile(QString path) {

  // Remove anything currently in this structure
  clear();

  QDomDocument doc;
  QFile file(path);

  if (!file.open(QFile::ReadOnly)) {
    return false; //error
  }

  doc.setContent(&file);

  // xml in memory, can release file
  file.close();

  QDomElement root = doc.documentElement();
  if (0 != QString::compare(root.tagName(), "amf", Qt::CaseInsensitive)) {
    return false; //error
  }

  // Get all the object nodes
  QDomNodeList objectNodes = root.elementsByTagName("Object");

  for (unsigned int i = 0; i < objectNodes.length(); i++) {
    AMFObject* newAmfObj = new AMFObject();
    objects.append(newAmfObj);
    QDomElement objectElement = objectNodes.at(i).toElement();
    newAmfObj->ReadFromXml(objectElement);
  }

  return true;
}

bool AMFFile::writeToFile(QString path) {
  QDomDocument doc;
  QDomElement root = doc.createElement("AMF");
  doc.appendChild(root);

  // Write each object to the xml
  for (int i = 0; i < objects.size(); i++) {
    QDomElement obj = doc.createElement("Object");
    objects.at(i)->WriteXml(obj);
    root.appendChild(obj);
  }

  QFile file(path);
  QTextStream outStream(&file);

  if (!file.open(QFile::WriteOnly))
    return false; //error

  outStream << "<!DOCTYPE AMF>\n";
  outStream << doc.toString(3);

  file.close();

  return true;
}

const QList<AMFObject*>& AMFFile::GetObjects() const {
  return objects;
}

void AMFFile::addObject(AMFObject* new_object) {
  objects.push_back(new_object);
}

void AMFFile::clear() {
  // Delete all of the objects
  for (int i = 0; i < objects.size(); i++) {
    delete(objects[i]);
  }
  objects.clear();
}

