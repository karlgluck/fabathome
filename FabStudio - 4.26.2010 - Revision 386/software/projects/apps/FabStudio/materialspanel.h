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
#ifndef MATERIALSPANEL_H
#define MATERIALSPANEL_H

#include <QtGui/QDockWidget>
#include <QMap>
#include <QMutex>
#include <QSet>

namespace Ui {
    class MaterialsPanel;
}

class MainWindow;
class ToolScript;
class LoadToolScriptsThread;
class PrintableObject;

class MaterialsPanel : public QDockWidget {
    Q_OBJECT
public:
  MaterialsPanel(MainWindow* main_window, QWidget *parent = 0);
  ~MaterialsPanel();

  /**
   * Returns 'true' if a materials panel's materials list is the source of the
   * drag-and-drop event.
   */
  bool isSourceOfEvent(QDragEnterEvent* event) const;

  /**
   * Returns the name of the material that was dragged onto an object in the main window.
   * If this event was not created by the user dragging a material to the main window,
   * this method returns 'false' and does not modify material_name.
   */
  bool getDraggedMaterialName(QDropEvent* event, QString* material_name);

  /**
   * Processes the input files and adds their material configuration to the panel.
   * This method is called from a separate thread by the LoadToolScriptsThread
   * class.
   */
  void addToolScripts(const QString& path, const QStringList& files);

  /**
   * Called when the given material is assigned to an object or an object
   */
  void materialAssignmentChanged(const QString& material_name, int references);

  /**
   * Provides the list of material names that are in use by objects in the scene
   * to the materials panel so that it can synchronize the active/compatible
   * materials lists.
   * @return A mapping of assigned material names to the color with which the
   *         material is associated in the panel
   */
  const QMap<QString, QColor>& updateAssignedMaterials(const QSet<QString>& assigned_materials);

  /**
   * Updates the active/compatible materials lists
   */
  void updateUI();

  /**
   * Returns the tool-script with the given name, or null if no such script exists
   */
  ToolScript* getToolScriptByName(const QString& name);

  /**
   * Turns on or off all "advanced" features of the program.  This is used to turn FabStudio
   * from a simple, general and fail-proof tool into a fully-featured 3d printing program
   * or back again.
   */
  void useAdvancedMode(bool enable);

  /**
   * Activates or deactivates the 'remove materials' button based on whether an object is
   * slected.
   */
  void setSelectedObject(PrintableObject* object);

public slots:
signals:

  /**
   * This signal should be connected to the print panel so that when the user changes
   * material assignments, the dependent combo box can update.
   */
  void changedMaterials(QStringList compatible_script_names);


protected:
  void changeEvent(QEvent *e);

  /**
   * Worker method called for each file in addToolScripts
   */
  void addToolScript(const QString& file);


private:
  Ui::MaterialsPanel *m_ui;

  MainWindow* main_window_;

  LoadToolScriptsThread* loading_thread_;

  /// Mutual-exclusion object to ensure that the loading thread doesn't interfere
  /// with any other thread when accessing materials
  QMutex materials_mutex_;

  /// Holds information about all toolscripts that have been loaded from disk
  QVector<ToolScript*> tool_scripts_;

  /// Current set of materials known to be assigned to objects
  QSet<QString> assigned_materials_;

  /// Gives each color a defined, unique color
  QMap<QString, QColor> assigned_material_colors_;

  /// Prevent reentrancy for signals
  bool is_updating_;

private slots:
    void on_removeMaterial_clicked();
    void on_printerCombo_currentIndexChanged(int index);
};

#endif // MATERIALSPANEL_H
