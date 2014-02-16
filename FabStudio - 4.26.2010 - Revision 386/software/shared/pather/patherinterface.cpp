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
#include "patherinterface.h"
#include <common/confirm.h>
#include "simplecrosshatchpather.h"
#include "doubleedgepather.h"
#include "edgeonlypather.h"
#include "concentricloopspather.h"



//----[  allPatherImplementations  ]-------------------------------------------
QStringList PatherInterface::allPatherImplementations() {
  QStringList pather_impls;

  int i = 0;
  PatherImplementation impl = getImplementation(0);
  while (impl != kInvalidPatherImplementation) {
    pather_impls.push_back(getPatherName(impl));
    impl = getImplementation(++i);
  }

  return pather_impls;
}

//----[  getPatherName  ]------------------------------------------------------
QString PatherInterface::getPatherName(PatherImplementation implementation) {
  switch (implementation) {
  case kSimpleCrosshatchPather: return "Simple Crosshatch Pather";
  case kDoubleEdgePather:       return "Double Edge Pather";
  case kEdgeOnlyDebugPather:    return "Edge-Only Pather";
  case kConcentricLoopsPather:  return "Concentric Loops Pather (Theoretical; Don't Use)";
  default:
  case kInvalidPatherImplementation:  return "(Invalid Pather)";
  }
}

//----[  getImplementation  ]--------------------------------------------------
PatherImplementation PatherInterface::getImplementation(
    int implementation_index) {
  static const PatherImplementation IMPLEMENTATIONS[] = {
    kSimpleCrosshatchPather,
    kDoubleEdgePather,
    kEdgeOnlyDebugPather,
    kConcentricLoopsPather,
  };
  static const int NUMBER_OF_IMPLEMENTATIONS
      = sizeof(IMPLEMENTATIONS) / sizeof(PatherImplementation);
  if (implementation_index < 0 ||
      implementation_index >= NUMBER_OF_IMPLEMENTATIONS) {
    return kInvalidPatherImplementation;
  } else {
    return IMPLEMENTATIONS[implementation_index];
  }
}



//----[  PatherInterface  ]----------------------------------------------------
PatherInterface::PatherInterface(PatherImplementation impl) {
  pather_ = makePather(impl);
}





//----[  ~PatherInterface  ]---------------------------------------------------
PatherInterface::~PatherInterface() {
  delete pather_;
}




//----[  makePather  ]---------------------------------------------------------
Pather* PatherInterface::makePather(PatherImplementation impl) {
  Pather* pather;

  switch (impl) {
  case kSimpleCrosshatchPather:
    pather = new SimpleCrossHatchPather();
    break;
  case kDoubleEdgePather:
    pather = new DoubleEdgePather();
    break;
  case kEdgeOnlyDebugPather:
    pather = new EdgeOnlyPather();
    break;
    case kConcentricLoopsPather:
  pather = new ConcentricLoopsPather();
    break;
  default:
    return NULL;
  }

  return pather;
}




//----[  set  ]----------------------------------------------------------------
void PatherInterface::set(const QString& name, float value) {
  confirm(pather_) then {
    pather_->getSettings().set(name,value);
  }
}



//----[  set  ]----------------------------------------------------------------
void PatherInterface::set(const QString& name, int value) {
  confirm(pather_) then {
    pather_->getSettings().set(name,value);
  }
}



//----[  set  ]----------------------------------------------------------------
void PatherInterface::set(const QString& name, bool value)  {
  confirm(pather_) then {
    pather_->getSettings().set(name,value);
  }
}



//----[  set  ]----------------------------------------------------------------
void PatherInterface::set(const QString& name, const QString& value) {
  confirm(pather_) then {
    pather_->getSettings().set(name,value);
  }
}




//----[  doPathing  ]----------------------------------------------------------
bool PatherInterface::doPathing(const SliceStack &slices,
                                PatherProgressCallback *callback) {
  if (pather_ == NULL) {
    return false;
  } else {
    return pather_->doPathing(slices, callback);
  }
}



//----[  getLastErrorString  ]-------------------------------------------------
QString PatherInterface::getLastErrorString() const {
  if (pather_ == NULL) {
    return "";
  } else {
    return pather_->getLastErrorString();
  }
}

const PathStack& PatherInterface::getPathStack() const {
  confirm(pather_ != NULL);
  return pather_->getPathStack();
}
