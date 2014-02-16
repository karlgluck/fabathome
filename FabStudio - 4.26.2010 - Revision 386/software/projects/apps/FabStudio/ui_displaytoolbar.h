/********************************************************************************
** Form generated from reading ui file 'displaytoolbar.ui'
**
** Created: Sun Apr 4 09:56:39 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DISPLAYTOOLBAR_H
#define UI_DISPLAYTOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DisplayToolbar
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QCheckBox *displayObjects;
    QCheckBox *displaySlices;
    QCheckBox *displayPaths;
    QFrame *line;
    QPushButton *pushButton;

    void setupUi(QWidget *DisplayToolbar)
    {
        if (DisplayToolbar->objectName().isEmpty())
            DisplayToolbar->setObjectName(QString::fromUtf8("DisplayToolbar"));
        DisplayToolbar->resize(432, 72);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DisplayToolbar->sizePolicy().hasHeightForWidth());
        DisplayToolbar->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(DisplayToolbar);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(10, 0, 6, 0);
        label = new QLabel(DisplayToolbar);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(60, 0));

        horizontalLayout->addWidget(label);

        displayObjects = new QCheckBox(DisplayToolbar);
        displayObjects->setObjectName(QString::fromUtf8("displayObjects"));
        displayObjects->setEnabled(true);
        displayObjects->setChecked(true);

        horizontalLayout->addWidget(displayObjects);

        displaySlices = new QCheckBox(DisplayToolbar);
        displaySlices->setObjectName(QString::fromUtf8("displaySlices"));
        displaySlices->setEnabled(true);
        displaySlices->setChecked(true);

        horizontalLayout->addWidget(displaySlices);

        displayPaths = new QCheckBox(DisplayToolbar);
        displayPaths->setObjectName(QString::fromUtf8("displayPaths"));
        displayPaths->setEnabled(true);
        displayPaths->setChecked(true);

        horizontalLayout->addWidget(displayPaths);

        line = new QFrame(DisplayToolbar);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        pushButton = new QPushButton(DisplayToolbar);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/media/positioning-tool.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon);
        pushButton->setIconSize(QSize(64, 64));

        horizontalLayout->addWidget(pushButton);


        retranslateUi(DisplayToolbar);

        QMetaObject::connectSlotsByName(DisplayToolbar);
    } // setupUi

    void retranslateUi(QWidget *DisplayToolbar)
    {
        DisplayToolbar->setWindowTitle(QApplication::translate("DisplayToolbar", "Display Toolbar", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DisplayToolbar", "Display:", 0, QApplication::UnicodeUTF8));
        displayObjects->setText(QApplication::translate("DisplayToolbar", "Objects", 0, QApplication::UnicodeUTF8));
        displaySlices->setText(QApplication::translate("DisplayToolbar", "Slices", 0, QApplication::UnicodeUTF8));
        displayPaths->setText(QApplication::translate("DisplayToolbar", "Paths", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QString());
        Q_UNUSED(DisplayToolbar);
    } // retranslateUi

};

namespace Ui {
    class DisplayToolbar: public Ui_DisplayToolbar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYTOOLBAR_H
