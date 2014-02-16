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
#include "materialspanel.h"
#include "ui_materialspanel.h"

#include "shared/toolscript/toolscript.h"
#include "shared/toolscript/toolscriptsetting.h"
#include "shared/toolscript/toolscripttool.h"
#include "loadtoolscriptsthread.h"

#include <QDir>

#include "common/todo.h"
#include "common/confirm.h"
#include "mainwindow.h"


#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDebug>


MaterialsPanel::MaterialsPanel(MainWindow* main_window, QWidget *parent) :
    QDockWidget(parent),
    main_window_(main_window),
    m_ui(new Ui::MaterialsPanel) {
  m_ui->setupUi(this);
  is_updating_ = false;

  loading_thread_ = new LoadToolScriptsThread(this);
  loading_thread_->start();
}

MaterialsPanel::~MaterialsPanel() {

  // Close the loading thread
  loading_thread_->exit(0);
  delete loading_thread_;

  // Erase all of the tool-scripts
  foreach (ToolScript* ts, tool_scripts_) {
    delete ts;
  }

  // Free our UI
  delete m_ui;
}


bool MaterialsPanel::isSourceOfEvent(QDragEnterEvent* event) const {
  return event->source() == m_ui->assignedMaterials || event->source() == m_ui->compatibleMaterials;
}

bool MaterialsPanel::getDraggedMaterialName(QDropEvent* event, QString* material_name) {
  if (event->source() == m_ui->assignedMaterials) {

    QList<QListWidgetItem*> items = m_ui->assignedMaterials->selectedItems();
    if (items.isEmpty()) return false;
    const QListWidgetItem* front = items.front();
    *material_name = front->text();

  } else if (event->source() == m_ui->compatibleMaterials) {

    QList<QListWidgetItem*> items = m_ui->compatibleMaterials->selectedItems();
    if (items.isEmpty()) return false;
    const QListWidgetItem* front = items.front();
    *material_name = front->text();

  } else {
    // this panel was not the source of the event
    return false;
  }

  return true;
}

void MaterialsPanel::addToolScripts(const QString& path, const QStringList& files) {
  foreach (const QString& file_name, files) {
    addToolScript(path + QDir::separator() + file_name);
  }
}


void MaterialsPanel::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

const QMap<QString, QColor>& MaterialsPanel::updateAssignedMaterials(const QSet<QString>& assigned_materials) {
  todo("kwg8", "optimize updateAssignedMaterials by not calling updateUI if the set of "\
               "materials is the same as the current one");
  assigned_materials_.clear();
  assigned_materials_.unite(assigned_materials);
  updateUI();
  return assigned_material_colors_;
}

