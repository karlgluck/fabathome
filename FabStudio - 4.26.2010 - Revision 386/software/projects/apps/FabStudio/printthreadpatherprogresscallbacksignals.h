/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2010, Karl Gluck (kwg8@cornell.edu)

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

#include <QObject>
#include "shared/pather/patherprogresscallback.h"


class Path;
class PathSlice;


class PrintThreadPatherProgressCallbackSignals : public QObject, public PatherProgressCallback {
  Q_OBJECT
public:
  PrintThreadPatherProgressCallbackSignals();
  virtual ~PrintThreadPatherProgressCallbackSignals();
  virtual void setNumberOfSlices(int count);
  virtual void sliceHasBeenComputed(int index, const PathSlice* slice);
  virtual void encounteredIssue(QString description);
  virtual void finishedPathing(const PathStack& path_stack);
signals:
  void signalSetTaskRange(int a, int b);
  void signalSetTaskProgress(int v);
  void signalEncounteredIssue(QString);
  void signalSetTaskTitle(QString);

  /**
   * Emitted for each computed path whenever a slice has been computed.
   * Note: this class emits a <b>new</b> instance of of a path, so if there is no
   * receiver for this signal, it will leak memory!  However, using a QSharedPointer
   * here is a ginormous pain because you have to use Q_DECLARE_METATYPE for
   * it wherever it used, which requires Path to be a complete class declaration,
   * which pulls in all sorts of other headers and generally bloats the code.
   * For our purposes, we know that the signal is always connected to a slot in
   * the print thread, which passes the pointer through to the print panel, which
   * will always free the memory.
   */
  void signalAddRenderablePath(int material_id, Path* path);
};

