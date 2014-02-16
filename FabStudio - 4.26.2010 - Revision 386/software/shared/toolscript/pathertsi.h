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

#include <QObject>
#include <QSharedPointer>
#include <QtScript>
#include "shared/slicer/slicer.h"
#include "toolscriptinterface.h"
#include "shared/pather/patherinterface.h"

#include "slicestacktsi.h"


/**
 * Script-file interface object for the pather.  Slots in this class can
 * be called from a tool-script by invoking them on the "pather" object
 */
class PatherTSI : public ToolScriptInterface {
  Q_OBJECT

public:
  PatherTSI(QScriptEngine& engine,
            PatherImplementation implementation,
            PatherProgressCallback* callback);

public slots:

  /**
   * Returns a PathStackTSI to the calling script, or throws an error in the
   * script's engine if something went wrong.  The incoming object must be
   * an AMFRegionTSI that has been sliced.
   */
  void doPathing(QScriptValue amf_region);

  /**
   * These methods are used to change parameter values used by the pather
   */
  void set(const QString& name, float value);
//  void set(const QString& name, int value);
//  void set(const QString& name, bool value);
//  void set(const QString& name, const QString& value);


private:
  PatherInterface pather_;
  PatherProgressCallback* callback_;
};
