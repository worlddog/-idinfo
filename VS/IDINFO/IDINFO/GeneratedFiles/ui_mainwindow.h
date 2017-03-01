/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *testaction;
    QAction *exitaction;
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QLabel *image_label;
    QGroupBox *groupBox_2;
    QGroupBox *groupBox_3;
    QLabel *image_label2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLabel *label_5;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QLabel *label_6;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1114, 626);
        testaction = new QAction(MainWindow);
        testaction->setObjectName(QStringLiteral("testaction"));
        exitaction = new QAction(MainWindow);
        exitaction->setObjectName(QStringLiteral("exitaction"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 531, 321));
        image_label = new QLabel(groupBox);
        image_label->setObjectName(QStringLiteral("image_label"));
        image_label->setGeometry(QRect(10, 30, 501, 271));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 340, 751, 211));
        groupBox_3 = new QGroupBox(groupBox_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 20, 191, 171));
        image_label2 = new QLabel(groupBox_3);
        image_label2->setObjectName(QStringLiteral("image_label2"));
        image_label2->setGeometry(QRect(30, 30, 141, 121));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(290, 30, 72, 15));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(550, 30, 72, 15));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(290, 70, 72, 15));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(530, 70, 72, 15));
        lineEdit = new QLineEdit(groupBox_2);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(350, 30, 161, 21));
        lineEdit_2 = new QLineEdit(groupBox_2);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(600, 30, 113, 21));
        lineEdit_3 = new QLineEdit(groupBox_2);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(350, 60, 161, 21));
        lineEdit_4 = new QLineEdit(groupBox_2);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(600, 70, 113, 21));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(290, 100, 72, 15));
        lineEdit_5 = new QLineEdit(groupBox_2);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(360, 100, 151, 21));
        lineEdit_6 = new QLineEdit(groupBox_2);
        lineEdit_6->setObjectName(QStringLiteral("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(360, 130, 113, 21));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(290, 130, 72, 15));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(560, 20, 221, 131));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(560, 220, 201, 121));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1114, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(testaction);
        menu->addSeparator();
        menu->addAction(exitaction);

        retranslateUi(MainWindow);
        QObject::connect(exitaction, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        testaction->setText(QApplication::translate("MainWindow", "\346\265\213\350\257\225\345\267\245\345\205\267", Q_NULLPTR));
        exitaction->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "\345\233\276\345\203\217\351\242\204\350\247\210", Q_NULLPTR));
        image_label->setText(QString());
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\344\270\252\344\272\272\344\277\241\346\201\257", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\347\205\247\347\211\207", Q_NULLPTR));
        image_label2->setText(QApplication::translate("MainWindow", "\345\244\264\345\203\217\351\242\204\350\247\210", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\345\247\223\345\220\215", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\346\200\247\345\210\253", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "\346\260\221\346\227\217", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "\345\207\272\347\224\237\346\227\245\346\234\237", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "\345\256\266\345\272\255\344\275\217\345\235\200", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "\350\272\253\344\273\275\350\257\201\345\217\267", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\345\233\276\345\203\217\346\226\207\344\273\266", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "\345\206\231\345\205\245", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\351\205\215\347\275\256", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
