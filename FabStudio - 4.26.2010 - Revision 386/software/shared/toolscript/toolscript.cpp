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
#include "toolscript.h"

#include "toolscripttool.h"
#include "toolscriptsetting.h"

#include "amffiletsi.h"   // TODO 3/9/10:  ELIMINATE THIS, DEPRECATED
#include "amfregiontsi.h"
#include "slicertsi.h"
#include "pathertsi.h"
#include "fabwritertsi.h"
#include "progresstsi.h"


#include <QDomDocument>
#include <QFile>
#include <QTextStream>

#include <QtScript>
#include <QTextStream>
#include <QDebug>

#include "common/todo.h"


ToolScript::~ToolScript() {
  reset();
}


bool ToolScript::load(const QString& file) {
  reset();

  QDomDocument document;

  { // load the source file into the DOM document
    QFile scriptFile(file);
    if (!scriptFile.open(QFile::ReadOnly)) {
      return setLastError(kFileNotFound);
    }

    document.setContent(&scriptFile);
    scriptFile.close();
  }

  // pull out each of the tools from the element
  QDomElement root = document.documentElement();
  if (root.tagName() != "toolScript") {
    qDebug() << "No toolScript element " << file;
    return setLastError(kInvalidRootElement);
  }

  name_ = root.attributes().namedItem("name").nodeValue();
  description_ = root.attributes().namedItem("description").nodeValue();
  printer_name_ = root.attributes().namedItem("printer").nodeValue();

  {
    QDomNodeList print_nodes = root.elementsByTagName("printScript");
    if (print_nodes.size() != 1) {
      qDebug() << "No printScript node in " << file;
      return setLastError(kInvalidPrintScriptNode);
    }
    print_nodes = print_nodes.at(0).childNodes();
    if (print_nodes.size() != 1 || !print_nodes.at(0).isCDATASection()) {
      qDebug() << "Invalid printScript node in " << file;
      return setLastError(kInvalidPrintScriptNode);
    }
    print_script_ = print_nodes.at(0).nodeValue();
  }

  QDomNodeList settingsNodes = root.elementsByTagName("settings");
  for (unsigned int i = 0; i < settingsNodes.length(); ++i) {
    QDomNode node = settingsNodes.at(i);
    QDomNode parent = node.parentNode();
    if (parent.nodeName() == "tool") {
      QString tool_name = parent.attributes().namedItem("name").nodeValue();
      QString material_name = parent.attributes().namedItem("material").nodeValue();
      todo("kwg8", "check these names for validity; else, toolscript is not valid");
      QString script_variable = parent.attributes().namedItem("scriptVariable").nodeValue();
      bool is_support_material = false;
      { // Determine if this tool is flagged as support material
        QString is_support_material_s = parent.attributes().namedItem("isSupportMaterial").nodeValue();
        bool ok = false;
        is_support_material = 0 != is_support_material_s.toInt(&ok, 10);
        if (!ok) {
          if (is_support_material_s.isEmpty()) {
            is_support_material = false;
          } else {
            char first = is_support_material_s.toAscii().at(0);
            is_support_material = first == 'T' || first == 't';
          }
        }
      }
      ToolScriptTool* tool = new ToolScriptTool(tool_name, material_name, is_support_material, script_variable);
      QDomNodeList settingNodes = node.childNodes();
      for (int j = 0; j < settingNodes.size(); ++j) {
        QDomNode settingNode = settingNodes.at(j);
        if (settingNode.isComment()) continue; // discard all the weird stuff
        tool->addSetting(new ToolScriptSetting(settingNode));
      }
      tools_.append(tool);
    } else if (parent.nodeName() == "toolScript") {
      // contains custom non-tool-specific setting
      QDomNodeList settingNodes = node.childNodes();
      for (int j = 0; j < settingNodes.size(); ++j) {
        QDomNode settingNode = settingNodes.at(j);
        if (settingNodes.at(j).isComment()) continue;
        settings_.append(new ToolScriptSetting(settingNode));
      }
    } else {
      // there is a problem with the tool-script, but it can be ignored
    }
  }

  return setLastError(kNoError);
}


ToolScript::Error ToolScript::getLastError() {
  return last_error_;
}


QString ToolScript::getErrorString(Error error) {
  switch (error) {
  case kNoError: return "No error";
  case kFileNotFound: return "File not found";
  case kInvalidRootElement: return "Invalid root element";
  case kInvalidPrintScriptNode: return "Invalid printScript XML node";
  default: return "Unknown error code";
  }
}


QString ToolScript::getLastErrorString() {
  return getErrorString(last_error_);
}


QString ToolScript::printerName() const {
  return printer_name_;
}


const QList<ToolScriptSetting*>& ToolScript::generalSettings() const {
  return settings_;
}


const QList<ToolScriptTool*>& ToolScript::tools() const {
  return tools_;
}




