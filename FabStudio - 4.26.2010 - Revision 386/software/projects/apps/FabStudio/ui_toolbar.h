/********************************************************************************
** Form generated from reading ui file 'toolbar.ui'
**
** Created: Sun Apr 18 16:53:40 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TOOLBAR_H
#define UI_TOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Toolbar
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *loadObjects;
    QFrame *line_2;
    QPushButton *selectMaterials;
    QFrame *line_3;
    QPushButton *sendToPrinter;

    void setupUi(QWidget *Toolbar)
    {
        if (Toolbar->objectName().isEmpty())
            Toolbar->setObjectName(QString::fromUtf8("Toolbar"));
        Toolbar->resize(663, 31);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Toolbar->sizePolicy().hasHeightForWidth());
        Toolbar->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(Toolbar);
        horizontalLayout->setSpacing(3);
        horizontalLayout->setMargin(3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        loadObjects = new QPushButton(Toolbar);
        loadObjects->setObjectName(QString::fromUtf8("loadObjects"));
        loadObjects->setFlat(true);

        horizontalLayout->addWidget(loadObjects);

        line_2 = new QFrame(Toolbar);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line_2);

        selectMaterials = new QPushButton(Toolbar);
        selectMaterials->setObjectName(QString::fromUtf8("selectMaterials"));
        selectMaterials->setFlat(true);

        horizontalLayout->addWidget(selectMaterials);

        line_3 = new QFrame(Toolbar);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line_3);

        sendToPrinter = new QPushButton(Toolbar);
        sendToPrinter->setObjectName(QString::fromUtf8("sendToPrinter"));
        sendToPrinter->setFlat(true);

        horizontalLayout->addWidget(sendToPrinter);


        retranslateUi(Toolbar);

        QMetaObject::connectSlotsByName(Toolbar);
    } // setupUi

    void retranslateUi(QWidget *Toolbar)
    {
        Toolbar->setWindowTitle(QApplication::translate("Toolbar", "Toolbar", 0, QApplication::UnicodeUTF8));
        loadObjects->setText(QApplication::translate("Toolbar", "Load Objects", 0, QApplication::UnicodeUTF8));
        selectMaterials->setText(QApplication::translate("Toolbar", "Select materials for all objects", 0, QApplication::UnicodeUTF8));
        sendToPrinter->setText(QApplication::translate("Toolbar", "Send to printer!", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Toolbar);
    } // retranslateUi

};

namespace Ui {
    class Toolbar: public Ui_Toolbar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLBAR_H
