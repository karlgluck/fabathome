/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009, Karl Gluck (kwg8@cornell.edu)
 
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
#include "slice.h"
#include "shared/fabathome-constants.h"


typedef QVector<Slice> SliceArray;

/**
 * Stores the output of the slicer, which is a set of one or more outer and
 * zero or more inner boundaries of one or more materials intersecting planes
 * parallel to the XY axis and at interval Z heights.
 */
class SliceStack {
public:
  SliceStack();
  SliceStack(const SliceStack& other);
  SliceStack(SliceArray slices);
  void addSlice(Slice slice);

  /**
   * Erases all members of the slice stack
   */
  void clear();

  // Returns all of the slices within this stack
  const SliceArray& getSlices() const;
  
  // Writes slice information into an XML file so it can be
  // examined by a separate program.  If this operation fails,
  // the method returns 'false' but does not modify the value
  // returned by getLastError.
  //
  // Format:
  //  for each Slice in getSlices():    <Slice z=""> ... </Slice>
  //  for each Material in a slice:     <Material id=""> ... </Material>
  //  for each Region in a material:    <Region> ... </Region>
  //  for each point in a region's boundary:  <Vertex seq="(index)" x="" y="" z="" />
  bool writeToXMLFile(const char* file);

  /**
   * Outputs a file that contains edge information recognized by the slice-stack
   * visualizer.
   * @param file_name Output file
   * @param source_triangles Optional pointer to the list of triangles in the source mesh
   */
  bool writeToVisualizerFile(const QString& file_name,
                             const QList<FAHTriangle>* source_triangles) const;

private:

  SliceArray slices_;
};

