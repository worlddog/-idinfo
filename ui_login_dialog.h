/********************************************************************************
** Form generated from reading UI file 'login_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_DIALOG_H
#define UI_LOGIN_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Login_Dialog
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;

    void setupUi(QDialog *Login_Dialog)
    {
        if (Login_Dialog->objectName().isEmpty())
            Login_Dialog->setObjectName(QStringLiteral("Login_Dialog"));
        Login_Dialog->resize(400, 300);
        pushButton = new QPushButton(Login_Dialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(270, 50, 93, 28));
        pushButton_2 = new QPushButton(Login_Dialog);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(270, 110, 93, 28));
        lineEdit = new QLineEdit(Login_Dialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(90, 50, 131, 21));
        lineEdit_2 = new QLineEdit(Login_Dialog);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(90, 110, 131, 21));

        retranslateUi(Login_Dialog);

        QMetaObject::connectSlotsByName(Login_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Login_Dialog)
    {
        Login_Dialog->setWindowTitle(QApplication::translate("Login_Dialog", "Dialog", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Login_Dialog", "PushButton", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Login_Dialog", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Login_Dialog: public Ui_Login_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_DIALOG_H
