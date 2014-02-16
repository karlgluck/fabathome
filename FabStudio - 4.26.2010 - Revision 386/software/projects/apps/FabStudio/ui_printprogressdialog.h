/********************************************************************************
** Form generated from reading ui file 'printprogressdialog.ui'
**
** Created: Mon Mar 15 15:38:01 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PRINTPROGRESSDIALOG_H
#define UI_PRINTPROGRESSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PrintProgressDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *banner;
    QHBoxLayout *horizontalLayout;
    QProgressBar *progressBar;
    QPushButton *saveButton;
    QFrame *moreFrame;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *buttons;
    QProgressBar *taskProgressBar;
    QLabel *taskText;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPlainTextEdit *textBox;

    void setupUi(QDialog *PrintProgressDialog)
    {
        if (PrintProgressDialog->objectName().isEmpty())
            PrintProgressDialog->setObjectName(QString::fromUtf8("PrintProgressDialog"));
        PrintProgressDialog->setWindowModality(Qt::ApplicationModal);
        PrintProgressDialog->resize(723, 302);
        PrintProgressDialog->setModal(true);
        verticalLayout = new QVBoxLayout(PrintProgressDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        banner = new QLabel(PrintProgressDialog);
        banner->setObjectName(QString::fromUtf8("banner"));
        banner->setPixmap(QPixmap(QString::fromUtf8(":/media/banner.PNG")));

        verticalLayout->addWidget(banner);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        progressBar = new QProgressBar(PrintProgressDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);
        progressBar->setTextVisible(false);
        progressBar->setInvertedAppearance(false);

        horizontalLayout->addWidget(progressBar);

        saveButton = new QPushButton(PrintProgressDialog);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setEnabled(false);

        horizontalLayout->addWidget(saveButton);


        verticalLayout->addLayout(horizontalLayout);

        moreFrame = new QFrame(PrintProgressDialog);
        moreFrame->setObjectName(QString::fromUtf8("moreFrame"));
        moreFrame->setFrameShape(QFrame::StyledPanel);
        moreFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(moreFrame);
        verticalLayout_2->setMargin(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        buttons = new QHBoxLayout();
        buttons->setObjectName(QString::fromUtf8("buttons"));
        taskProgressBar = new QProgressBar(moreFrame);
        taskProgressBar->setObjectName(QString::fromUtf8("taskProgressBar"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(taskProgressBar->sizePolicy().hasHeightForWidth());
        taskProgressBar->setSizePolicy(sizePolicy);
        taskProgressBar->setMinimumSize(QSize(200, 0));
        taskProgressBar->setValue(24);
        taskProgressBar->setTextVisible(false);
        taskProgressBar->setOrientation(Qt::Horizontal);

        buttons->addWidget(taskProgressBar);

        taskText = new QLabel(moreFrame);
        taskText->setObjectName(QString::fromUtf8("taskText"));

        buttons->addWidget(taskText);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttons->addItem(horizontalSpacer);

        cancelButton = new QPushButton(moreFrame);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        buttons->addWidget(cancelButton);


        verticalLayout_2->addLayout(buttons);

        textBox = new QPlainTextEdit(moreFrame);
        textBox->setObjectName(QString::fromUtf8("textBox"));
        textBox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        textBox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textBox->setTabChangesFocus(true);
        textBox->setReadOnly(true);

        verticalLayout_2->addWidget(textBox);

        textBox->raise();

        verticalLayout->addWidget(moreFrame);


        retranslateUi(PrintProgressDialog);

        QMetaObject::connectSlotsByName(PrintProgressDialog);
    } // setupUi

    void retranslateUi(QDialog *PrintProgressDialog)
    {
        PrintProgressDialog->setWindowTitle(QApplication::translate("PrintProgressDialog", "Printing...", 0, QApplication::UnicodeUTF8));
        banner->setText(QString());
        saveButton->setText(QApplication::translate("PrintProgressDialog", "Save", 0, QApplication::UnicodeUTF8));
        taskText->setText(QApplication::translate("PrintProgressDialog", "Progress", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("PrintProgressDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(PrintProgressDialog);
    } // retranslateUi

};

namespace Ui {
    class PrintProgressDialog: public Ui_PrintProgressDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINTPROGRESSDIALOG_H
