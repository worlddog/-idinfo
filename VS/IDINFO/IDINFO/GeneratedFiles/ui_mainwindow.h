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
#include <QtWidgets/Qprogressbar>

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
    QLabel *label;
    QLabel *label_2;
    QLabel *label_4;
    QLineEdit *name_lineEdit;
    QLineEdit *sex_lineEdit;
    QLineEdit *date_lineEdit;
    QLabel *label_5;
    QLineEdit *add_lineEdit;
    QLineEdit *id_lineEdit;
    QLabel *label_6;
    QLineEdit *mz_lineEdit;
    QLabel *label_7;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QGroupBox *groupBox_3;
    QLabel *image_label2;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
	QProgressBar *progressBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(856, 624);
        testaction = new QAction(MainWindow);
        testaction->setObjectName(QStringLiteral("testaction"));
        exitaction = new QAction(MainWindow);
        exitaction->setObjectName(QStringLiteral("exitaction"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 481, 321));
        image_label = new QLabel(groupBox);
        image_label->setObjectName(QStringLiteral("image_label"));
        image_label->setEnabled(true);
        image_label->setGeometry(QRect(10, 30, 428, 270));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 340, 491, 191));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 30, 72, 15));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(280, 30, 72, 15));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(240, 70, 72, 15));
        name_lineEdit = new QLineEdit(groupBox_2);
        name_lineEdit->setObjectName(QStringLiteral("name_lineEdit"));
        name_lineEdit->setGeometry(QRect(80, 30, 161, 21));
        sex_lineEdit = new QLineEdit(groupBox_2);
        sex_lineEdit->setObjectName(QStringLiteral("sex_lineEdit"));
        sex_lineEdit->setGeometry(QRect(330, 30, 113, 21));
        date_lineEdit = new QLineEdit(groupBox_2);
        date_lineEdit->setObjectName(QStringLiteral("date_lineEdit"));
        date_lineEdit->setGeometry(QRect(330, 70, 113, 21));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 110, 72, 15));
        add_lineEdit = new QLineEdit(groupBox_2);
        add_lineEdit->setObjectName(QStringLiteral("add_lineEdit"));
        add_lineEdit->setGeometry(QRect(100, 100, 341, 31));
        id_lineEdit = new QLineEdit(groupBox_2);
        id_lineEdit->setObjectName(QStringLiteral("id_lineEdit"));
        id_lineEdit->setGeometry(QRect(120, 140, 321, 21));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 140, 72, 15));
        mz_lineEdit = new QLineEdit(groupBox_2);
        mz_lineEdit->setObjectName(QStringLiteral("mz_lineEdit"));
        mz_lineEdit->setGeometry(QRect(70, 70, 121, 21));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 70, 72, 15));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(520, 330, 191, 81));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(520, 420, 191, 61));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(520, 10, 281, 301));
        image_label2 = new QLabel(groupBox_3);
        image_label2->setObjectName(QStringLiteral("image_label2"));
        image_label2->setGeometry(QRect(30, 30, 208, 256));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 856, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

		progressBar = new QProgressBar(MainWindow);
	
		


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
        label->setText(QApplication::translate("MainWindow", "\345\247\223\345\220\215", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\346\200\247\345\210\253", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "\345\207\272\347\224\237\346\227\245\346\234\237", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "\345\256\266\345\272\255\344\275\217\345\235\200", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "\350\272\253\344\273\275\350\257\201\345\217\267", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "\346\260\221\346\227\217", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\345\233\276\345\203\217\346\226\207\344\273\266", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "\345\206\231\345\205\245", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\345\244\264\345\203\217\351\242\204\350\247\210", Q_NULLPTR));
        image_label2->setText(QString());
        menu->setTitle(QApplication::translate("MainWindow", "\351\205\215\347\275\256", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
