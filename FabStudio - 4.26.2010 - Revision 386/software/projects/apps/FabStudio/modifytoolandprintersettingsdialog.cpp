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
#include "modifytoolandprintersettingsdialog.h"
#include "ui_modifytoolandprintersettingsdialog.h"

#include "shared/slicer/slicer.h"
#include "shared/toolscript/toolscript.h"
#include "shared/toolscript/toolscriptsetting.h"
#include "shared/toolscript/toolscripttool.h"
#include "shared/pather/pather.h"
#include "shared/pather/patherinterface.h"

ModifyToolAndPrinterSettingsDialog::ModifyToolAndPrinterSettingsDialog(ToolScript* tool_script,
                                                                       bool generate_support_material,
                                                                       PatherImplementation pather_implementation,
                                                                       QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ModifyToolAndPrinterSettingsDialog),
    tool_script_(tool_script)
{
  m_ui->setupUi(this);

  m_ui->generateSupportMaterial->setChecked(generate_support_material);

  m_ui->parametersTable->headerItem()->setText(0, "Setting");
  m_ui->parametersTable->headerItem()->setText(1, "Value");
  m_ui->parametersTable->headerItem()->setText(2, "Units");

//  // add all of the slicer alignments to the list
//  foreach (QString s, Slicer::alignmentStrings()) {
//    m_ui->slicerAlignment->addItem(s);
//  }

  // add all of the pathers to the pather options list
  int pather_index = 0;
  QStringList implementations = PatherInterface::allPatherImplementations();
  for (int i = 0; i < implementations.size(); ++i) {
    m_ui->patherCombo->addItem(implementations.at(i));
    if (PatherInterface::getImplementation(i) == pather_implementation) {
      pather_index = i;
    }
  }
  m_ui->patherCombo->setCurrentIndex(pather_index);

  // Let the user view how this tool is described
  m_ui->basicDescription->setPlainText(tool_script_->description());

  { // Add general settings to the tree
    QTreeWidgetItem* general_settings_item
        = new QTreeWidgetItem();
    general_settings_item->setText(0, "General");
    m_ui->parametersTable->addTopLevelItem(general_settings_item);
    foreach (ToolScriptSetting* setting, tool_script_->generalSettings()) {
      QTreeWidgetItem* child = new QTreeWidgetItem();
      child->setText(0, setting->name());
      child->setText(1, setting->valueString(false));
      child->setText(2, setting->units());
      general_settings_item->addChild(child);
    }
  }

  // Add all tools
  foreach (ToolScriptTool* tool, tool_script_->tools()) {
    QTreeWidgetItem* tool_item = new QTreeWidgetItem();
    tool_item->setText(0, tool->name());
    { // Add the setting's variable-name as an item in the tree
      QTreeWidgetItem* setting_item = new QTreeWidgetItem();
      setting_item->setText(0, "script variable");
      setting_item->setDisabled(true);
      setting_item->setText(1, tool->scriptVariable());
      tool_item->addChild(setting_item);
    }
    { // Add whether or not this is a support-material tool as an item
      QTreeWidgetItem* setting_item = new QTreeWidgetItem();
      setting_item->setText(0, "support material?");
      setting_item->setDisabled(true);
      setting_item->setText(1, tool->isSupportMaterial() ? "yes" : "no");
      tool_item->addChild(setting_item);
    }
    foreach (ToolScriptSetting* setting, tool->settings()) {
      QTreeWidgetItem* setting_item = new QTreeWidgetItem();
      setting_item->setText(0, setting->name());
      setting_item->setText(1, setting->valueString(false));
      setting_item->setText(2, setting->units());
      tool_item->addChild(setting_item);
    }
    m_ui->parametersTable->addTopLevelItem(tool_item);
  }

  // Set the printing script
  m_ui->scriptText->setPlainText(tool_script_->printScript());

  todo("kwg8", "set toolscript's preferred slicer");
  todo("kwg8", "set toolscript's preferred pather");
}

bool ModifyToolAndPrinterSettingsDialog::generateSupportMaterial() const {
  return m_ui->generateSupportMaterial->isChecked();
}

PatherImplementation ModifyToolAndPrinterSettingsDialog::patherImplementation() const {
  return PatherInterface::getImplementation(m_ui->patherCombo->currentIndex());
}

ModifyToolAndPrinterSettingsDialog::~ModifyToolAndPrinterSettingsDialog() {
  delete m_ui;
}

void ModifyToolAndPrinterSettingsDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
      m_ui->retranslateUi(this);
      break;
  default:
      break;
  }
}
