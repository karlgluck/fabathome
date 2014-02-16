/********************************************************************************
** Form generated from reading ui file 'loadobjectspanel.ui'
**
** Created: Sun Apr 18 16:53:40 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LOADOBJECTSPANEL_H
#define UI_LOADOBJECTSPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoadObjectsPanel
{
public:
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *fileNameTypeahead;
    QPushButton *clearFileNameTypeahead;
    QLabel *label_2;
    QListWidget *objectsList;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *rescan;
    QPushButton *browse;

    void setupUi(QDockWidget *LoadObjectsPanel)
    {
        if (LoadObjectsPanel->objectName().isEmpty())
            LoadObjectsPanel->setObjectName(QString::fromUtf8("LoadObjectsPanel"));
        LoadObjectsPanel->setWindowModality(Qt::NonModal);
        LoadObjectsPanel->resize(250, 508);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LoadObjectsPanel->sizePolicy().hasHeightForWidth());
        LoadObjectsPanel->setSizePolicy(sizePolicy);
        LoadObjectsPanel->setMinimumSize(QSize(250, 420));
        LoadObjectsPanel->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        LoadObjectsPanel->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setMargin(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(dockWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        fileNameTypeahead = new QLineEdit(dockWidgetContents);
        fileNameTypeahead->setObjectName(QString::fromUtf8("fileNameTypeahead"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fileNameTypeahead->sizePolicy().hasHeightForWidth());
        fileNameTypeahead->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(fileNameTypeahead);

        clearFileNameTypeahead = new QPushButton(dockWidgetContents);
        clearFileNameTypeahead->setObjectName(QString::fromUtf8("clearFileNameTypeahead"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(clearFileNameTypeahead->sizePolicy().hasHeightForWidth());
        clearFileNameTypeahead->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(clearFileNameTypeahead);


        verticalLayout->addLayout(horizontalLayout);

        label_2 = new QLabel(dockWidgetContents);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        objectsList = new QListWidget(dockWidgetContents);
        objectsList->setObjectName(QString::fromUtf8("objectsList"));
        objectsList->setDragEnabled(true);
        objectsList->setDragDropMode(QAbstractItemView::DragOnly);

        verticalLayout->addWidget(objectsList);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        rescan = new QPushButton(dockWidgetContents);
        rescan->setObjectName(QString::fromUtf8("rescan"));
        rescan->setEnabled(false);

        horizontalLayout_2->addWidget(rescan);

        browse = new QPushButton(dockWidgetContents);
        browse->setObjectName(QString::fromUtf8("browse"));

        horizontalLayout_2->addWidget(browse);


        verticalLayout->addLayout(horizontalLayout_2);

        LoadObjectsPanel->setWidget(dockWidgetContents);

        retranslateUi(LoadObjectsPanel);

        QMetaObject::connectSlotsByName(LoadObjectsPanel);
    } // setupUi

    void retranslateUi(QDockWidget *LoadObjectsPanel)
    {
        LoadObjectsPanel->setWindowTitle(QApplication::translate("LoadObjectsPanel", "Load Objects", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("LoadObjectsPanel", "Begin typing the file name:", 0, QApplication::UnicodeUTF8));
        clearFileNameTypeahead->setText(QApplication::translate("LoadObjectsPanel", "clear", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("LoadObjectsPanel", "Drag objects from here to the workspace:", 0, QApplication::UnicodeUTF8));
        rescan->setText(QApplication::translate("LoadObjectsPanel", "Re-Scan", 0, QApplication::UnicodeUTF8));
        browse->setText(QApplication::translate("LoadObjectsPanel", "Browse...", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(LoadObjectsPanel);
    } // retranslateUi

};

namespace Ui {
    class LoadObjectsPanel: public Ui_LoadObjectsPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADOBJECTSPANEL_H
