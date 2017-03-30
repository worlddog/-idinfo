#include "set_dialog.h"
#include "ui_set_dialog.h"

set_Dialog::set_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::set_Dialog)
{
    ui->setupUi(this);
}

set_Dialog::~set_Dialog()
{
    delete ui;
}