void ToolScript::remapMaterialKeysToToolKeys(QMultiMap<QString,AMFRegion*>* map) const {
  QMultiMap<QString,AMFRegion*> remap;

  // Scrape all of the regions matching a given material name from the map
  foreach (ToolScriptTool* tool, tools_) {
    QList<AMFRegion*> regions = map->values(tool->materialName());
    foreach (AMFRegion* region, regions) {
      remap.insert(tool->name(), region);
    }
    map->remove(tool->materialName());
  }

  // Add the newly generated elements back to the output map
  map->clear();
  map->unite(remap);
}


bool ToolScript::getSupportMaterialTool(QString* tool_name) const {
  foreach (ToolScriptTool* tool, tools_) {
    if (tool->isSupportMaterial()) {
      tool_name->operator =(tool->name());
      return true;
    }
  }

  // No support material found
  return false;
}


bool ToolScript::run(const QMultiMap<QString,AMFRegion*>& meshes,
                     SlicerProgressCallback* slicer_progress_callback,
                     PatherImplementation pather,
                     PatherProgressCallback* pather_progress_callback,
                     ProgressTSI* progress_callback,
                     QString* fab_file) const {

  QScriptEngine engine;

  // Initialize the major tool-script interface objects
  SlicerTSI slicer_tsi(engine, slicer_progress_callback);
  PatherTSI pather_tsi(engine, pather, pather_progress_callback);
  FabWriterTSI fab_writer_tsi(engine);
  engine.globalObject().setProperty("slicer", engine.newQObject(&slicer_tsi));
  engine.globalObject().setProperty("pather", engine.newQObject(&pather_tsi));
  engine.globalObject().setProperty("fabFile", engine.newQObject(&fab_writer_tsi));
  engine.globalObject().setProperty("progress", engine.newQObject(progress_callback));

  // Generate initialization text that is prepended to the user's script before
  // execution.  This text exposes the tool settings via a variable.  This variable
  // also contains an array that references all of the mesh regions associated with
  // that tool.
  QList<AMFRegionTSI*> region_tsis;
  QString full_script = "";
  foreach (const ToolScriptSetting* setting, settings_) {
    QString setting_text;
    QTextStream(&setting_text) << "var " << setting->name() << " = " << setting->valueString(false) << ";\n";
    full_script.append(setting_text);
  }
  full_script.append("\n");
  foreach (const ToolScriptTool* tool, tools_) {

    QString tool_text;
    QTextStream(&tool_text) << "var " << tool->scriptVariable() << " = {";

    bool is_first = true;
    foreach (const ToolScriptSetting* setting, tool->settings()) {
      QString entry;
      QTextStream(&entry) << (is_first ? "" : ",")
                          << setting->scriptVariable()
                          << ":" << setting->valueString(false);
      tool_text.append(entry);
      is_first = false;
    }

    // Build the meshes member
    is_first = true;
    QList<AMFRegion*> assigned_regions = meshes.values(tool->name());
    QString meshes_array_string = ",\nmeshes:new Array(";
    foreach (AMFRegion* region, assigned_regions) {
      AMFRegionTSI* tsi = new AMFRegionTSI(region);
      region_tsis.append(tsi);
      QScriptValue tsi_script_object = engine.newQObject(tsi);
      engine.globalObject().setProperty(tsi->uniqueName(), tsi_script_object);
      QString entry;
      QTextStream(&entry) << (is_first ? "" : ",") << tsi->uniqueName();
      meshes_array_string += entry;
      is_first = false;
    }
    meshes_array_string += ")";

    // Add the meshes member to the tool
    tool_text += meshes_array_string;
    tool_text += "};\n\n";

    // Add this tool initialization to the initialization script
    full_script += tool_text;
  }

  // Append the tool-script's text to the full script
  full_script += "\n";
  full_script += print_script_;

  // GO!
  engine.evaluate(full_script, name_);
  if (engine.hasUncaughtException()) {
    progress_callback->log("FAILED!");
    progress_callback->log(engine.uncaughtException().toString());
    foreach (QString str, engine.uncaughtExceptionBacktrace()) {
      progress_callback->log(str);
    }
    progress_callback->log("Script Dump:");
    progress_callback->log(full_script);
    return false;
  }

  fab_writer_tsi.write(fab_file);

  return true;
}

const QString& ToolScript::printScript() const {
  return print_script_;
}


const QString& ToolScript::name() const {
  return name_;
}


const QString& ToolScript::description() const {
  return description_;
}



bool ToolScript::setLastError(Error error) {
  last_error_ = error;

  // kNoError is the only "success" error code, so all others should
  // return false from this method
  return error == kNoError;
}


void ToolScript::reset() {
  name_.clear();
  description_.clear();
  print_script_.clear();
  for (int i = 0; i < settings_.size(); ++i) {
    delete settings_.at(i);
  }
  settings_.clear();
  for (int i = 0; i < tools_.size(); ++i) {
    delete tools_.at(i);
  }
  tools_.clear();
}


