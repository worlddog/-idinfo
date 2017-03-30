#include "login_dialog.h"
#include "ui_login_dialog.h"

Login_Dialog::Login_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login_Dialog)
{
    ui->setupUi(this);
}

Login_Dialog::~Login_Dialog()
{
    delete ui;
}