void MaterialsPanel::updateUI() {
  is_updating_ = true;
  QStringList compatible_script_names;

  { // Find all printers
    QSet<QString> printers;
    foreach (ToolScript* tool_script, tool_scripts_) {
      printers.insert(tool_script->printerName());
    }
    QString old = m_ui->printerCombo->currentText();
    int old_index = -1;
    m_ui->printerCombo->clear();
    QStringList printers_list(printers.toList());
    for (int i = 0; i < printers_list.size(); ++i) {
      const QString& printer = printers_list.at(i);
      if ((old_index == -1) &&
          (0 == QString::compare(printer,old,Qt::CaseSensitive))) {
        old_index = i;
      }
      m_ui->printerCombo->addItem(printer);
    }

    // Select the first ool by default
    if (old_index == -1 && !printers_list.isEmpty()) old_index = 0;

    // Re-select a tool from the list
    m_ui->printerCombo->setCurrentIndex(old_index);
  }

  {
    QString current_printer_name = m_ui->printerCombo->currentText();
    QMutexLocker lock(&materials_mutex_);

    // All materials that can be assigned to objects are here
    QSet<QString> compatible_materials;
    QList<ToolScript*> compatible_scripts;

    // Search all of the scripts to find available materials
    foreach (ToolScript* tool_script, tool_scripts_) {
      QSet<QString> script_materials;

      // Ignore scripts that work with a different printer
      if (0 != QString::compare(tool_script->printerName(), current_printer_name)) {
        continue;
      }

      // Add all of the different processable materials
      foreach (ToolScriptTool* tool, tool_script->tools()) {
        script_materials.insert(tool->materialName());
      }

      bool is_compatible = true;
      foreach (const QString& material, assigned_materials_) {
        if (script_materials.contains(material) == false) {
          // this script can't be used since it isn't able to
          // process all of the materials in the scene
          is_compatible = false;
          break;
        }
      }

      if (is_compatible) {
        // If we get here, the script has all of the materials in the
        // scene and possibly extra--so add them all to the list of
        // available materials.
        compatible_materials.unite(script_materials);
        compatible_scripts.append(tool_script);
      }
    }

    // Update the set of compatible tool-scripts
    foreach (ToolScript* ts, compatible_scripts) {
      compatible_script_names.append(ts->name());
    }

    // Remove assigned materials from the compatible materials so the
    // user isn't confused by having the same material in multiple
    // lists.
    compatible_materials.subtract(assigned_materials_);

    // Reset the state of the compatible materials UI element and
    // re-fill it with the new set of material names.
    m_ui->compatibleMaterials->clear();
    foreach (const QString& material, compatible_materials) {
      QListWidgetItem* material_item = new QListWidgetItem();
      material_item->setText(material);
      m_ui->compatibleMaterials->addItem(material_item);
    }

    // Rebuild the state of the active materials UI element
    m_ui->assignedMaterials->clear();
    foreach (const QString& material, assigned_materials_) {
      QListWidgetItem* material_item = new QListWidgetItem();
      material_item->setText(material);
      m_ui->assignedMaterials->addItem(material_item);

      // Has this material been assigned a color?
      QColor assigned_color;
      if (!assigned_material_colors_.contains(material)) {
        // No color yet, so find a unique one and assign it
        static const Qt::GlobalColor kMaterialColorPalette[] = {
          Qt::green,     Qt::red,         Qt::yellow,
          Qt::cyan,      Qt::blue,        Qt::magenta,
          Qt::darkGreen, Qt::darkRed,     Qt::darkBlue,
          Qt::darkCyan,  Qt::darkMagenta, Qt::darkYellow
        };
        static const int kMaterialColorPaletteSize
            = sizeof(kMaterialColorPalette) / sizeof(Qt::GlobalColor);
        QList<QColor> used_colors = assigned_material_colors_.values();
        int i = 0;
        for (i = 0; i < kMaterialColorPaletteSize; ++i) {
          QColor color(kMaterialColorPalette[i]);
          if (!used_colors.contains(color)) {
            // assign this color to the material
            assigned_material_colors_.insert(material, color);
            assigned_color = color;
            break;
          }
        }
        if (i == kMaterialColorPaletteSize) {
          // We were unable to find a unique color, so just randomly pick a color
          // This could involve a guarantee of uniqueness by adding a while() loop
          // but it's unlikely that there will be more than a handful of materials
          // and any new color won't be highly distinguishable anyway, so adding the
          // code isn't really worth it.
          QColor color(qrand()%255, qrand()%255, qrand()%255);
          assigned_material_colors_.insert(material, color);
          assigned_color = color;
        }
      } else {
        assigned_color = assigned_material_colors_.find(material).value();
      }

      // Set the background to match the material's color so that it's easy
      // to match objects with materials
      material_item->setBackgroundColor(assigned_color);
    }

    { // Remove material color pairings for materials that are no longer assigned
      QList<QString> unused_materials;
      foreach (const QString& material, assigned_material_colors_.keys()) {
        if (!assigned_materials_.contains(material)) {
          unused_materials.append(material);
        }
      }
      foreach (const QString& material, unused_materials) {
        assigned_material_colors_.remove(material);
      }
    }
  }

  // Signal that the materials have changed.  This is done outside of
  // the tool-scripts mutex because it can cause deadlock otherwise.
  emit changedMaterials(compatible_script_names);

  // Finished!
  is_updating_ = false;
}


ToolScript* MaterialsPanel::getToolScriptByName(const QString& name) {
  QMutexLocker lock(&materials_mutex_);
  foreach (ToolScript* ts, tool_scripts_) {
    if (0 == QString::compare(ts->name(), name, Qt::CaseSensitive)) return ts;
  }
  return 0;
}


void MaterialsPanel::useAdvancedMode(bool enable) {
}

void MaterialsPanel::setSelectedObject(PrintableObject* object) {
  m_ui->removeMaterial->setEnabled(object != 0);
}


void MaterialsPanel::addToolScript(const QString& file) {

  { // Add this new tool script.  Scope this section so that the
    // mutex doesn't get doubly locked when we call updateUI below
    QMutexLocker lock(&materials_mutex_);

    // Load the tool-script from disk
    ToolScript* tool_script = new ToolScript();
    if (!tool_script->load(file)) {
      qDebug() << "Failed to load tool script " << file;
      delete tool_script;
      return;
    }

    // Add to the list of tools
    qDebug() << "Loaded tool script " << file;
    tool_scripts_.append(tool_script);
  }

  // Sync the list of compatible materials
  updateUI();
}



void MaterialsPanel::on_printerCombo_currentIndexChanged(int index) {
  if (!is_updating_) {

    // Clear all material assignments since the printer is changing
    main_window_->clearAllObjectFillMaterials();

    // Sync the user interface
    updateUI();
  }
}

void MaterialsPanel::on_removeMaterial_clicked() {
  main_window_->clearSelectedObjectFillMaterials();
}
