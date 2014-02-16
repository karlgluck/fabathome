/********************************************************************************
** Form generated from reading ui file 'modifytoolandprintersettingsdialog.ui'
**
** Created: Sun Apr 18 18:53:16 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MODIFYTOOLANDPRINTERSETTINGSDIALOG_H
#define UI_MODIFYTOOLANDPRINTERSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModifyToolAndPrinterSettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTabWidget *primaryTabWidget;
    QWidget *tabToolScript;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_5;
    QTabWidget *basicAdvancedTabs;
    QWidget *tabBasic;
    QVBoxLayout *verticalLayout_4;
    QPlainTextEdit *basicDescription;
    QWidget *tabParameters;
    QVBoxLayout *verticalLayout_6;
    QTreeWidget *parametersTable;
    QWidget *tabScript;
    QVBoxLayout *verticalLayout_5;
    QPlainTextEdit *scriptText;
    QWidget *tabPrintingProcess;
    QFormLayout *formLayout_2;
    QCheckBox *generateSupportMaterial;
    QLabel *label_4;
    QComboBox *patherCombo;

    void setupUi(QDialog *ModifyToolAndPrinterSettingsDialog)
    {
        if (ModifyToolAndPrinterSettingsDialog->objectName().isEmpty())
            ModifyToolAndPrinterSettingsDialog->setObjectName(QString::fromUtf8("ModifyToolAndPrinterSettingsDialog"));
        ModifyToolAndPrinterSettingsDialog->setWindowModality(Qt::ApplicationModal);
        ModifyToolAndPrinterSettingsDialog->resize(443, 618);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/media/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        ModifyToolAndPrinterSettingsDialog->setWindowIcon(icon);
        ModifyToolAndPrinterSettingsDialog->setSizeGripEnabled(true);
        ModifyToolAndPrinterSettingsDialog->setModal(true);
        verticalLayout = new QVBoxLayout(ModifyToolAndPrinterSettingsDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(ModifyToolAndPrinterSettingsDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setWordWrap(true);
        label->setMargin(16);

        verticalLayout->addWidget(label);

        primaryTabWidget = new QTabWidget(ModifyToolAndPrinterSettingsDialog);
        primaryTabWidget->setObjectName(QString::fromUtf8("primaryTabWidget"));
        tabToolScript = new QWidget();
        tabToolScript->setObjectName(QString::fromUtf8("tabToolScript"));
        verticalLayout_7 = new QVBoxLayout(tabToolScript);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        label_5 = new QLabel(tabToolScript);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_7->addWidget(label_5);

        basicAdvancedTabs = new QTabWidget(tabToolScript);
        basicAdvancedTabs->setObjectName(QString::fromUtf8("basicAdvancedTabs"));
        basicAdvancedTabs->setTabPosition(QTabWidget::North);
        tabBasic = new QWidget();
        tabBasic->setObjectName(QString::fromUtf8("tabBasic"));
        verticalLayout_4 = new QVBoxLayout(tabBasic);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        basicDescription = new QPlainTextEdit(tabBasic);
        basicDescription->setObjectName(QString::fromUtf8("basicDescription"));
        basicDescription->setTabChangesFocus(true);
        basicDescription->setUndoRedoEnabled(false);
        basicDescription->setReadOnly(true);

        verticalLayout_4->addWidget(basicDescription);

        basicAdvancedTabs->addTab(tabBasic, QString());
        tabParameters = new QWidget();
        tabParameters->setObjectName(QString::fromUtf8("tabParameters"));
        verticalLayout_6 = new QVBoxLayout(tabParameters);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        parametersTable = new QTreeWidget(tabParameters);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(2, QString::fromUtf8("3"));
        __qtreewidgetitem->setText(1, QString::fromUtf8("2"));
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        parametersTable->setHeaderItem(__qtreewidgetitem);
        parametersTable->setObjectName(QString::fromUtf8("parametersTable"));
        parametersTable->setColumnCount(3);
        parametersTable->header()->setDefaultSectionSize(135);
        parametersTable->header()->setMinimumSectionSize(35);
        parametersTable->header()->setProperty("showSortIndicator", QVariant(true));

        verticalLayout_6->addWidget(parametersTable);

        basicAdvancedTabs->addTab(tabParameters, QString());
        tabScript = new QWidget();
        tabScript->setObjectName(QString::fromUtf8("tabScript"));
        verticalLayout_5 = new QVBoxLayout(tabScript);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        scriptText = new QPlainTextEdit(tabScript);
        scriptText->setObjectName(QString::fromUtf8("scriptText"));
        scriptText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scriptText->setReadOnly(true);

        verticalLayout_5->addWidget(scriptText);

        basicAdvancedTabs->addTab(tabScript, QString());

        verticalLayout_7->addWidget(basicAdvancedTabs);

        primaryTabWidget->addTab(tabToolScript, QString());
        tabPrintingProcess = new QWidget();
        tabPrintingProcess->setObjectName(QString::fromUtf8("tabPrintingProcess"));
        formLayout_2 = new QFormLayout(tabPrintingProcess);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        generateSupportMaterial = new QCheckBox(tabPrintingProcess);
        generateSupportMaterial->setObjectName(QString::fromUtf8("generateSupportMaterial"));
        generateSupportMaterial->setChecked(true);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, generateSupportMaterial);

        label_4 = new QLabel(tabPrintingProcess);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_4);

        patherCombo = new QComboBox(tabPrintingProcess);
        patherCombo->setObjectName(QString::fromUtf8("patherCombo"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, patherCombo);

        primaryTabWidget->addTab(tabPrintingProcess, QString());

        verticalLayout->addWidget(primaryTabWidget);


        retranslateUi(ModifyToolAndPrinterSettingsDialog);

        primaryTabWidget->setCurrentIndex(1);
        basicAdvancedTabs->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ModifyToolAndPrinterSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *ModifyToolAndPrinterSettingsDialog)
    {
        ModifyToolAndPrinterSettingsDialog->setWindowTitle(QApplication::translate("ModifyToolAndPrinterSettingsDialog", "Modify Tool and Printer Settings", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ModifyToolAndPrinterSettingsDialog", "Make any changes to the print settings and tool-script below.  Once you are finished, close this dialog to return to printing.", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("ModifyToolAndPrinterSettingsDialog", "<b>This version of FabStudio does not support script editing.</b>", 0, QApplication::UnicodeUTF8));
        basicAdvancedTabs->setTabText(basicAdvancedTabs->indexOf(tabBasic), QApplication::translate("ModifyToolAndPrinterSettingsDialog", "Basic Info", 0, QApplication::UnicodeUTF8));
        basicAdvancedTabs->setTabText(basicAdvancedTabs->indexOf(tabParameters), QApplication::translate("ModifyToolAndPrinterSettingsDialog", "Parameters", 0, QApplication::UnicodeUTF8));
        basicAdvancedTabs->setTabText(basicAdvancedTabs->indexOf(tabScript), QApplication::translate("ModifyToolAndPrinterSettingsDialog", "Script", 0, QApplication::UnicodeUTF8));
        primaryTabWidget->setTabText(primaryTabWidget->indexOf(tabToolScript), QApplication::translate("ModifyToolAndPrinterSettingsDialog", "Tool Script", 0, QApplication::UnicodeUTF8));
        generateSupportMaterial->setText(QApplication::translate("ModifyToolAndPrinterSettingsDialog", "Generate Support Material", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ModifyToolAndPrinterSettingsDialog", "Pathing Algorithm:", 0, QApplication::UnicodeUTF8));
        primaryTabWidget->setTabText(primaryTabWidget->indexOf(tabPrintingProcess), QApplication::translate("ModifyToolAndPrinterSettingsDialog", "Printing Process", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ModifyToolAndPrinterSettingsDialog);
    } // retranslateUi

};

namespace Ui {
    class ModifyToolAndPrinterSettingsDialog: public Ui_ModifyToolAndPrinterSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODIFYTOOLANDPRINTERSETTINGSDIALOG_H
