#ifndef TEST_DIALOG_H
#define TEST_DIALOG_H

#include <QDialog>

namespace Ui {
class Test_Dialog;
}

class Test_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Test_Dialog(QWidget *parent = 0);
    ~Test_Dialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Test_Dialog *ui;
};

#endif // TEST_DIALOG_H
