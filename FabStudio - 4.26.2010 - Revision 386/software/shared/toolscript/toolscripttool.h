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
#include <QList>

class ToolScriptSetting;

/**
 *
 */
class ToolScriptTool {
public:

  /**
   * Sets up this tool with the given name.  The name is what is displayed
   * in the print dialog.
   */
  ToolScriptTool(const QString& tool_name,
                 const QString& material_name,
                 bool is_support_material,
                 const QString& script_variable);

  /**
   * Frees memory used by the tool-script settings
   */
  ~ToolScriptTool();

  /**
   * When loading a tool-script, the parsing class uses this to add
   * settings discovered in the file to this tool.  The setting should be
   * allocated on the heap using 'new'.  It will be automatically deleted
   * with this class.
   */
  void addSetting(ToolScriptSetting* setting);

  /**
   * Obtains the list of all settings that can be configured for this tool
   */
  const QList<ToolScriptSetting*>& settings() const;

  /**
   * Gets the name of the tool
   */
  QString name() const;

  /**
   * Gets the name of the material from which this tool creates objects
   */
  QString materialName() const;

  /**
   * Returns true if this tool prints support material, or false if it
   * prints model material.
   */
  bool isSupportMaterial() const;

  /**
   * Obtains the variable name the toolscript wants to use for referencing
   * this variable in the script's execution.
   */
  QString scriptVariable() const;


private:
    QString name_, material_name_, script_variable_;
    bool is_support_material_;
    QList<ToolScriptSetting*> settings_;
};
