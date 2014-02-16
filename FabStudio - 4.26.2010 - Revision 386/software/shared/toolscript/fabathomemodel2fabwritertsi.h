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
#include <QtScript>
#include "toolscriptinterface.h"
#include <QDomDocument>

class AMFRegionTSI;

/**
 * Prepares and writes out mesh data into the FabFile
 */
class FabAtHomeModel2FabWriterTSI : public ToolScriptInterface {
  Q_OBJECT

public:

  typedef QMap<QString,QString> MaterialProperties;
  typedef QList<MaterialProperties*> MaterialsList;

  typedef QPair<MaterialProperties*,AMFRegionTSI*> PrintableRegionPair;
  typedef QList<PrintableRegionPair> PrintableRegions;

public:
  FabAtHomeModel2FabWriterTSI(QScriptEngine& engine, QDomDocument& fab_document);
  ~FabAtHomeModel2FabWriterTSI();

public slots:

  void addMeshes(QString material_name,
                 QScriptValue material_calibration,
                 QScriptValue meshes_array);

  /**
   * Arranges the slices of the meshes in this writer from lowest Z
   * to highest Z.
   * Sorting performed on print.
   */
  void sortBottomUp();

  /**
   * Arranges the slices of the meshes in this writer from highest Z
   * to lowest Z.
   * Sorting performed on print.
   */
  void sortTopDown();

  /**
   * Changes the value of the print acceleration, millimeters/(second^2)
   */
  void setPrintAcceleration(int acceleration);

  /**
   * Writes the contents of the writer to the output file.  Any changes to the
   * writer after this method is called will not be applied.
   */
  void print();


private:

  MaterialsList materials_list_;
  PrintableRegions regions_;

  /// Output document where elements are written
  QDomDocument& fab_document_;

  /// Acceleration in millimeters/(second^2) that is used while printing
  int print_acceleration_;

  /// Whether to arrange the slices from lowest Z to highest Z or the reverse
  bool print_bottom_up_;
};


