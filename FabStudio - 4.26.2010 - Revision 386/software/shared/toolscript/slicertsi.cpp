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
#include "slicertsi.h"
#include "shared/slicer/slicer.h"
#include "amffiletsi.h"
#include "slicestacktsi.h"
#include "amfregiontsi.h"

SlicerTSI::SlicerTSI(QScriptEngine& engine, SlicerProgressCallback* callback)
    : ToolScriptInterface(engine),
      slicer_(new Slicer()),
      callback_(callback) {
}
//
//SliceStack* SlicerTSI::doSlicing(AMFRegion* region) {
//  if (!slicer_->doSlicing(region, callback_)) {
//    throwError("Slicer failed: " + slicer_->getLastErrorString());
//    return 0;
//  }
//  return new SliceStack(slicer_->getSliceStack());
//}


void SlicerTSI::setSliceHeight(double v) {
  //jlz27 - Does not work in Qt 4.6.2
    /*if (!_finite(v) || _isnan(v)) {
    throwError("Invalid slice height");
  }*/

  slicer_->setSliceHeight((FAHFloat)v);
}

void SlicerTSI::alignTray() {
  slicer_->setAlignment(Slicer::kAlignTray);
}

void SlicerTSI::alignTop() {
  slicer_->setAlignment(Slicer::kAlignTop);
}

void SlicerTSI::alignBottom() {
  slicer_->setAlignment(Slicer::kAlignBottom);
}

void SlicerTSI::alignCenter() {
  slicer_->setAlignment(Slicer::kAlignCenter);
}

void SlicerTSI::doSlicing(QScriptValue amf_region) {
  AMFRegionTSI* argument = (AMFRegionTSI*)castTo(amf_region, "AMFRegionTSI");

  if (!argument) {
    throwError("Invalid object passed to slicer's doSlicing()");
    return;
  }

  if (!slicer_->doSlicing(argument->region(), callback_)) {
    throwError("Unable to perform slicing:  " + slicer_->getLastErrorString());
    return;
  }

  argument->setSlices(new SliceStack(slicer_->getSliceStack()));
//
//  AMFFileTSI* argument = (AMFFileTSI*)castTo(amf_object, "AMFFileTSI");
//
//  if (!argument) {
//    return throwError("Invalid object passed to slicer's doSlicing()");
//  }
//
//  if (!slicer_->doSlicing(argument->amf(), callback_)) {
//    return throwError("Unable to perform slicing:  " + slicer_->getLastErrorString());
//  }
//
//  return newQObject(new SliceStackTSI(engine(), slicer_));
}

