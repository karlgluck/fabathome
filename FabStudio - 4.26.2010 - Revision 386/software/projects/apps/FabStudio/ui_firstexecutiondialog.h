/********************************************************************************
** Form generated from reading ui file 'firstexecutiondialog.ui'
**
** Created: Sun Apr 18 16:53:40 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FIRSTEXECUTIONDIALOG_H
#define UI_FIRSTEXECUTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FirstExecutionDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_5;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QLabel *cameraControls;
    QVBoxLayout *verticalLayout_3;
    QCommandLinkButton *startUsingFabStudio;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *FirstExecutionDialog)
    {
        if (FirstExecutionDialog->objectName().isEmpty())
            FirstExecutionDialog->setObjectName(QString::fromUtf8("FirstExecutionDialog"));
        FirstExecutionDialog->setWindowModality(Qt::ApplicationModal);
        FirstExecutionDialog->setEnabled(true);
        FirstExecutionDialog->resize(555, 358);
        FirstExecutionDialog->setAutoFillBackground(false);
        FirstExecutionDialog->setSizeGripEnabled(false);
        FirstExecutionDialog->setModal(true);
        verticalLayout_2 = new QVBoxLayout(FirstExecutionDialog);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setMargin(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame = new QFrame(FirstExecutionDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(5);
        verticalLayout_4 = new QVBoxLayout(frame);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFrameShape(QFrame::NoFrame);
        label->setPixmap(QPixmap(QString::fromUtf8(":/media/fabstudio.png")));

        verticalLayout_5->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        cameraControls = new QLabel(groupBox);
        cameraControls->setObjectName(QString::fromUtf8("cameraControls"));
        cameraControls->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        cameraControls->setWordWrap(true);

        verticalLayout->addWidget(cameraControls);


        horizontalLayout->addWidget(groupBox);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        startUsingFabStudio = new QCommandLinkButton(frame);
        startUsingFabStudio->setObjectName(QString::fromUtf8("startUsingFabStudio"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(startUsingFabStudio->sizePolicy().hasHeightForWidth());
        startUsingFabStudio->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(startUsingFabStudio);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout_5->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout_5);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(frame);


        retranslateUi(FirstExecutionDialog);

        QMetaObject::connectSlotsByName(FirstExecutionDialog);
    } // setupUi

    void retranslateUi(QDialog *FirstExecutionDialog)
    {
        FirstExecutionDialog->setWindowTitle(QApplication::translate("FirstExecutionDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        groupBox->setTitle(QApplication::translate("FirstExecutionDialog", "Camera Controls", 0, QApplication::UnicodeUTF8));
        cameraControls->setText(QApplication::translate("FirstExecutionDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Mouse</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">With the alternate button:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">- Click to look at a location</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-inden"
                        "t:0; text-indent:0px;\"><span style=\" font-size:8pt;\">- Drag to zoom and rotate</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Keyboard</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\"> - Arrow keys move your target location</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\"> - Shift + Left/Right rotates left/right</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8"
                        "pt;\"> - Shift + Up/Down zooms in/out</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        startUsingFabStudio->setText(QApplication::translate("FirstExecutionDialog", "Start Using FabStudio", 0, QApplication::UnicodeUTF8));
        startUsingFabStudio->setDescription(QApplication::translate("FirstExecutionDialog", "Open up your workspace", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(FirstExecutionDialog);
    } // retranslateUi

};

namespace Ui {
    class FirstExecutionDialog: public Ui_FirstExecutionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIRSTEXECUTIONDIALOG_H
