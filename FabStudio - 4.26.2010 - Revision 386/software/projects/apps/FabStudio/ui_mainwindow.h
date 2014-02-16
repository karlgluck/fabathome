/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Mon Apr 19 16:55:48 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionEnable_Advanced_Features;
    QAction *actionReset_Environment;
    QAction *actionVisit_FabAtHome_org;
    QAction *actionAbout_FabStudio;
    QAction *actionExit;
    QAction *actionShow_Information_at_Startup;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menuOptions;
    QMenu *menuHelp;
    QMenu *menuView;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/media/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        actionEnable_Advanced_Features = new QAction(MainWindow);
        actionEnable_Advanced_Features->setObjectName(QString::fromUtf8("actionEnable_Advanced_Features"));
        actionEnable_Advanced_Features->setCheckable(true);
        actionReset_Environment = new QAction(MainWindow);
        actionReset_Environment->setObjectName(QString::fromUtf8("actionReset_Environment"));
        actionVisit_FabAtHome_org = new QAction(MainWindow);
        actionVisit_FabAtHome_org->setObjectName(QString::fromUtf8("actionVisit_FabAtHome_org"));
        actionAbout_FabStudio = new QAction(MainWindow);
        actionAbout_FabStudio->setObjectName(QString::fromUtf8("actionAbout_FabStudio"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionShow_Information_at_Startup = new QAction(MainWindow);
        actionShow_Information_at_Startup->setObjectName(QString::fromUtf8("actionShow_Information_at_Startup"));
        actionShow_Information_at_Startup->setCheckable(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setAcceptDrops(true);
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(0);
        gridLayout->setMargin(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setFrameShape(QFrame::StyledPanel);
        graphicsView->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 22));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuOptions->addAction(actionEnable_Advanced_Features);
        menuOptions->addAction(actionShow_Information_at_Startup);
        menuHelp->addAction(actionVisit_FabAtHome_org);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout_FabStudio);
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "FabStudio", 0, QApplication::UnicodeUTF8));
        actionEnable_Advanced_Features->setText(QApplication::translate("MainWindow", "Enable Advanced Features", 0, QApplication::UnicodeUTF8));
        actionReset_Environment->setText(QApplication::translate("MainWindow", "Show Information at Startup", 0, QApplication::UnicodeUTF8));
        actionVisit_FabAtHome_org->setText(QApplication::translate("MainWindow", "Visit FabAtHome.org", 0, QApplication::UnicodeUTF8));
        actionVisit_FabAtHome_org->setShortcut(QApplication::translate("MainWindow", "F1", 0, QApplication::UnicodeUTF8));
        actionAbout_FabStudio->setText(QApplication::translate("MainWindow", "About FabStudio...", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionShow_Information_at_Startup->setText(QApplication::translate("MainWindow", "Show Information at Startup", 0, QApplication::UnicodeUTF8));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
