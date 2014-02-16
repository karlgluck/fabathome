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
#ifndef POSITIONOBJECTSPANEL_H
#define POSITIONOBJECTSPANEL_H

#include <QtGui/QDockWidget>
#include "shared/fabathome-constants.h"

namespace Ui {
    class PositionObjectsPanel;
}

class MainWindow;
class PrintableObject;

class PositionObjectsPanel : public QDockWidget {
  Q_OBJECT
public:
  PositionObjectsPanel(MainWindow* main_window, QWidget *parent = 0);
  ~PositionObjectsPanel();

  /**
   * Turns on or off all "advanced" features of the program.  This is used to turn FabStudio
   * from a simple, general and fail-proof tool into a fully-featured 3d printing program
   * or back again.
   */
  void useAdvancedMode(bool enable);

  void setSelectedObject(PrintableObject* object);

  void manipulatedObject(PrintableObject* object);

  void showScaling();
  void showRotation();
  void showTranslation();

  void showDeletingObjectLabel(bool show);

protected:
  void changeEvent(QEvent *e);

private:
  Ui::PositionObjectsPanel *m_ui;
  MainWindow* main_window_;
  PrintableObject* selected_object_;
  bool manipulating_; // ignore valueChanged slots when manipulating

private slots:
  void on_uniformScalingCheckbox_toggled(bool checked);
  void on_scalingZ_valueChanged(double );
  void on_scalingY_valueChanged(double );
  void on_scalingX_valueChanged(double );
  void on_rotationZ_valueChanged(double );
  void on_rotationY_valueChanged(double );
  void on_rotationX_valueChanged(double );
  void on_positionZ_valueChanged(double );
  void on_positionY_valueChanged(double );
  void on_positionX_valueChanged(double );
  void on_findBestRotation_clicked();
  void on_deleteObject_clicked();
  void on_pickReasonableSize_clicked();
  void on_scaleFromInches_clicked();
  void on_putOnTray_clicked();
  void on_autoArrange_clicked();
};

#endif // POSITIONOBJECTSPANEL_H
