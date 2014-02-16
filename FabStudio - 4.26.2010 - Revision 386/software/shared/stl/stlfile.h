/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009, Sean Cretella (sac76@cornell.edu)
                     Nathan Lloyd (nsl6@cornell.edu)
               2010, Jimmy Liu (jl2222@cornell.edu)
                     Karina Sobhani (ks598@cornell.edu)

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

#include <QString>
#include <QImage>
#include <qfile.h>
#include "STLMesh.h"

class AMFFile;

class STLFile {
public:

  /**
   * Detects an STL file as either binary or ASCII and reads it in.
   * Returns whether the file was successfully loaded.
   */
  bool read(QString path);

  /**
   * Reads in an STL file with a specified path, validates it, and
   * stores its data as a reference to a list of facet pointers.
   */
  bool readASCII(QString path);

  /**
   * Reads in an binary STL file with a specified path, validates it, and
   * stores its data as a reference to a list of facet pointers.
   */
  bool readBinary(QString path);
	
  /**
   * Writes facet data from a mesh into an STL file
   * with the specified output path.
   */
  bool WriteASCII(QString path);

  /**
   * Writes facet data from a BMP file, outputing a mesh outlining the
   * edges of the input file
   */
  bool ReadBMP(QString path);

  /**
   * Gets the mesh of this STL file
   */
  const STLMesh& GetMesh();

  /**
   * Translates this STL into an AMF model
   */
  void convertToAMF(AMFFile* output);

private:

  /**
   * Traverses the path specified by (dx,dy,0) starting at point
   * (x,y,z) in the image Im, and writes points A, B, C, and D.
   */
  void traversePath(FAHVector3* A, FAHVector3* B, FAHVector3* C, FAHVector3* D,
                    const QImage* Im, bool** visited, const int& x,
                    const int& y, const int& z, const int dx, const int dy,
                    const bool out);

  STLMesh mesh_;
};
