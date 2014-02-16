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
#include "positionobjectspanel.h"
#include "ui_positionobjectspanel.h"
#include "mainwindow.h"
#include "printableobject.h"
#include <common/confirm.h>

PositionObjectsPanel::PositionObjectsPanel(MainWindow* main_window, QWidget *parent) :
    QDockWidget(parent),
    m_ui(new Ui::PositionObjectsPanel),
    main_window_(main_window),
    selected_object_(0),
    manipulating_(false) {
  m_ui->setupUi(this);
  m_ui->selectionTabs->setEnabled(false);
  m_ui->deletingObjectLabel->hide();
}

PositionObjectsPanel::~PositionObjectsPanel() {
  delete m_ui;
}


void PositionObjectsPanel::useAdvancedMode(bool enable) {
  m_ui->putOnTray->setVisible(enable);
  m_ui->positionGrid->setVisible(enable);
  m_ui->rotationGrid->setVisible(enable);
  m_ui->scalingGrid->setVisible(enable);
  m_ui->uniformScalingCheckbox->setVisible(enable);
}

void PositionObjectsPanel::setSelectedObject(PrintableObject* object) {
  m_ui->selectionTabs->setEnabled(object != 0);
  m_ui->deleteObject->setEnabled(object != 0);
  m_ui->selectionTabs->setVisible(true);
  m_ui->deleteObject->setVisible(true);
  manipulatedObject(object);
  selected_object_ = object;
}

void PositionObjectsPanel::manipulatedObject(PrintableObject* object) {
  FAHVector3 t, r, s;
  if (object) {
    t.set(object->translation());
    r.set(object->rotation());
    s.set(object->scaling());
  } else {
    t.zero();
    r.zero();
    s.zero();
  }
  manipulating_ = true;
  m_ui->positionX->setValue(t.x);
  m_ui->positionY->setValue(t.y);
  m_ui->positionZ->setValue(t.z);
#define toDeg(x) (x/Math::k2Pi)*360.0
  m_ui->rotationX->setValue(toDeg(r.x));
  m_ui->rotationY->setValue(toDeg(r.y));
  m_ui->rotationZ->setValue(toDeg(r.z));
  m_ui->scalingX->setValue(s.x*100);
  m_ui->scalingY->setValue(s.y*100);
  m_ui->scalingZ->setValue(s.z*100);
  manipulating_ = false;
}

void PositionObjectsPanel::showScaling() {
  m_ui->selectionTabs->setCurrentWidget(m_ui->scalingTab);
}

void PositionObjectsPanel::showRotation() {
  m_ui->selectionTabs->setCurrentWidget(m_ui->rotationTab);
}

void PositionObjectsPanel::showTranslation() {
  m_ui->selectionTabs->setCurrentWidget(m_ui->positionTab);
}

void PositionObjectsPanel::showDeletingObjectLabel(bool show) {
  m_ui->deletingObjectLabel->setVisible(show);
  m_ui->selectionTabs->setVisible(!show);
  m_ui->deleteObject->setVisible(!show);
}

void PositionObjectsPanel::changeEvent(QEvent *e) {
  QDockWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
      m_ui->retranslateUi(this);
      break;
  default:
      break;
  }
}

void PositionObjectsPanel::on_autoArrange_clicked() {
  main_window_->arrangeBuildTray();
}

void PositionObjectsPanel::on_putOnTray_clicked() {
  confirm(selected_object_) then {
    selected_object_->snapToRestOnXYPlane();
    manipulatedObject(selected_object_);
    main_window_->glRepaint();
  }
}

void PositionObjectsPanel::on_scaleFromInches_clicked() {
  confirm(selected_object_) then {
    // This is because most syringes are 10cc, and most models are ~30% material
    // 30 = cuberoot(1/.3 * 8 cubic centimeters of volume)
    selected_object_->setUniformScale(24.5);
    if (!main_window_->isInAdvancedMode()) selected_object_->snapToRestOnXYPlane();
    manipulatedObject(selected_object_);
    main_window_->glRepaint();
  }
}

void PositionObjectsPanel::on_pickReasonableSize_clicked() {
  confirm(selected_object_) then {
    // This is because most syringes are 10cc, and most models are ~30% material
    // 30 = cuberoot(1/.3 * 8 cubic centimeters of volume)
    selected_object_->setUnitScale(30);
    if (!main_window_->isInAdvancedMode()) selected_object_->snapToRestOnXYPlane();
    manipulatedObject(selected_object_);
    main_window_->glRepaint();
  }
}

void PositionObjectsPanel::on_deleteObject_clicked() {
  confirm(selected_object_) then {
    main_window_->removeObject(selected_object_); // will call setSelectedObject(0)
  }
}

