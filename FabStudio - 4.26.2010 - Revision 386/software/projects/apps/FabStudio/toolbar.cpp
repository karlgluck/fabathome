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
#include "toolbar.h"
#include "ui_toolbar.h"
#include "mainwindow.h"

Toolbar::Toolbar(MainWindow* main_window) :
    QWidget(0), // the toolbar is never created with a parent
    m_ui(new Ui::Toolbar),
    main_window_(main_window)
{
    m_ui->setupUi(this);
}

Toolbar::~Toolbar()
{
    delete m_ui;
}


void Toolbar::setCheckMarks(bool load_objects,
                            bool assign_materials,
                            bool send_to_printer) {
  QIcon check_icon(":/media/check.png");
  QIcon none;

  m_ui->loadObjects->setIcon(load_objects ? check_icon : none);
  m_ui->selectMaterials->setEnabled(load_objects);
  m_ui->selectMaterials->setIcon(assign_materials ? check_icon : none);
  m_ui->sendToPrinter->setEnabled(load_objects && assign_materials);
  m_ui->sendToPrinter->setIcon(send_to_printer ? check_icon : none);
}

void Toolbar::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Toolbar::on_loadObjects_clicked() {
  // Display the 'load objects' panel on the main window
  main_window_->showLoadObjectsPanel();
}

void Toolbar::on_sendToPrinter_clicked() {
  main_window_->showPrintPanel();
}

void Toolbar::on_selectMaterials_clicked() {
  main_window_->showMaterialsPanel();
}
