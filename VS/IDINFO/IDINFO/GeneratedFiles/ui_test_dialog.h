/********************************************************************************
** Form generated from reading UI file 'test_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEST_DIALOG_H
#define UI_TEST_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Test_Dialog
{
public:
    QPushButton *pushButton;

    void setupUi(QDialog *Test_Dialog)
    {
        if (Test_Dialog->objectName().isEmpty())
            Test_Dialog->setObjectName(QStringLiteral("Test_Dialog"));
        Test_Dialog->resize(400, 300);
        pushButton = new QPushButton(Test_Dialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(170, 120, 93, 28));

        retranslateUi(Test_Dialog);

        QMetaObject::connectSlotsByName(Test_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Test_Dialog)
    {
        Test_Dialog->setWindowTitle(QApplication::translate("Test_Dialog", "Dialog", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Test_Dialog", "\346\211\223\345\274\200\345\233\276\345\203\217", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Test_Dialog: public Ui_Test_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEST_DIALOG_H
