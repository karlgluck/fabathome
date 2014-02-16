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
#include <QtScript>

class ToolScriptInterface : public QObject {
  Q_OBJECT

public:

  ToolScriptInterface(QScriptEngine& engine);

  /**
   * Passes an error exception to the script interface.  This is just a wrapper for
   * the engine's context's method.
   */
  QScriptValue throwError(const QString& error);

  /**
   * Returns a pointer to the object represented by 'value' if its C++ class type is
   * the name indicated by 'type'.  If the value is not an object or the
   * type isn't correct, this method returns null.
   */
  QObject* castTo(QScriptValue value, const QString& type);

  /**
   * Obtains the engine used by this interface.  This engine is the one currently
   * running the script that holds this object.  This method is used when constructing
   * new TSIs to be returned from TSI methods (e.g. SlicerTSI::doPathing)
   */
  QScriptEngine& engine();

  /**
   * Wraps this method of the internal engine, but fills in the parameter designating
   * that the object is script-owned automatically
   */
  QScriptValue newQObject(QObject* object);

private:
  QScriptEngine& engine_;
};
