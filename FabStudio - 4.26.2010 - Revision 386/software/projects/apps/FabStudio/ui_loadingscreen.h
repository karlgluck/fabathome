/********************************************************************************
** Form generated from reading ui file 'loadingscreen.ui'
**
** Created: Sun Mar 7 15:01:35 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LOADINGSCREEN_H
#define UI_LOADINGSCREEN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LoadingScreen
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *banner;
    QProgressBar *progressBar;

    void setupUi(QDialog *LoadingScreen)
    {
        if (LoadingScreen->objectName().isEmpty())
            LoadingScreen->setObjectName(QString::fromUtf8("LoadingScreen"));
        LoadingScreen->resize(723, 123);
        verticalLayout = new QVBoxLayout(LoadingScreen);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        banner = new QLabel(LoadingScreen);
        banner->setObjectName(QString::fromUtf8("banner"));
        banner->setPixmap(QPixmap(QString::fromUtf8(":/media/banner.PNG")));

        verticalLayout->addWidget(banner);

        progressBar = new QProgressBar(LoadingScreen);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);
        progressBar->setTextVisible(false);
        progressBar->setInvertedAppearance(false);

        verticalLayout->addWidget(progressBar);


        retranslateUi(LoadingScreen);

        QMetaObject::connectSlotsByName(LoadingScreen);
    } // setupUi

    void retranslateUi(QDialog *LoadingScreen)
    {
        LoadingScreen->setWindowTitle(QApplication::translate("LoadingScreen", "Dialog", 0, QApplication::UnicodeUTF8));
        banner->setText(QString());
        Q_UNUSED(LoadingScreen);
    } // retranslateUi

};

namespace Ui {
    class LoadingScreen: public Ui_LoadingScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADINGSCREEN_H
