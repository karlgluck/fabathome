/********************************************************************************
** Form generated from reading ui file 'materialspanel.ui'
**
** Created: Sun Apr 18 16:53:40 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MATERIALSPANEL_H
#define UI_MATERIALSPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QDockWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MaterialsPanel
{
public:
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *printerCombo;
    QGroupBox *groupAssigned;
    QVBoxLayout *verticalLayout_3;
    QListWidget *assignedMaterials;
    QCommandLinkButton *removeMaterial;
    QGroupBox *groupCompatible;
    QVBoxLayout *verticalLayout_2;
    QListWidget *compatibleMaterials;

    void setupUi(QDockWidget *MaterialsPanel)
    {
        if (MaterialsPanel->objectName().isEmpty())
            MaterialsPanel->setObjectName(QString::fromUtf8("MaterialsPanel"));
        MaterialsPanel->resize(250, 619);
        MaterialsPanel->setMinimumSize(QSize(250, 481));
        MaterialsPanel->setFloating(true);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(dockWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        printerCombo = new QComboBox(dockWidgetContents);
        printerCombo->setObjectName(QString::fromUtf8("printerCombo"));

        horizontalLayout->addWidget(printerCombo);


        verticalLayout->addLayout(horizontalLayout);

        groupAssigned = new QGroupBox(dockWidgetContents);
        groupAssigned->setObjectName(QString::fromUtf8("groupAssigned"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupAssigned->sizePolicy().hasHeightForWidth());
        groupAssigned->setSizePolicy(sizePolicy1);
        verticalLayout_3 = new QVBoxLayout(groupAssigned);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        assignedMaterials = new QListWidget(groupAssigned);
        assignedMaterials->setObjectName(QString::fromUtf8("assignedMaterials"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(assignedMaterials->sizePolicy().hasHeightForWidth());
        assignedMaterials->setSizePolicy(sizePolicy2);
        assignedMaterials->setMaximumSize(QSize(16777215, 16777215));
        assignedMaterials->setEditTriggers(QAbstractItemView::NoEditTriggers);
        assignedMaterials->setProperty("showDropIndicator", QVariant(false));
        assignedMaterials->setDragEnabled(true);
        assignedMaterials->setDragDropMode(QAbstractItemView::DragOnly);

        verticalLayout_3->addWidget(assignedMaterials);

        removeMaterial = new QCommandLinkButton(groupAssigned);
        removeMaterial->setObjectName(QString::fromUtf8("removeMaterial"));
        removeMaterial->setEnabled(false);

        verticalLayout_3->addWidget(removeMaterial);


        verticalLayout->addWidget(groupAssigned);

        groupCompatible = new QGroupBox(dockWidgetContents);
        groupCompatible->setObjectName(QString::fromUtf8("groupCompatible"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupCompatible->sizePolicy().hasHeightForWidth());
        groupCompatible->setSizePolicy(sizePolicy3);
        verticalLayout_2 = new QVBoxLayout(groupCompatible);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        compatibleMaterials = new QListWidget(groupCompatible);
        compatibleMaterials->setObjectName(QString::fromUtf8("compatibleMaterials"));
        compatibleMaterials->setEditTriggers(QAbstractItemView::NoEditTriggers);
        compatibleMaterials->setProperty("showDropIndicator", QVariant(false));
        compatibleMaterials->setDragEnabled(true);
        compatibleMaterials->setDragDropMode(QAbstractItemView::DragOnly);
        compatibleMaterials->setSortingEnabled(true);

        verticalLayout_2->addWidget(compatibleMaterials);


        verticalLayout->addWidget(groupCompatible);

        MaterialsPanel->setWidget(dockWidgetContents);

        retranslateUi(MaterialsPanel);

        QMetaObject::connectSlotsByName(MaterialsPanel);
    } // setupUi

    void retranslateUi(QDockWidget *MaterialsPanel)
    {
        MaterialsPanel->setWindowTitle(QApplication::translate("MaterialsPanel", "Materials", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MaterialsPanel", "Printer:", 0, QApplication::UnicodeUTF8));
        groupAssigned->setTitle(QApplication::translate("MaterialsPanel", "Assigned Build Materials", 0, QApplication::UnicodeUTF8));
        removeMaterial->setText(QApplication::translate("MaterialsPanel", "Remove Object Material", 0, QApplication::UnicodeUTF8));
        removeMaterial->setDescription(QApplication::translate("MaterialsPanel", "Clears the material assigned to the selected object", 0, QApplication::UnicodeUTF8));
        groupCompatible->setTitle(QApplication::translate("MaterialsPanel", "Compatible Build Materials", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MaterialsPanel);
    } // retranslateUi

};

namespace Ui {
    class MaterialsPanel: public Ui_MaterialsPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIALSPANEL_H
