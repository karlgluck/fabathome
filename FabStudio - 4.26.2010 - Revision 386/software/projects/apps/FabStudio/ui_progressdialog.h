/********************************************************************************
** Form generated from reading ui file 'progressdialog.ui'
**
** Created: Mon Mar 15 15:38:01 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PROGRESSDIALOG_H
#define UI_PROGRESSDIALOG_H

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

class Ui_ProgressDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *banner;
    QHBoxLayout *horizontalLayout;
    QProgressBar *progressBar;
    QPushButton *moreButton;
    QFrame *moreFrame;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *buttons;
    QProgressBar *taskProgressBar;
    QLabel *progressText;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPlainTextEdit *textBox;

    void setupUi(QDialog *ProgressDialog)
    {
        if (ProgressDialog->objectName().isEmpty())
            ProgressDialog->setObjectName(QString::fromUtf8("ProgressDialog"));
        ProgressDialog->setWindowModality(Qt::ApplicationModal);
        ProgressDialog->resize(723, 361);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ProgressDialog->sizePolicy().hasHeightForWidth());
        ProgressDialog->setSizePolicy(sizePolicy);
        ProgressDialog->setModal(true);
        verticalLayout = new QVBoxLayout(ProgressDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        banner = new QLabel(ProgressDialog);
        banner->setObjectName(QString::fromUtf8("banner"));
        banner->setPixmap(QPixmap(QString::fromUtf8(":/media/banner.PNG")));

        verticalLayout->addWidget(banner);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        progressBar = new QProgressBar(ProgressDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);
        progressBar->setTextVisible(false);
        progressBar->setInvertedAppearance(false);

        horizontalLayout->addWidget(progressBar);

        moreButton = new QPushButton(ProgressDialog);
        moreButton->setObjectName(QString::fromUtf8("moreButton"));

        horizontalLayout->addWidget(moreButton);


        verticalLayout->addLayout(horizontalLayout);

        moreFrame = new QFrame(ProgressDialog);
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
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(taskProgressBar->sizePolicy().hasHeightForWidth());
        taskProgressBar->setSizePolicy(sizePolicy1);
        taskProgressBar->setMinimumSize(QSize(200, 0));
        taskProgressBar->setValue(24);
        taskProgressBar->setTextVisible(false);
        taskProgressBar->setOrientation(Qt::Horizontal);

        buttons->addWidget(taskProgressBar);

        progressText = new QLabel(moreFrame);
        progressText->setObjectName(QString::fromUtf8("progressText"));

        buttons->addWidget(progressText);

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


        retranslateUi(ProgressDialog);

        QMetaObject::connectSlotsByName(ProgressDialog);
    } // setupUi

    void retranslateUi(QDialog *ProgressDialog)
    {
        ProgressDialog->setWindowTitle(QApplication::translate("ProgressDialog", "Printing...", 0, QApplication::UnicodeUTF8));
        banner->setText(QString());
        moreButton->setText(QApplication::translate("ProgressDialog", "More", 0, QApplication::UnicodeUTF8));
        progressText->setText(QApplication::translate("ProgressDialog", "Progress", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("ProgressDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ProgressDialog);
    } // retranslateUi

};

namespace Ui {
    class ProgressDialog: public Ui_ProgressDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESSDIALOG_H
