#include "test_dialog.h"
#include "ui_test_dialog.h"
#include <QFileDialog>
#include<qmessagebox.h>
#include <QDebug>

#include <cv.h> 
#include <highgui.h> 

Test_Dialog::Test_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test_Dialog)
{
    ui->setupUi(this);
}

Test_Dialog::~Test_Dialog()
{
    delete ui;
}

void Test_Dialog::on_pushButton_clicked()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		"打开图像",
		QDir::currentPath(),
		"Document files (*.jpg *.bmp);;All files(*.*)");
	


	if (!filename.isNull()) 
	{ 
		// 处理文件

		IplImage * test; test = cvLoadImage(filename.toLocal8Bit().data());
		cvNamedWindow("test_demo", 1); 
		cvShowImage("test_demo", test); 
		cvWaitKey(0); cvDestroyWindow("test_demo"); 
		cvReleaseImage(&test); 
	}
	

}

