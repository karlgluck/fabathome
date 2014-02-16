/********************************************************************************
** Form generated from reading ui file 'positionobjectspanel.ui'
**
** Created: Sun Apr 18 16:53:40 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_POSITIONOBJECTSPANEL_H
#define UI_POSITIONOBJECTSPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QDockWidget>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PositionObjectsPanel
{
public:
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QCommandLinkButton *autoArrange;
    QGroupBox *positionObjectsCurrentSelectionGroupBox;
    QVBoxLayout *verticalLayout_2;
    QLabel *deletingObjectLabel;
    QTabWidget *selectionTabs;
    QWidget *positionTab;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_3;
    QVBoxLayout *verticalLayout_8;
    QCommandLinkButton *putOnTray;
    QFrame *positionGrid;
    QFormLayout *formLayout;
    QLabel *label;
    QDoubleSpinBox *positionX;
    QDoubleSpinBox *positionY;
    QDoubleSpinBox *positionZ;
    QLabel *label_2;
    QLabel *label_3;
    QSpacerItem *verticalSpacer;
    QWidget *rotationTab;
    QVBoxLayout *verticalLayout_4;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QVBoxLayout *verticalLayout_7;
    QCommandLinkButton *findBestRotation;
    QFrame *rotationGrid;
    QFormLayout *formLayout_2;
    QLabel *label_7;
    QDoubleSpinBox *rotationX;
    QDoubleSpinBox *rotationY;
    QDoubleSpinBox *rotationZ;
    QLabel *label_9;
    QLabel *label_8;
    QSpacerItem *verticalSpacer_2;
    QWidget *scalingTab;
    QVBoxLayout *verticalLayout_5;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_6;
    QCommandLinkButton *scaleFromInches;
    QCommandLinkButton *pickReasonableSize;
    QCheckBox *uniformScalingCheckbox;
    QFrame *scalingGrid;
    QFormLayout *formLayout_3;
    QLabel *label_10;
    QDoubleSpinBox *scalingX;
    QDoubleSpinBox *scalingY;
    QDoubleSpinBox *scalingZ;
    QLabel *label_12;
    QLabel *label_11;
    QSpacerItem *verticalSpacer_3;
    QCommandLinkButton *deleteObject;

    void setupUi(QDockWidget *PositionObjectsPanel)
    {
        if (PositionObjectsPanel->objectName().isEmpty())
            PositionObjectsPanel->setObjectName(QString::fromUtf8("PositionObjectsPanel"));
        PositionObjectsPanel->resize(258, 557);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PositionObjectsPanel->sizePolicy().hasHeightForWidth());
        PositionObjectsPanel->setSizePolicy(sizePolicy);
        PositionObjectsPanel->setMinimumSize(QSize(258, 483));
        PositionObjectsPanel->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        PositionObjectsPanel->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        autoArrange = new QCommandLinkButton(dockWidgetContents);
        autoArrange->setObjectName(QString::fromUtf8("autoArrange"));

        verticalLayout->addWidget(autoArrange);

        positionObjectsCurrentSelectionGroupBox = new QGroupBox(dockWidgetContents);
        positionObjectsCurrentSelectionGroupBox->setObjectName(QString::fromUtf8("positionObjectsCurrentSelectionGroupBox"));
        positionObjectsCurrentSelectionGroupBox->setEnabled(true);
        positionObjectsCurrentSelectionGroupBox->setFlat(false);
        positionObjectsCurrentSelectionGroupBox->setCheckable(false);
        verticalLayout_2 = new QVBoxLayout(positionObjectsCurrentSelectionGroupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        deletingObjectLabel = new QLabel(positionObjectsCurrentSelectionGroupBox);
        deletingObjectLabel->setObjectName(QString::fromUtf8("deletingObjectLabel"));

        verticalLayout_2->addWidget(deletingObjectLabel);

        selectionTabs = new QTabWidget(positionObjectsCurrentSelectionGroupBox);
        selectionTabs->setObjectName(QString::fromUtf8("selectionTabs"));
        selectionTabs->setEnabled(false);
        positionTab = new QWidget();
        positionTab->setObjectName(QString::fromUtf8("positionTab"));
        verticalLayout_3 = new QVBoxLayout(positionTab);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setMargin(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        scrollArea_3 = new QScrollArea(positionTab);
        scrollArea_3->setObjectName(QString::fromUtf8("scrollArea_3"));
        scrollArea_3->setFrameShape(QFrame::NoFrame);
        scrollArea_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 214, 310));
        verticalLayout_8 = new QVBoxLayout(scrollAreaWidgetContents_3);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        putOnTray = new QCommandLinkButton(scrollAreaWidgetContents_3);
        putOnTray->setObjectName(QString::fromUtf8("putOnTray"));

        verticalLayout_8->addWidget(putOnTray);

        positionGrid = new QFrame(scrollAreaWidgetContents_3);
        positionGrid->setObjectName(QString::fromUtf8("positionGrid"));
        positionGrid->setFrameShape(QFrame::StyledPanel);
        positionGrid->setFrameShadow(QFrame::Raised);
        formLayout = new QFormLayout(positionGrid);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(positionGrid);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMaximumSize(QSize(20, 16777215));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        positionX = new QDoubleSpinBox(positionGrid);
        positionX->setObjectName(QString::fromUtf8("positionX"));
        positionX->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        positionX->setAccelerated(true);
        positionX->setMinimum(-500);
        positionX->setMaximum(500);

        formLayout->setWidget(0, QFormLayout::FieldRole, positionX);

        positionY = new QDoubleSpinBox(positionGrid);
        positionY->setObjectName(QString::fromUtf8("positionY"));
        positionY->setAccelerated(true);
        positionY->setMinimum(-500);
        positionY->setMaximum(500);

        formLayout->setWidget(1, QFormLayout::FieldRole, positionY);

        positionZ = new QDoubleSpinBox(positionGrid);
        positionZ->setObjectName(QString::fromUtf8("positionZ"));
        positionZ->setAccelerated(true);
        positionZ->setMinimum(-500);
        positionZ->setMaximum(500);

        formLayout->setWidget(2, QFormLayout::FieldRole, positionZ);

        label_2 = new QLabel(positionGrid);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setMaximumSize(QSize(20, 16777215));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(positionGrid);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMaximumSize(QSize(20, 16777215));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);


        verticalLayout_8->addWidget(positionGrid);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer);

        scrollArea_3->setWidget(scrollAreaWidgetContents_3);

        verticalLayout_3->addWidget(scrollArea_3);

        selectionTabs->addTab(positionTab, QString());
        rotationTab = new QWidget();
        rotationTab->setObjectName(QString::fromUtf8("rotationTab"));
        verticalLayout_4 = new QVBoxLayout(rotationTab);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setMargin(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        scrollArea_2 = new QScrollArea(rotationTab);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setFrameShape(QFrame::NoFrame);
        scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 214, 310));
        verticalLayout_7 = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        findBestRotation = new QCommandLinkButton(scrollAreaWidgetContents_2);
        findBestRotation->setObjectName(QString::fromUtf8("findBestRotation"));

        verticalLayout_7->addWidget(findBestRotation);

        rotationGrid = new QFrame(scrollAreaWidgetContents_2);
        rotationGrid->setObjectName(QString::fromUtf8("rotationGrid"));
        rotationGrid->setFrameShape(QFrame::StyledPanel);
        rotationGrid->setFrameShadow(QFrame::Raised);
        formLayout_2 = new QFormLayout(rotationGrid);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        label_7 = new QLabel(rotationGrid);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);
        label_7->setMaximumSize(QSize(20, 16777215));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_7);

        rotationX = new QDoubleSpinBox(rotationGrid);
        rotationX->setObjectName(QString::fromUtf8("rotationX"));
        rotationX->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        rotationX->setAccelerated(false);
        rotationX->setMinimum(-360);
        rotationX->setMaximum(360);
        rotationX->setSingleStep(90);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, rotationX);

        rotationY = new QDoubleSpinBox(rotationGrid);
        rotationY->setObjectName(QString::fromUtf8("rotationY"));
        rotationY->setAccelerated(false);
        rotationY->setMinimum(-360);
        rotationY->setMaximum(360);
        rotationY->setSingleStep(90);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, rotationY);

        rotationZ = new QDoubleSpinBox(rotationGrid);
        rotationZ->setObjectName(QString::fromUtf8("rotationZ"));
        rotationZ->setAccelerated(false);
        rotationZ->setMinimum(-360);
        rotationZ->setMaximum(360);
        rotationZ->setSingleStep(90);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, rotationZ);

        label_9 = new QLabel(rotationGrid);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy1.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy1);
        label_9->setMaximumSize(QSize(20, 16777215));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_9);

        label_8 = new QLabel(rotationGrid);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy1.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy1);
        label_8->setMaximumSize(QSize(20, 16777215));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_8);


        verticalLayout_7->addWidget(rotationGrid);

        verticalSpacer_2 = new QSpacerItem(20, 123, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_2);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_4->addWidget(scrollArea_2);

        selectionTabs->addTab(rotationTab, QString());
        scalingTab = new QWidget();
        scalingTab->setObjectName(QString::fromUtf8("scalingTab"));
        verticalLayout_5 = new QVBoxLayout(scalingTab);
        verticalLayout_5->setMargin(0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        scrollArea = new QScrollArea(scalingTab);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 214, 316));
        verticalLayout_6 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        scaleFromInches = new QCommandLinkButton(scrollAreaWidgetContents);
        scaleFromInches->setObjectName(QString::fromUtf8("scaleFromInches"));

        verticalLayout_6->addWidget(scaleFromInches);

        pickReasonableSize = new QCommandLinkButton(scrollAreaWidgetContents);
        pickReasonableSize->setObjectName(QString::fromUtf8("pickReasonableSize"));

        verticalLayout_6->addWidget(pickReasonableSize);

        uniformScalingCheckbox = new QCheckBox(scrollAreaWidgetContents);
        uniformScalingCheckbox->setObjectName(QString::fromUtf8("uniformScalingCheckbox"));
        uniformScalingCheckbox->setChecked(true);

        verticalLayout_6->addWidget(uniformScalingCheckbox);

        scalingGrid = new QFrame(scrollAreaWidgetContents);
        scalingGrid->setObjectName(QString::fromUtf8("scalingGrid"));
        scalingGrid->setFrameShape(QFrame::StyledPanel);
        scalingGrid->setFrameShadow(QFrame::Raised);
        formLayout_3 = new QFormLayout(scalingGrid);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        label_10 = new QLabel(scalingGrid);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        sizePolicy1.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy1);
        label_10->setMaximumSize(QSize(20, 16777215));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_10);

        scalingX = new QDoubleSpinBox(scalingGrid);
        scalingX->setObjectName(QString::fromUtf8("scalingX"));
        scalingX->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        scalingX->setAccelerated(false);
        scalingX->setDecimals(1);
        scalingX->setMinimum(0);
        scalingX->setMaximum(10000);
        scalingX->setSingleStep(5);
        scalingX->setValue(100);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, scalingX);

        scalingY = new QDoubleSpinBox(scalingGrid);
        scalingY->setObjectName(QString::fromUtf8("scalingY"));
        scalingY->setReadOnly(true);
        scalingY->setAccelerated(false);
        scalingY->setDecimals(1);
        scalingY->setMinimum(0);
        scalingY->setMaximum(10000);
        scalingY->setSingleStep(5);
        scalingY->setValue(100);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, scalingY);

        scalingZ = new QDoubleSpinBox(scalingGrid);
        scalingZ->setObjectName(QString::fromUtf8("scalingZ"));
        scalingZ->setReadOnly(true);
        scalingZ->setAccelerated(false);
        scalingZ->setDecimals(1);
        scalingZ->setMinimum(0);
        scalingZ->setMaximum(10000);
        scalingZ->setSingleStep(5);
        scalingZ->setValue(100);

        formLayout_3->setWidget(2, QFormLayout::FieldRole, scalingZ);

        label_12 = new QLabel(scalingGrid);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        sizePolicy1.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy1);
        label_12->setMaximumSize(QSize(20, 16777215));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_12);

        label_11 = new QLabel(scalingGrid);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy1.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy1);
        label_11->setMaximumSize(QSize(20, 16777215));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(2, QFormLayout::LabelRole, label_11);


        verticalLayout_6->addWidget(scalingGrid);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_3);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_5->addWidget(scrollArea);

        selectionTabs->addTab(scalingTab, QString());

        verticalLayout_2->addWidget(selectionTabs);

        deleteObject = new QCommandLinkButton(positionObjectsCurrentSelectionGroupBox);
        deleteObject->setObjectName(QString::fromUtf8("deleteObject"));

        verticalLayout_2->addWidget(deleteObject);


        verticalLayout->addWidget(positionObjectsCurrentSelectionGroupBox);

        PositionObjectsPanel->setWidget(dockWidgetContents);

        retranslateUi(PositionObjectsPanel);

        selectionTabs->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(PositionObjectsPanel);
    } // setupUi

    void retranslateUi(QDockWidget *PositionObjectsPanel)
    {
        PositionObjectsPanel->setWindowTitle(QApplication::translate("PositionObjectsPanel", "Edit Objects", 0, QApplication::UnicodeUTF8));
        autoArrange->setText(QApplication::translate("PositionObjectsPanel", "Arrange Objects", 0, QApplication::UnicodeUTF8));
        autoArrange->setDescription(QApplication::translate("PositionObjectsPanel", "Automatically put all objects in a good position for printing", 0, QApplication::UnicodeUTF8));
        positionObjectsCurrentSelectionGroupBox->setTitle(QApplication::translate("PositionObjectsPanel", "With Current Selection...", 0, QApplication::UnicodeUTF8));
        deletingObjectLabel->setText(QApplication::translate("PositionObjectsPanel", "This object will be deleted.", 0, QApplication::UnicodeUTF8));
        putOnTray->setText(QApplication::translate("PositionObjectsPanel", "Put on Tray", 0, QApplication::UnicodeUTF8));
        putOnTray->setDescription(QApplication::translate("PositionObjectsPanel", "Reposition vertically to set the base on the tray surface", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PositionObjectsPanel", "X:", 0, QApplication::UnicodeUTF8));
        positionX->setSuffix(QApplication::translate("PositionObjectsPanel", " mm", 0, QApplication::UnicodeUTF8));
        positionY->setSuffix(QApplication::translate("PositionObjectsPanel", " mm", 0, QApplication::UnicodeUTF8));
        positionZ->setSuffix(QApplication::translate("PositionObjectsPanel", " mm", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PositionObjectsPanel", "Y:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PositionObjectsPanel", "Z:", 0, QApplication::UnicodeUTF8));
        selectionTabs->setTabText(selectionTabs->indexOf(positionTab), QApplication::translate("PositionObjectsPanel", "Position", 0, QApplication::UnicodeUTF8));
        findBestRotation->setText(QApplication::translate("PositionObjectsPanel", "Find Best Rotation", 0, QApplication::UnicodeUTF8));
        findBestRotation->setDescription(QApplication::translate("PositionObjectsPanel", "Orient the object to the best build rotation", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("PositionObjectsPanel", "X:", 0, QApplication::UnicodeUTF8));
        rotationX->setSuffix(QApplication::translate("PositionObjectsPanel", " deg", 0, QApplication::UnicodeUTF8));
        rotationY->setSuffix(QApplication::translate("PositionObjectsPanel", " deg", 0, QApplication::UnicodeUTF8));
        rotationZ->setSuffix(QApplication::translate("PositionObjectsPanel", " deg", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("PositionObjectsPanel", "Y:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("PositionObjectsPanel", "Z:", 0, QApplication::UnicodeUTF8));
        selectionTabs->setTabText(selectionTabs->indexOf(rotationTab), QApplication::translate("PositionObjectsPanel", "Rotation", 0, QApplication::UnicodeUTF8));
        scaleFromInches->setText(QApplication::translate("PositionObjectsPanel", "Scale from Inches", 0, QApplication::UnicodeUTF8));
        scaleFromInches->setDescription(QApplication::translate("PositionObjectsPanel", "Set the object's size to scale from a model with coordinates in inches", 0, QApplication::UnicodeUTF8));
        pickReasonableSize->setText(QApplication::translate("PositionObjectsPanel", "Pick Reasonable Size", 0, QApplication::UnicodeUTF8));
        pickReasonableSize->setDescription(QApplication::translate("PositionObjectsPanel", "Estimate a size that could be good for printing", 0, QApplication::UnicodeUTF8));
        uniformScalingCheckbox->setText(QApplication::translate("PositionObjectsPanel", "Uniform Scaling", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("PositionObjectsPanel", "X:", 0, QApplication::UnicodeUTF8));
        scalingX->setPrefix(QString());
        scalingX->setSuffix(QApplication::translate("PositionObjectsPanel", "%", 0, QApplication::UnicodeUTF8));
        scalingY->setSuffix(QApplication::translate("PositionObjectsPanel", "%", 0, QApplication::UnicodeUTF8));
        scalingZ->setSuffix(QApplication::translate("PositionObjectsPanel", "%", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("PositionObjectsPanel", "Y:", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("PositionObjectsPanel", "Z:", 0, QApplication::UnicodeUTF8));
        selectionTabs->setTabText(selectionTabs->indexOf(scalingTab), QApplication::translate("PositionObjectsPanel", "Scaling", 0, QApplication::UnicodeUTF8));
        deleteObject->setText(QApplication::translate("PositionObjectsPanel", "Delete Object", 0, QApplication::UnicodeUTF8));
        deleteObject->setDescription(QApplication::translate("PositionObjectsPanel", "Erase this object from the scene", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(PositionObjectsPanel);
    } // retranslateUi

};

namespace Ui {
    class PositionObjectsPanel: public Ui_PositionObjectsPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POSITIONOBJECTSPANEL_H
