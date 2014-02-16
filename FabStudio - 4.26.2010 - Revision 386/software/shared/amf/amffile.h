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
#pragma once

#include <QList>
#include <QString>


class AMFObject;

/**
 *
 */
class AMFFile {
public:

   /**
    * Default constructor
    * Creates an empty AMF file
    */
   AMFFile();

   /**
    * Creates a deep copy of the source data
    */
   explicit AMFFile(const AMFFile* copy_source);

   /**
    * Destructor
    * Frees all memory associated with the AMF file
    */
   ~AMFFile();

   /**
    * Sets the AMF file to have the values of the AMF file given by path
    * @param path the path to the AMF file to read
    * @return true if the file is successfully loaded, false otherwise
    * @see writeToFile()
    */
   bool readFromFile(QString path);

   /**
    * Writes this AMF file to the file given by path
    * @param path the path to write the AMF file to
    * @return true if the file is successfully written, false otherwise
    * @see readFromFile()
    */
   bool writeToFile(QString path);

   /**
    * Gets the AMFObjects that make up this AMF file
    * @return this AMF file's list of objects
    * @see addObject()
    */
   const QList<AMFObject*>& GetObjects() const;

   /**
    * Adds the object to this AMF file
    * @param the object to add
    * @see GetObjects()
    */
   void addObject(AMFObject* new_object);

   /**
    * Frees all memory associated with the AMF file and erases
    * all contents of the structure
    */
   void clear();

private:
   QList<AMFObject*> objects;
};
