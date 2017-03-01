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
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Login_Dialog
{
public:
    QPushButton *pushButton;

    void setupUi(QDialog *Login_Dialog)
    {
        if (Login_Dialog->objectName().isEmpty())
            Login_Dialog->setObjectName(QStringLiteral("Login_Dialog"));
        Login_Dialog->resize(400, 300);
        pushButton = new QPushButton(Login_Dialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(160, 140, 93, 51));

        retranslateUi(Login_Dialog);
        QObject::connect(pushButton, SIGNAL(clicked()), Login_Dialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(Login_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Login_Dialog)
    {
        Login_Dialog->setWindowTitle(QApplication::translate("Login_Dialog", "Dialog", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Login_Dialog", "LOGIN", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Login_Dialog: public Ui_Login_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_DIALOG_H
