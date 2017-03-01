#include "mainwindow.h"
#include <QApplication>
#include "login_dialog.h"
#include"test_dialog.h"
#include <QTextCodec>// ±àÂë

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Login_Dialog login_dlg;
	Test_Dialog test_dlg;
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");//Çé¿ö1
    if(login_dlg.exec()== QDialog::Accepted )
{
    w.show();

    return a.exec();
}


else return 0;
}


