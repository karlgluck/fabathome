/********************************************************************************
** Form generated from reading ui file 'printpanel.ui'
**
** Created: Mon Apr 19 16:49:05 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PRINTPANEL_H
#define UI_PRINTPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QDockWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrintPanel
{
public:
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *pageBeforePrint;
    QVBoxLayout *verticalLayout_7;
    QComboBox *toolCombo;
    QPlainTextEdit *textToolScriptDescription;
    QCommandLinkButton *print;
    QCommandLinkButton *modifyPrintSettings;
    QSpacerItem *verticalSpacer_2;
    QWidget *pageDuringPrint;
    QVBoxLayout *verticalLayout_10;
    QGroupBox *progressGroupBox;
    QVBoxLayout *verticalLayout_14;
    QProgressBar *taskProgressBar;
    QProgressBar *masterProgressBar;
    QCommandLinkButton *sendToPrinter;
    QCommandLinkButton *savePrintJob;
    QCommandLinkButton *abandonPrintJob;
    QSpacerItem *verticalSpacer;

    void setupUi(QDockWidget *PrintPanel)
    {
        if (PrintPanel->objectName().isEmpty())
            PrintPanel->setObjectName(QString::fromUtf8("PrintPanel"));
        PrintPanel->resize(300, 599);
        PrintPanel->setMinimumSize(QSize(300, 599));
        PrintPanel->setMaximumSize(QSize(388, 524287));
        PrintPanel->setFloating(true);
        PrintPanel->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(0);
        verticalLayout->setMargin(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        stackedWidget = new QStackedWidget(dockWidgetContents);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        pageBeforePrint = new QWidget();
        pageBeforePrint->setObjectName(QString::fromUtf8("pageBeforePrint"));
        verticalLayout_7 = new QVBoxLayout(pageBeforePrint);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        toolCombo = new QComboBox(pageBeforePrint);
        toolCombo->setObjectName(QString::fromUtf8("toolCombo"));

        verticalLayout_7->addWidget(toolCombo);

        textToolScriptDescription = new QPlainTextEdit(pageBeforePrint);
        textToolScriptDescription->setObjectName(QString::fromUtf8("textToolScriptDescription"));
        textToolScriptDescription->setTabChangesFocus(true);
        textToolScriptDescription->setUndoRedoEnabled(false);
        textToolScriptDescription->setReadOnly(true);

        verticalLayout_7->addWidget(textToolScriptDescription);

        print = new QCommandLinkButton(pageBeforePrint);
        print->setObjectName(QString::fromUtf8("print"));

        verticalLayout_7->addWidget(print);

        modifyPrintSettings = new QCommandLinkButton(pageBeforePrint);
        modifyPrintSettings->setObjectName(QString::fromUtf8("modifyPrintSettings"));

        verticalLayout_7->addWidget(modifyPrintSettings);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_2);

        stackedWidget->addWidget(pageBeforePrint);
        pageDuringPrint = new QWidget();
        pageDuringPrint->setObjectName(QString::fromUtf8("pageDuringPrint"));
        verticalLayout_10 = new QVBoxLayout(pageDuringPrint);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        progressGroupBox = new QGroupBox(pageDuringPrint);
        progressGroupBox->setObjectName(QString::fromUtf8("progressGroupBox"));
        verticalLayout_14 = new QVBoxLayout(progressGroupBox);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        taskProgressBar = new QProgressBar(progressGroupBox);
        taskProgressBar->setObjectName(QString::fromUtf8("taskProgressBar"));
        taskProgressBar->setMinimumSize(QSize(0, 24));
        taskProgressBar->setValue(0);
        taskProgressBar->setTextVisible(false);

        verticalLayout_14->addWidget(taskProgressBar);

        masterProgressBar = new QProgressBar(progressGroupBox);
        masterProgressBar->setObjectName(QString::fromUtf8("masterProgressBar"));
        masterProgressBar->setMinimumSize(QSize(0, 48));
        masterProgressBar->setValue(0);
        masterProgressBar->setTextVisible(false);

        verticalLayout_14->addWidget(masterProgressBar);


        verticalLayout_10->addWidget(progressGroupBox);

        sendToPrinter = new QCommandLinkButton(pageDuringPrint);
        sendToPrinter->setObjectName(QString::fromUtf8("sendToPrinter"));
        sendToPrinter->setEnabled(false);

        verticalLayout_10->addWidget(sendToPrinter);

        savePrintJob = new QCommandLinkButton(pageDuringPrint);
        savePrintJob->setObjectName(QString::fromUtf8("savePrintJob"));
        savePrintJob->setEnabled(false);

        verticalLayout_10->addWidget(savePrintJob);

        abandonPrintJob = new QCommandLinkButton(pageDuringPrint);
        abandonPrintJob->setObjectName(QString::fromUtf8("abandonPrintJob"));

        verticalLayout_10->addWidget(abandonPrintJob);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_10->addItem(verticalSpacer);

        stackedWidget->addWidget(pageDuringPrint);

        verticalLayout->addWidget(stackedWidget);

        PrintPanel->setWidget(dockWidgetContents);

        retranslateUi(PrintPanel);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PrintPanel);
    } // setupUi

    void retranslateUi(QDockWidget *PrintPanel)
    {
        PrintPanel->setWindowTitle(QApplication::translate("PrintPanel", "Print Scene", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolCombo->setToolTip(QApplication::translate("PrintPanel", "Pick a tool-script with which to print", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        toolCombo->setStatusTip(QApplication::translate("PrintPanel", "Pick a tool-script with which to print", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        textToolScriptDescription->setPlainText(QApplication::translate("PrintPanel", "To use this script, you need:\n"
"\n"
"    - Two bays available on the Fab@Home Model 2\n"
"    - A 10 cc syringe with an orange (1 mm) tip\n"
"    - A 10 cc syringe with a blue (0.5mm) tip\n"
"    - FabEpoxy, 3:1 mixture\n"
"    - Frosting\n"
"\n"
"Be sure to have extra supplies on hand if you are printing a large model.  This script will automatically support the object you are printing so it won't fall over, even if it has overhangs.", 0, QApplication::UnicodeUTF8));
        print->setText(QApplication::translate("PrintPanel", "Create Print Job", 0, QApplication::UnicodeUTF8));
        print->setDescription(QApplication::translate("PrintPanel", "Generates instructions for the printer .  This can take several minutes.  You can review the results before the printer starts building.", 0, QApplication::UnicodeUTF8));
        modifyPrintSettings->setText(QApplication::translate("PrintPanel", "Modify Tool and Printer Settings (Advanced)", 0, QApplication::UnicodeUTF8));
        modifyPrintSettings->setDescription(QApplication::translate("PrintPanel", "Edit the parameter values used by the print script, choose alternate planning algorithms, or give FabStudio hints about how to create your print job.", 0, QApplication::UnicodeUTF8));
        progressGroupBox->setTitle(QApplication::translate("PrintPanel", "Working....", 0, QApplication::UnicodeUTF8));
        sendToPrinter->setText(QApplication::translate("PrintPanel", "Send to Printer", 0, QApplication::UnicodeUTF8));
        sendToPrinter->setDescription(QApplication::translate("PrintPanel", "Connect to the printer and send it the current scene.  The program will step you through attaching the necessary tools.", 0, QApplication::UnicodeUTF8));
        savePrintJob->setText(QApplication::translate("PrintPanel", "Save to a File (Advanced)", 0, QApplication::UnicodeUTF8));
        savePrintJob->setDescription(QApplication::translate("PrintPanel", "Store this print job in a .FAB file.  This will save only the information the printer needs and not the original objects.", 0, QApplication::UnicodeUTF8));
        abandonPrintJob->setText(QApplication::translate("PrintPanel", "Start Over", 0, QApplication::UnicodeUTF8));
        abandonPrintJob->setDescription(QApplication::translate("PrintPanel", "Discard this print job and go back to loading objects and assigning materials.", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(PrintPanel);
    } // retranslateUi

};

namespace Ui {
    class PrintPanel: public Ui_PrintPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINTPANEL_H
