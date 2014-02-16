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

#include <QString>

class QDomNode;
class QScriptEngine;

/**
 * Represents a setting from the tool-script that can be displayed to the user
 * in the print dialog and passed into the script itself as a value after
 * adjustment by the user.
 */
class ToolScriptSetting {
public:

  /**
   * Fills in the script's values from the DOM node that represents a setting
   * configuration in the script file.
   */
  ToolScriptSetting(const QDomNode& sourceDomNode);

  /**
   * Returns the string that is a nice, easy-to-understand short description of
   * what this setting does.  e.g. "Path Width"
   */
  const QString& name() const;

  /**
   * Gets the string by which this setting should be referenced in code and
   * when executing the tool-script itself. e.g. "pathWidth"
   */
  const QString& scriptVariable() const;

  /**
   * Obtains the user-friendly name of this setting
   */
  const QString& text() const;

  /**
   * Returns the units to display alongside this setting
   */
  const QString& units() const;

  /**
   * Gets the setting's current numeric value
   */
  double value() const;

  /**
   * Obtains the textual representation of this setting.
   * @param append_units if true, the setting's units will appear after
   *                     a space at the end of the setting's text
   */
  QString valueString(bool append_units) const;

  /**
   * Changes the value in this node
   */
  void set(double value);

private:
  QString name_, script_variable_, text_, units_;
  double value_;
};

