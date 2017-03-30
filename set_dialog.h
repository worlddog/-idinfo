#ifndef SET_DIALOG_H
#define SET_DIALOG_H

#include <QDialog>

namespace Ui {
class set_Dialog;
}

class set_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit set_Dialog(QWidget *parent = 0);
    ~set_Dialog();

private:
    Ui::set_Dialog *ui;
};

#endif // SET_DIALOG_H
