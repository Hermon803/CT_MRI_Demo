/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qvtkopenglnativewidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QVTKOpenGLNativeWidget *openGLWidget;
    QVTKOpenGLNativeWidget *openGLWidget_2;
    QVTKOpenGLNativeWidget *openGLWidget_3;
    QVTKOpenGLNativeWidget *openGLWidget_4;
    QStatusBar *statusbar;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1032, 793);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName("groupBox");
        groupBox->setFlat(true);
        groupBox->setCheckable(false);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        openGLWidget = new QVTKOpenGLNativeWidget(groupBox);
        openGLWidget->setObjectName("openGLWidget");

        gridLayout->addWidget(openGLWidget, 0, 0, 1, 1);

        openGLWidget_2 = new QVTKOpenGLNativeWidget(groupBox);
        openGLWidget_2->setObjectName("openGLWidget_2");

        gridLayout->addWidget(openGLWidget_2, 0, 1, 1, 1);

        openGLWidget_3 = new QVTKOpenGLNativeWidget(groupBox);
        openGLWidget_3->setObjectName("openGLWidget_3");

        gridLayout->addWidget(openGLWidget_3, 1, 0, 1, 1);

        openGLWidget_4 = new QVTKOpenGLNativeWidget(groupBox);
        openGLWidget_4->setObjectName("openGLWidget_4");

        gridLayout->addWidget(openGLWidget_4, 1, 1, 1, 1);


        verticalLayout->addWidget(groupBox);


        horizontalLayout->addWidget(widget);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1032, 25));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
