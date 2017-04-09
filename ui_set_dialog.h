/********************************************************************************
** Form generated from reading UI file 'set_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SET_DIALOG_H
#define UI_SET_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_set_Dialog
{
public:

    void setupUi(QDialog *set_Dialog)
    {
        if (set_Dialog->objectName().isEmpty())
            set_Dialog->setObjectName(QStringLiteral("set_Dialog"));
        set_Dialog->resize(400, 300);

        retranslateUi(set_Dialog);

        QMetaObject::connectSlotsByName(set_Dialog);
    } // setupUi

    void retranslateUi(QDialog *set_Dialog)
    {
        set_Dialog->setWindowTitle(QApplication::translate("set_Dialog", "Dialog", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class set_Dialog: public Ui_set_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SET_DIALOG_H
