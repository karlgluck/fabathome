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

#include <QThread>
#include <QMap>
#include <QColor>

#include "shared/pather/patherimplementation.h"

class MainWindow;
class ToolScript;
class Path;
class RenderablePath;

/**
 * This thread is used by the print panel to execute the tool-script on the
 * scene without locking up the GUI.
 */
class PrintThread : public QThread {
  Q_OBJECT

public:
  /**
   * @param main_window The primary application window (source of objects)
   * @param tool_script Script with which to print this scene
   * @param pather_implementation Choice of pather to use
   * @param generate_support_material Whether or not to automatically generate
   *                                  supporting material, should the script
   *                                  have a tool for this purpose available
   */
  PrintThread(MainWindow* main_window,
              const ToolScript* tool_script,
              PatherImplementation pather_implementation,
              bool generate_support_material);
  ~PrintThread();

  virtual void run();

private:
  MainWindow* main_window_;
  const ToolScript* tool_script_;
  PatherImplementation pather_implementation_;
  bool generate_support_material_;
  QMap<int,QColor> internal_material_id_to_color_;

private slots:
  void emitSetMasterRange(int a, int b);
  void emitSetMasterProgress(int a);
  void emitLogEntry(const QString& text);
  void emitSetTaskRange(int a, int b);
  void emitSetTaskProgress(int a);
  void emitSetTaskTitle(const QString& text);
  void emitAddRenderablePath(int material_id, Path* path);
  void emitSetProcessingPlane(float z);

signals:
  void setMasterRange(int,int);
  void setMasterProgress(int);
  void setTaskRange(int,int);
  void setTaskProgress(int);
  void setTaskTitle(QString);
  void logEntry(QString);
  void finishedFabFile(QString);
  void addRenderablePath(QColor color, Path* path);

  /**
   * Notifies the listener what slice is being computed; this is just to give
   * some visual feedback.
   * @param Z
   */
  void setProcessingPlane(float);
};
