/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *run_cmd;
    QComboBox *select_cmd;
    QWidget *_3d_widget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        run_cmd = new QPushButton(widget);
        run_cmd->setObjectName(QString::fromUtf8("run_cmd"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(run_cmd->sizePolicy().hasHeightForWidth());
        run_cmd->setSizePolicy(sizePolicy1);
        run_cmd->setMinimumSize(QSize(120, 40));
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        run_cmd->setFont(font);

        horizontalLayout_2->addWidget(run_cmd);

        select_cmd = new QComboBox(widget);
        select_cmd->addItem(QString());
        select_cmd->addItem(QString());
        select_cmd->addItem(QString());
        select_cmd->addItem(QString());
        select_cmd->setObjectName(QString::fromUtf8("select_cmd"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(3);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(select_cmd->sizePolicy().hasHeightForWidth());
        select_cmd->setSizePolicy(sizePolicy2);
        select_cmd->setMinimumSize(QSize(360, 40));
        select_cmd->setFont(font);

        horizontalLayout_2->addWidget(select_cmd);


        verticalLayout->addWidget(widget);

        _3d_widget->setObjectName(QString::fromUtf8("_3d_widget"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(14);
        sizePolicy3.setHeightForWidth(_3d_widget->sizePolicy().hasHeightForWidth());
        _3d_widget->setSizePolicy(sizePolicy3);

        verticalLayout->addWidget(_3d_widget);


        verticalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        run_cmd->setText(QCoreApplication::translate("MainWindow", "Run", nullptr));
        select_cmd->setItemText(0, QCoreApplication::translate("MainWindow", "Shuffle the cube", nullptr));
        select_cmd->setItemText(1, QCoreApplication::translate("MainWindow", "Reset the cube", nullptr));
        select_cmd->setItemText(2, QCoreApplication::translate("MainWindow", "Restore the cube", nullptr));
        select_cmd->setItemText(3, QCoreApplication::translate("MainWindow", "Reload the program", nullptr));

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOW_H
