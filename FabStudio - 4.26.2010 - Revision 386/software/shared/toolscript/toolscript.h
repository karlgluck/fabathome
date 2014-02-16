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

class ToolScriptTool;
class ToolScriptSetting;
class AMFFileTSI;
class FabFileTSI;
class SlicerProgressCallback;
class PatherProgressCallback;
class AMFRegion;
class ProgressTSI;


#include "shared/pather/patherinterface.h"

/**
 * This class is responsible for loading a tool-script definition file from the
 * disk, creating the tools/settings structures that the print dialog uses to
 * configure the print and finally executing the script on the source model.
 */
class ToolScript {
public:
  enum Error {
    kNoError,
    kFileNotFound,
    kInvalidRootElement,
    kInvalidPrintScriptNode,
  };

public:

  ~ToolScript();

  /**
   * Processes the source file for information about the tool-script.  After this
   * method returns 'true', don't call load() again on the same object.
   * This method sets the last-error value.
   */
  bool load(const QString& file);

  /**
   * Returns the result code of the last operation that set the last-error value.
   */
  Error getLastError();

  /**
   * Obtains the string representation of a given result code
   */
  static QString getErrorString(Error error);

  /**
   * Combines the getLastError/getErrorString methods to return a textual version
   * of the last result code.  This is useful for displaying status messages to
   * the user.
   */
  QString getLastErrorString();

  /**
   * Returns the name of the printer for which this script processes objects
   */
  QString printerName() const;


  /**
   * Obtains all of the global settings that were found in the tool script.
   * Usually the slice-height is a global setting.  This list should be displayed
   * to the user in the print dialog's "advanced" section so that they can
   * tweak the contained values.
   */
  const QList<ToolScriptSetting*>& generalSettings() const;

  /**
   * Returns a list of all of the different tools that can be assigned to process
   * materials in the model to be printed.  The user should be able to assign
   * materials using the print dialog, and edit the properties of each of the tools
   * in the advanced tab.  If there is only one tool, all materials in the source
   * model should be automatically assigned to that single tool and the option to
   * assign materials at all should be disabled.
   */
  const QList<ToolScriptTool*>& tools() const;

  /**
   * Executes the tool-script to write out the fab file from the source AMF
   * @param amf Source model
   * @param fab Output for xml file
   */
  void run(AMFFileTSI& amf,
           SlicerProgressCallback* slicer_progress_callback,
           PatherImplementation pather,
           PatherProgressCallback* pather_progress_callback,
           QString output_fab_file_name);

  /**
   * Takes an input map of material names to regions and translates it into an
   * output map of tool names to regions
   * @param map in:  <material, region> pairs
   *            out: <tool name, region> pairs
   */
  void remapMaterialKeysToToolKeys(QMultiMap<QString,AMFRegion*>* map) const;

  /**
   * Obtains the name of the tool that should be used to support objects in the
   * build process.  If no tool is configured to generate support material, this
   * method returns 'false'
   * @param tool_name
   */
  bool getSupportMaterialTool(QString* tool_name) const;

  /**
   * Executes the tool-script to write out the fab file from the source set of objects
   * @param meshes List of all of the meshes that are to be printed paired with the name
   *               of the tool that should be used to print it.
   * @param fab Output for xml file
   */
  bool run(const QMultiMap<QString,AMFRegion*>& meshes,
           SlicerProgressCallback* slicer_progress_callback,
           PatherImplementation pather,
           PatherProgressCallback* pather_progress_callback,
           ProgressTSI* progress_callback, // has signals emitted by the script
           QString* fab_file) const;

  /**
   * Retrieves the text of the script that performs the conversion of an object to
   * a fab file.
   */
  const QString& printScript() const;

  /**
   *
   */
  const QString& name() const;

  /**
   *
   */
  const QString& description() const;


private:

  /**
   * Returns false if the error code is a failure code,
   * or true if it is a success code.
   */
  bool setLastError(Error error);

  /**
   * Frees memory and zeroes out internal elements
   */
  void reset();

private:

  /// Name and description this tool-script configuration
  QString name_, description_, printer_name_;

  /// The actual Qt-script that drives printing.  Loaded from a CDATA section
  /// in the source file.
  QString print_script_;

  /// Global settings
  QList<ToolScriptSetting*> settings_;

  /// List of all tools configured for this script
  QList<ToolScriptTool*> tools_;

  /// Last error value
  Error last_error_;
};