void PositionObjectsPanel::on_findBestRotation_clicked() {
  confirm(selected_object_) then {
    selected_object_->setBestOrientation();
    if (!main_window_->isInAdvancedMode()) selected_object_->snapToRestOnXYPlane();
    manipulatedObject(selected_object_);
    main_window_->glRepaint();
  }
}

void PositionObjectsPanel::on_positionX_valueChanged(double x) {
  if (manipulating_) return;
  confirm (selected_object_) then {
    selected_object_->translateAbsolute(FAHVector3::make(x,
                                                         m_ui->positionY->value(),
                                                         m_ui->positionZ->value()));
    main_window_->glRepaint();
  }
}

void PositionObjectsPanel::on_positionY_valueChanged(double y) {
  if (manipulating_) return;
  confirm (selected_object_) then {
    selected_object_->translateAbsolute(FAHVector3::make(m_ui->positionX->value(),
                                                         y,
                                                         m_ui->positionZ->value()));
    main_window_->glRepaint();
  }
}

void PositionObjectsPanel::on_positionZ_valueChanged(double z) {
  if (manipulating_) return;
  confirm (selected_object_) then {
    selected_object_->translateAbsolute(FAHVector3::make(m_ui->positionX->value(),
                                                         m_ui->positionY->value(),
                                                         z));
    main_window_->glRepaint();
  }
}

void PositionObjectsPanel::on_rotationX_valueChanged(double x) {
  if (manipulating_) return;
  confirm (selected_object_) then {
    selected_object_->rotateEuler(FAHVector3::make(Math::toRadians(x),
                                                   Math::toRadians(m_ui->rotationY->value()),
                                                   Math::toRadians(m_ui->rotationZ->value())));
    main_window_->glRepaint();
  }
}

void PositionObjectsPanel::on_rotationY_valueChanged(double y) {
  if (manipulating_) return;
  confirm (selected_object_) then {
    selected_object_->rotateEuler(FAHVector3::make(Math::toRadians(m_ui->rotationX->value()),
                                                   Math::toRadians(y),
                                                   Math::toRadians(m_ui->rotationZ->value())));
    main_window_->glRepaint();
  }

}

void PositionObjectsPanel::on_rotationZ_valueChanged(double z) {
  if (manipulating_) return;
  confirm (selected_object_) then {
    selected_object_->rotateEuler(FAHVector3::make(Math::toRadians(m_ui->rotationX->value()),
                                                   Math::toRadians(m_ui->rotationY->value()),
                                                   Math::toRadians(z)));
    main_window_->glRepaint();
  }
}

void PositionObjectsPanel::on_scalingX_valueChanged(double x) {
  if (manipulating_) return;
  if (m_ui->uniformScalingCheckbox->isChecked()) {
    manipulating_ = true;
    m_ui->scalingY->setValue(x);
    m_ui->scalingZ->setValue(x);
    manipulating_ = false;
  }
  confirm (selected_object_) then {
    selected_object_->scale(FAHVector3::make(x/100.0,
                                             m_ui->scalingY->value()/100.0,
                                             m_ui->scalingZ->value()/100.0));
    main_window_->glRepaint();
  }
}

void PositionObjectsPanel::on_scalingY_valueChanged(double y) {
  if (manipulating_) return;
  if (m_ui->uniformScalingCheckbox->isChecked()) {
    manipulating_ = true;
    m_ui->scalingX->setValue(y);
    m_ui->scalingZ->setValue(y);
    manipulating_ = false;
  }
  confirm (selected_object_) then {
    selected_object_->scale(FAHVector3::make(m_ui->scalingX->value()/100.0,
                                             y/100.0,
                                             m_ui->scalingZ->value()/100.0));
    main_window_->glRepaint();
  }
}

void PositionObjectsPanel::on_scalingZ_valueChanged(double z) {
  if (manipulating_) return;
  if (m_ui->uniformScalingCheckbox->isChecked()) {
    manipulating_ = true;
    m_ui->scalingX->setValue(z);
    m_ui->scalingY->setValue(z);
    manipulating_ = false;
  }
  confirm (selected_object_) then {
    selected_object_->scale(FAHVector3::make(m_ui->scalingX->value()/100.0,
                                             m_ui->scalingY->value()/100.0,
                                             z/100.0));
    main_window_->glRepaint();
  }
}

void PositionObjectsPanel::on_uniformScalingCheckbox_toggled(bool checked) {
  if (checked) {
    manipulating_ = true;
    Math::Float s = m_ui->scalingX->value();
    m_ui->scalingX->setValue(s);
    m_ui->scalingY->setValue(s);
    m_ui->scalingZ->setValue(s);
    manipulating_ = false;
  }
  confirm (selected_object_) then {
    selected_object_->scale(FAHVector3::make(m_ui->scalingX->value()/100.0,
                                             m_ui->scalingY->value()/100.0,
                                             m_ui->scalingZ->value()/100.0));
    main_window_->glRepaint();
  }
}
