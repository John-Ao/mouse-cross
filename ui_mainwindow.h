/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.0.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label_1;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(30, 30);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label_1 = new QLabel(centralwidget);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        label_1->setGeometry(QRect(-1, -1, 30, 30));
        QFont font;
        font.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font.setPointSize(28);
        label_1->setFont(font);
        label_1->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(-1, 1, 30, 30));
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8(""));
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(1, -1, 30, 30));
        label_3->setFont(font);
        label_3->setStyleSheet(QString::fromUtf8(""));
        label_3->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(1, 1, 30, 30));
        label_4->setFont(font);
        label_4->setStyleSheet(QString::fromUtf8(""));
        label_4->setAlignment(Qt::AlignCenter);
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(0, 0, 30, 30));
        label_5->setFont(font);
        label_5->setStyleSheet(QString::fromUtf8("QLabel {color: white;}"));
        label_5->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_1->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
