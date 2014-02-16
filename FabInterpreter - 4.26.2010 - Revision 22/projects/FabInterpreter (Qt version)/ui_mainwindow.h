/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Sun Dec 6 18:29:21 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *gridGroupBox;
    QGridLayout *_2;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBox_2;
    QLineEdit *lineEdit_2;
    QSpacerItem *horizontalSpacer;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *lineEdit_9;
    QLineEdit *lineEdit_6;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEdit_7;
    QLineEdit *lineEdit_8;
    QDoubleSpinBox *doubleSpinBox_3;
    QLineEdit *lineEdit_13;
    QLineEdit *lineEdit_10;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *lineEdit_11;
    QLineEdit *lineEdit_12;
    QDoubleSpinBox *doubleSpinBox_4;
    QLineEdit *lineEdit_17;
    QLineEdit *lineEdit_14;
    QSpacerItem *horizontalSpacer_4;
    QLineEdit *lineEdit_15;
    QLineEdit *lineEdit_16;
    QDoubleSpinBox *doubleSpinBox_5;
    QLineEdit *lineEdit_21;
    QLineEdit *lineEdit_18;
    QLineEdit *lineEdit_19;
    QSpacerItem *horizontalSpacer_5;
    QLineEdit *lineEdit_20;
    QDoubleSpinBox *doubleSpinBox_6;
    QLineEdit *lineEdit_22;
    QLineEdit *lineEdit_23;
    QSpacerItem *horizontalSpacer_6;
    QLineEdit *lineEdit_24;
    QLineEdit *lineEdit_25;
    QDoubleSpinBox *doubleSpinBox_7;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QComboBox *comboBox_3;
    QLabel *label_12;
    QPushButton *pushButton_3;
    QGroupBox *gridGroupBox1;
    QGridLayout *gridLayout;
    QPushButton *loadConfigButton;
    QGroupBox *gridGroupBox2;
    QGridLayout *gridLayout_2;
    QPushButton *loadModelButton;
    QPushButton *pushButton_5;
    QLabel *label_13;
    QGroupBox *verticalGroupBox;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_6;
    QPushButton *exitButton;
    QGroupBox *gridGroupBox3;
    QGridLayout *gridLayout_3;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QProgressBar *progressBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(910, 461);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridGroupBox = new QGroupBox(centralWidget);
        gridGroupBox->setObjectName(QString::fromUtf8("gridGroupBox"));
        gridGroupBox->setGeometry(QRect(10, 80, 891, 201));
        gridGroupBox->setMinimumSize(QSize(721, 0));
        _2 = new QGridLayout(gridGroupBox);
        _2->setSpacing(6);
        _2->setMargin(11);
        _2->setObjectName(QString::fromUtf8("_2"));
        label = new QLabel(gridGroupBox);
        label->setObjectName(QString::fromUtf8("label"));

        _2->addWidget(label, 1, 0, 1, 1);

        doubleSpinBox_2 = new QDoubleSpinBox(gridGroupBox);
        doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
        doubleSpinBox_2->setEnabled(false);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(doubleSpinBox_2->sizePolicy().hasHeightForWidth());
        doubleSpinBox_2->setSizePolicy(sizePolicy);
        doubleSpinBox_2->setMinimumSize(QSize(130, 0));
        doubleSpinBox_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        doubleSpinBox_2->setDecimals(6);

        _2->addWidget(doubleSpinBox_2, 1, 6, 1, 1);

        lineEdit_2 = new QLineEdit(gridGroupBox);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_2->sizePolicy().hasHeightForWidth());
        lineEdit_2->setSizePolicy(sizePolicy);
        lineEdit_2->setMinimumSize(QSize(120, 0));
        lineEdit_2->setMaximumSize(QSize(50, 16777215));
        lineEdit_2->setFocusPolicy(Qt::NoFocus);
        lineEdit_2->setAutoFillBackground(false);
        lineEdit_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_2->setReadOnly(true);

        _2->addWidget(lineEdit_2, 1, 5, 1, 1);

        horizontalSpacer = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        _2->addItem(horizontalSpacer, 1, 3, 1, 1);

        lineEdit_3 = new QLineEdit(gridGroupBox);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_3->sizePolicy().hasHeightForWidth());
        lineEdit_3->setSizePolicy(sizePolicy);
        lineEdit_3->setMinimumSize(QSize(120, 0));
        lineEdit_3->setMaximumSize(QSize(50, 16777215));
        lineEdit_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_3, 1, 4, 1, 1);

        lineEdit_4 = new QLineEdit(gridGroupBox);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_4->sizePolicy().hasHeightForWidth());
        lineEdit_4->setSizePolicy(sizePolicy);
        lineEdit_4->setMinimumSize(QSize(120, 0));
        lineEdit_4->setMaximumSize(QSize(50, 16777215));
        lineEdit_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_4, 1, 2, 1, 1);

        lineEdit_5 = new QLineEdit(gridGroupBox);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_5->sizePolicy().hasHeightForWidth());
        lineEdit_5->setSizePolicy(sizePolicy);
        lineEdit_5->setMinimumSize(QSize(120, 0));
        lineEdit_5->setMaximumSize(QSize(50, 16777215));
        lineEdit_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_5, 1, 1, 1, 1);

        label_2 = new QLabel(gridGroupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        _2->addWidget(label_2, 2, 0, 1, 1);

        label_3 = new QLabel(gridGroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        _2->addWidget(label_3, 3, 0, 1, 1);

        label_4 = new QLabel(gridGroupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        _2->addWidget(label_4, 6, 0, 1, 1);

        label_5 = new QLabel(gridGroupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        _2->addWidget(label_5, 8, 0, 1, 1);

        label_6 = new QLabel(gridGroupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        _2->addWidget(label_6, 7, 0, 1, 1);

        lineEdit_9 = new QLineEdit(gridGroupBox);
        lineEdit_9->setObjectName(QString::fromUtf8("lineEdit_9"));
        lineEdit_9->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_9->sizePolicy().hasHeightForWidth());
        lineEdit_9->setSizePolicy(sizePolicy);
        lineEdit_9->setMinimumSize(QSize(120, 0));
        lineEdit_9->setMaximumSize(QSize(50, 16777215));
        lineEdit_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_9, 2, 1, 1, 1);

        lineEdit_6 = new QLineEdit(gridGroupBox);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_6->sizePolicy().hasHeightForWidth());
        lineEdit_6->setSizePolicy(sizePolicy);
        lineEdit_6->setMinimumSize(QSize(120, 0));
        lineEdit_6->setMaximumSize(QSize(50, 16777215));
        lineEdit_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_6, 2, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        _2->addItem(horizontalSpacer_2, 2, 3, 1, 1);

        lineEdit_7 = new QLineEdit(gridGroupBox);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        lineEdit_7->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_7->sizePolicy().hasHeightForWidth());
        lineEdit_7->setSizePolicy(sizePolicy);
        lineEdit_7->setMinimumSize(QSize(120, 0));
        lineEdit_7->setMaximumSize(QSize(50, 16777215));
        lineEdit_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_7, 2, 4, 1, 1);

        lineEdit_8 = new QLineEdit(gridGroupBox);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));
        lineEdit_8->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_8->sizePolicy().hasHeightForWidth());
        lineEdit_8->setSizePolicy(sizePolicy);
        lineEdit_8->setMinimumSize(QSize(120, 0));
        lineEdit_8->setMaximumSize(QSize(50, 16777215));
        lineEdit_8->setFocusPolicy(Qt::NoFocus);
        lineEdit_8->setAutoFillBackground(false);
        lineEdit_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_8->setReadOnly(true);

        _2->addWidget(lineEdit_8, 2, 5, 1, 1);

        doubleSpinBox_3 = new QDoubleSpinBox(gridGroupBox);
        doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));
        doubleSpinBox_3->setEnabled(false);
        sizePolicy.setHeightForWidth(doubleSpinBox_3->sizePolicy().hasHeightForWidth());
        doubleSpinBox_3->setSizePolicy(sizePolicy);
        doubleSpinBox_3->setMinimumSize(QSize(130, 0));
        doubleSpinBox_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        doubleSpinBox_3->setDecimals(6);

        _2->addWidget(doubleSpinBox_3, 2, 6, 1, 1);

        lineEdit_13 = new QLineEdit(gridGroupBox);
        lineEdit_13->setObjectName(QString::fromUtf8("lineEdit_13"));
        lineEdit_13->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_13->sizePolicy().hasHeightForWidth());
        lineEdit_13->setSizePolicy(sizePolicy);
        lineEdit_13->setMinimumSize(QSize(120, 0));
        lineEdit_13->setMaximumSize(QSize(50, 16777215));
        lineEdit_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_13, 3, 1, 1, 1);

        lineEdit_10 = new QLineEdit(gridGroupBox);
        lineEdit_10->setObjectName(QString::fromUtf8("lineEdit_10"));
        lineEdit_10->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_10->sizePolicy().hasHeightForWidth());
        lineEdit_10->setSizePolicy(sizePolicy);
        lineEdit_10->setMinimumSize(QSize(120, 0));
        lineEdit_10->setMaximumSize(QSize(50, 16777215));
        lineEdit_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_10, 3, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        _2->addItem(horizontalSpacer_3, 3, 3, 1, 1);

        lineEdit_11 = new QLineEdit(gridGroupBox);
        lineEdit_11->setObjectName(QString::fromUtf8("lineEdit_11"));
        lineEdit_11->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_11->sizePolicy().hasHeightForWidth());
        lineEdit_11->setSizePolicy(sizePolicy);
        lineEdit_11->setMinimumSize(QSize(120, 0));
        lineEdit_11->setMaximumSize(QSize(50, 16777215));
        lineEdit_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_11, 3, 4, 1, 1);

        lineEdit_12 = new QLineEdit(gridGroupBox);
        lineEdit_12->setObjectName(QString::fromUtf8("lineEdit_12"));
        lineEdit_12->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_12->sizePolicy().hasHeightForWidth());
        lineEdit_12->setSizePolicy(sizePolicy);
        lineEdit_12->setMinimumSize(QSize(120, 0));
        lineEdit_12->setMaximumSize(QSize(50, 16777215));
        lineEdit_12->setFocusPolicy(Qt::NoFocus);
        lineEdit_12->setAutoFillBackground(false);
        lineEdit_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_12->setReadOnly(true);

        _2->addWidget(lineEdit_12, 3, 5, 1, 1);

        doubleSpinBox_4 = new QDoubleSpinBox(gridGroupBox);
        doubleSpinBox_4->setObjectName(QString::fromUtf8("doubleSpinBox_4"));
        doubleSpinBox_4->setEnabled(false);
        sizePolicy.setHeightForWidth(doubleSpinBox_4->sizePolicy().hasHeightForWidth());
        doubleSpinBox_4->setSizePolicy(sizePolicy);
        doubleSpinBox_4->setMinimumSize(QSize(130, 0));
        doubleSpinBox_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        doubleSpinBox_4->setDecimals(6);

        _2->addWidget(doubleSpinBox_4, 3, 6, 1, 1);

        lineEdit_17 = new QLineEdit(gridGroupBox);
        lineEdit_17->setObjectName(QString::fromUtf8("lineEdit_17"));
        lineEdit_17->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_17->sizePolicy().hasHeightForWidth());
        lineEdit_17->setSizePolicy(sizePolicy);
        lineEdit_17->setMinimumSize(QSize(120, 0));
        lineEdit_17->setMaximumSize(QSize(50, 16777215));
        lineEdit_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_17, 6, 1, 1, 1);

        lineEdit_14 = new QLineEdit(gridGroupBox);
        lineEdit_14->setObjectName(QString::fromUtf8("lineEdit_14"));
        lineEdit_14->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_14->sizePolicy().hasHeightForWidth());
        lineEdit_14->setSizePolicy(sizePolicy);
        lineEdit_14->setMinimumSize(QSize(120, 0));
        lineEdit_14->setMaximumSize(QSize(50, 16777215));
        lineEdit_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_14, 6, 2, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        _2->addItem(horizontalSpacer_4, 6, 3, 1, 1);

        lineEdit_15 = new QLineEdit(gridGroupBox);
        lineEdit_15->setObjectName(QString::fromUtf8("lineEdit_15"));
        lineEdit_15->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_15->sizePolicy().hasHeightForWidth());
        lineEdit_15->setSizePolicy(sizePolicy);
        lineEdit_15->setMinimumSize(QSize(120, 0));
        lineEdit_15->setMaximumSize(QSize(50, 16777215));
        lineEdit_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_15, 6, 4, 1, 1);

        lineEdit_16 = new QLineEdit(gridGroupBox);
        lineEdit_16->setObjectName(QString::fromUtf8("lineEdit_16"));
        lineEdit_16->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_16->sizePolicy().hasHeightForWidth());
        lineEdit_16->setSizePolicy(sizePolicy);
        lineEdit_16->setMinimumSize(QSize(120, 0));
        lineEdit_16->setMaximumSize(QSize(50, 16777215));
        lineEdit_16->setFocusPolicy(Qt::NoFocus);
        lineEdit_16->setAutoFillBackground(false);
        lineEdit_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_16->setReadOnly(true);

        _2->addWidget(lineEdit_16, 6, 5, 1, 1);

        doubleSpinBox_5 = new QDoubleSpinBox(gridGroupBox);
        doubleSpinBox_5->setObjectName(QString::fromUtf8("doubleSpinBox_5"));
        doubleSpinBox_5->setEnabled(false);
        sizePolicy.setHeightForWidth(doubleSpinBox_5->sizePolicy().hasHeightForWidth());
        doubleSpinBox_5->setSizePolicy(sizePolicy);
        doubleSpinBox_5->setMinimumSize(QSize(130, 0));
        doubleSpinBox_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        doubleSpinBox_5->setDecimals(6);

        _2->addWidget(doubleSpinBox_5, 6, 6, 1, 1);

        lineEdit_21 = new QLineEdit(gridGroupBox);
        lineEdit_21->setObjectName(QString::fromUtf8("lineEdit_21"));
        lineEdit_21->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_21->sizePolicy().hasHeightForWidth());
        lineEdit_21->setSizePolicy(sizePolicy);
        lineEdit_21->setMinimumSize(QSize(120, 0));
        lineEdit_21->setMaximumSize(QSize(50, 16777215));
        lineEdit_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_21, 7, 1, 1, 1);

        lineEdit_18 = new QLineEdit(gridGroupBox);
        lineEdit_18->setObjectName(QString::fromUtf8("lineEdit_18"));
        lineEdit_18->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_18->sizePolicy().hasHeightForWidth());
        lineEdit_18->setSizePolicy(sizePolicy);
        lineEdit_18->setMinimumSize(QSize(120, 0));
        lineEdit_18->setMaximumSize(QSize(50, 16777215));
        lineEdit_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_18, 7, 2, 1, 1);

        lineEdit_19 = new QLineEdit(gridGroupBox);
        lineEdit_19->setObjectName(QString::fromUtf8("lineEdit_19"));
        lineEdit_19->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_19->sizePolicy().hasHeightForWidth());
        lineEdit_19->setSizePolicy(sizePolicy);
        lineEdit_19->setMinimumSize(QSize(120, 0));
        lineEdit_19->setMaximumSize(QSize(50, 16777215));
        lineEdit_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_19, 7, 4, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        _2->addItem(horizontalSpacer_5, 7, 3, 1, 1);

        lineEdit_20 = new QLineEdit(gridGroupBox);
        lineEdit_20->setObjectName(QString::fromUtf8("lineEdit_20"));
        lineEdit_20->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_20->sizePolicy().hasHeightForWidth());
        lineEdit_20->setSizePolicy(sizePolicy);
        lineEdit_20->setMinimumSize(QSize(120, 0));
        lineEdit_20->setMaximumSize(QSize(50, 16777215));
        lineEdit_20->setFocusPolicy(Qt::NoFocus);
        lineEdit_20->setAutoFillBackground(false);
        lineEdit_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_20->setReadOnly(true);

        _2->addWidget(lineEdit_20, 7, 5, 1, 1);

        doubleSpinBox_6 = new QDoubleSpinBox(gridGroupBox);
        doubleSpinBox_6->setObjectName(QString::fromUtf8("doubleSpinBox_6"));
        doubleSpinBox_6->setEnabled(false);
        sizePolicy.setHeightForWidth(doubleSpinBox_6->sizePolicy().hasHeightForWidth());
        doubleSpinBox_6->setSizePolicy(sizePolicy);
        doubleSpinBox_6->setMinimumSize(QSize(130, 0));
        doubleSpinBox_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        doubleSpinBox_6->setDecimals(6);

        _2->addWidget(doubleSpinBox_6, 7, 6, 1, 1);

        lineEdit_22 = new QLineEdit(gridGroupBox);
        lineEdit_22->setObjectName(QString::fromUtf8("lineEdit_22"));
        lineEdit_22->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_22->sizePolicy().hasHeightForWidth());
        lineEdit_22->setSizePolicy(sizePolicy);
        lineEdit_22->setMinimumSize(QSize(120, 0));
        lineEdit_22->setMaximumSize(QSize(50, 16777215));
        lineEdit_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_22, 8, 1, 1, 1);

        lineEdit_23 = new QLineEdit(gridGroupBox);
        lineEdit_23->setObjectName(QString::fromUtf8("lineEdit_23"));
        lineEdit_23->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_23->sizePolicy().hasHeightForWidth());
        lineEdit_23->setSizePolicy(sizePolicy);
        lineEdit_23->setMinimumSize(QSize(120, 0));
        lineEdit_23->setMaximumSize(QSize(50, 16777215));
        lineEdit_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_23, 8, 2, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        _2->addItem(horizontalSpacer_6, 8, 3, 1, 1);

        lineEdit_24 = new QLineEdit(gridGroupBox);
        lineEdit_24->setObjectName(QString::fromUtf8("lineEdit_24"));
        lineEdit_24->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_24->sizePolicy().hasHeightForWidth());
        lineEdit_24->setSizePolicy(sizePolicy);
        lineEdit_24->setMinimumSize(QSize(120, 0));
        lineEdit_24->setMaximumSize(QSize(50, 16777215));
        lineEdit_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(lineEdit_24, 8, 4, 1, 1);

        lineEdit_25 = new QLineEdit(gridGroupBox);
        lineEdit_25->setObjectName(QString::fromUtf8("lineEdit_25"));
        lineEdit_25->setEnabled(false);
        sizePolicy.setHeightForWidth(lineEdit_25->sizePolicy().hasHeightForWidth());
        lineEdit_25->setSizePolicy(sizePolicy);
        lineEdit_25->setMinimumSize(QSize(120, 0));
        lineEdit_25->setMaximumSize(QSize(50, 16777215));
        QFont font;
        font.setUnderline(false);
        lineEdit_25->setFont(font);
        lineEdit_25->setFocusPolicy(Qt::NoFocus);
        lineEdit_25->setAutoFillBackground(false);
        lineEdit_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_25->setDragEnabled(false);
        lineEdit_25->setReadOnly(true);

        _2->addWidget(lineEdit_25, 8, 5, 1, 1);

        doubleSpinBox_7 = new QDoubleSpinBox(gridGroupBox);
        doubleSpinBox_7->setObjectName(QString::fromUtf8("doubleSpinBox_7"));
        doubleSpinBox_7->setEnabled(false);
        sizePolicy.setHeightForWidth(doubleSpinBox_7->sizePolicy().hasHeightForWidth());
        doubleSpinBox_7->setSizePolicy(sizePolicy);
        doubleSpinBox_7->setMinimumSize(QSize(130, 0));
        doubleSpinBox_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        doubleSpinBox_7->setDecimals(6);
        doubleSpinBox_7->setMinimum(-100);
        doubleSpinBox_7->setMaximum(100);
        doubleSpinBox_7->setSingleStep(1);

        _2->addWidget(doubleSpinBox_7, 8, 6, 1, 1);

        label_7 = new QLabel(gridGroupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        _2->addWidget(label_7, 0, 1, 1, 1);

        label_8 = new QLabel(gridGroupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        _2->addWidget(label_8, 0, 2, 1, 1);

        label_9 = new QLabel(gridGroupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        _2->addWidget(label_9, 0, 4, 1, 1);

        label_10 = new QLabel(gridGroupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        _2->addWidget(label_10, 0, 5, 1, 1);

        label_11 = new QLabel(gridGroupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        _2->addWidget(label_11, 0, 6, 1, 1);

        comboBox = new QComboBox(gridGroupBox);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setEnabled(false);
        sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy);
        comboBox->setMinimumSize(QSize(140, 0));

        _2->addWidget(comboBox, 6, 7, 1, 1);

        comboBox_2 = new QComboBox(gridGroupBox);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setEnabled(false);
        sizePolicy.setHeightForWidth(comboBox_2->sizePolicy().hasHeightForWidth());
        comboBox_2->setSizePolicy(sizePolicy);
        comboBox_2->setMinimumSize(QSize(140, 0));

        _2->addWidget(comboBox_2, 7, 7, 1, 1);

        comboBox_3 = new QComboBox(gridGroupBox);
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));
        comboBox_3->setEnabled(false);
        sizePolicy.setHeightForWidth(comboBox_3->sizePolicy().hasHeightForWidth());
        comboBox_3->setSizePolicy(sizePolicy);
        comboBox_3->setMinimumSize(QSize(140, 0));

        _2->addWidget(comboBox_3, 8, 7, 1, 1);

        label_12 = new QLabel(gridGroupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setAlignment(Qt::AlignCenter);

        _2->addWidget(label_12, 3, 7, 1, 1);

        pushButton_3 = new QPushButton(gridGroupBox);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setEnabled(false);
        QFont font1;
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setWeight(75);
        pushButton_3->setFont(font1);

        _2->addWidget(pushButton_3, 1, 7, 2, 1);

        gridGroupBox1 = new QGroupBox(centralWidget);
        gridGroupBox1->setObjectName(QString::fromUtf8("gridGroupBox1"));
        gridGroupBox1->setGeometry(QRect(10, 10, 391, 61));
        gridLayout = new QGridLayout(gridGroupBox1);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        loadConfigButton = new QPushButton(gridGroupBox1);
        loadConfigButton->setObjectName(QString::fromUtf8("loadConfigButton"));
        QFont font2;
        font2.setPointSize(18);
        font2.setBold(true);
        font2.setWeight(75);
        loadConfigButton->setFont(font2);

        gridLayout->addWidget(loadConfigButton, 0, 1, 1, 1);

        gridGroupBox2 = new QGroupBox(centralWidget);
        gridGroupBox2->setObjectName(QString::fromUtf8("gridGroupBox2"));
        gridGroupBox2->setGeometry(QRect(10, 290, 711, 51));
        gridLayout_2 = new QGridLayout(gridGroupBox2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setMargin(11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        loadModelButton = new QPushButton(gridGroupBox2);
        loadModelButton->setObjectName(QString::fromUtf8("loadModelButton"));
        loadModelButton->setEnabled(false);
        sizePolicy.setHeightForWidth(loadModelButton->sizePolicy().hasHeightForWidth());
        loadModelButton->setSizePolicy(sizePolicy);
        loadModelButton->setMinimumSize(QSize(120, 0));
        QFont font3;
        font3.setPointSize(16);
        font3.setBold(false);
        font3.setWeight(50);
        loadModelButton->setFont(font3);

        gridLayout_2->addWidget(loadModelButton, 0, 0, 1, 1);

        pushButton_5 = new QPushButton(gridGroupBox2);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setEnabled(false);
        sizePolicy.setHeightForWidth(pushButton_5->sizePolicy().hasHeightForWidth());
        pushButton_5->setSizePolicy(sizePolicy);
        pushButton_5->setMinimumSize(QSize(120, 0));
        pushButton_5->setFont(font3);

        gridLayout_2->addWidget(pushButton_5, 0, 1, 1, 1);

        label_13 = new QLabel(gridGroupBox2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setAutoFillBackground(true);

        gridLayout_2->addWidget(label_13, 0, 2, 1, 1);

        verticalGroupBox = new QGroupBox(centralWidget);
        verticalGroupBox->setObjectName(QString::fromUtf8("verticalGroupBox"));
        verticalGroupBox->setGeometry(QRect(740, 290, 151, 141));
        verticalLayout = new QVBoxLayout(verticalGroupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton_6 = new QPushButton(verticalGroupBox);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
        pushButton_6->setSizePolicy(sizePolicy1);
        pushButton_6->setMinimumSize(QSize(0, 50));
        QFont font4;
        font4.setPointSize(16);
        font4.setBold(true);
        font4.setWeight(75);
        pushButton_6->setFont(font4);

        verticalLayout->addWidget(pushButton_6);

        exitButton = new QPushButton(verticalGroupBox);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        sizePolicy1.setHeightForWidth(exitButton->sizePolicy().hasHeightForWidth());
        exitButton->setSizePolicy(sizePolicy1);
        exitButton->setMinimumSize(QSize(0, 70));
        QFont font5;
        font5.setPointSize(24);
        font5.setBold(true);
        font5.setWeight(75);
        exitButton->setFont(font5);

        verticalLayout->addWidget(exitButton);

        gridGroupBox3 = new QGroupBox(centralWidget);
        gridGroupBox3->setObjectName(QString::fromUtf8("gridGroupBox3"));
        gridGroupBox3->setGeometry(QRect(10, 350, 711, 81));
        gridLayout_3 = new QGridLayout(gridGroupBox3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setMargin(11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        pushButton_8 = new QPushButton(gridGroupBox3);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setEnabled(false);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_8->sizePolicy().hasHeightForWidth());
        pushButton_8->setSizePolicy(sizePolicy2);
        pushButton_8->setMinimumSize(QSize(120, 0));
        pushButton_8->setFont(font3);

        gridLayout_3->addWidget(pushButton_8, 0, 0, 1, 1);

        pushButton_9 = new QPushButton(gridGroupBox3);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setEnabled(false);
        sizePolicy2.setHeightForWidth(pushButton_9->sizePolicy().hasHeightForWidth());
        pushButton_9->setSizePolicy(sizePolicy2);
        pushButton_9->setMinimumSize(QSize(120, 0));
        pushButton_9->setFont(font3);

        gridLayout_3->addWidget(pushButton_9, 0, 1, 1, 1);

        pushButton_10 = new QPushButton(gridGroupBox3);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));
        pushButton_10->setEnabled(false);
        sizePolicy2.setHeightForWidth(pushButton_10->sizePolicy().hasHeightForWidth());
        pushButton_10->setSizePolicy(sizePolicy2);
        pushButton_10->setMinimumSize(QSize(120, 0));
        pushButton_10->setFont(font3);

        gridLayout_3->addWidget(pushButton_10, 0, 2, 1, 1);

        progressBar = new QProgressBar(gridGroupBox3);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);

        gridLayout_3->addWidget(progressBar, 1, 0, 1, 3);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "FAB@Home Interpreter", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Not Loaded", 0, QApplication::UnicodeUTF8));
        lineEdit_2->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_3->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_4->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_5->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Not Loaded", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Not Loaded", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Not Loaded", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Not Loaded", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Not Loaded", 0, QApplication::UnicodeUTF8));
        lineEdit_9->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_6->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_7->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_8->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_13->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_10->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_11->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_12->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_17->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_14->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_15->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_16->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_21->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_18->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_19->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_20->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_22->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_23->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_24->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        lineEdit_25->setInputMask(QString());
        lineEdit_25->setText(QApplication::translate("MainWindow", "0.0", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Velocity (mm/s)", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Acceleration (mm/s)", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "Position Increment (mm)", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "Motor Position (mm)", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Commanded Position (mm)", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "Material Calibration", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("MainWindow", "Reset Position", 0, QApplication::UnicodeUTF8));
        loadConfigButton->setText(QApplication::translate("MainWindow", "Load Printer Configuration", 0, QApplication::UnicodeUTF8));
        loadModelButton->setText(QApplication::translate("MainWindow", "Load Model", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("MainWindow", "Execute", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MainWindow", "Path Information Display", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("MainWindow", "Redo Path", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        pushButton_8->setText(QApplication::translate("MainWindow", "Pause", 0, QApplication::UnicodeUTF8));
        pushButton_9->setText(QApplication::translate("MainWindow", "Cancel", 0, QApplication::UnicodeUTF8));
        pushButton_10->setText(QApplication::translate("MainWindow", "Force Stop", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
