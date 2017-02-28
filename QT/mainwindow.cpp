#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"test_dialog.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_testaction_triggered()
{
	Test_Dialog *test_dlg;
	test_dlg = new Test_Dialog();
	test_dlg->show();
}

void MainWindow::on_pushButton_clicked()
{

	QString imagefile = QFileDialog::getOpenFileName(this,"打开图像",QDir::currentPath(),"Document files (*.jpg *.bmp);;All files(*.*)");
	
	this->image = cvLoadImage(imagefile.toLocal8Bit().data(), 1);

	//	IplImage * test; test = cvLoadImage(filename.toLocal8Bit().data());

	this->qImage = new QImage(QSize(this->image->width, this->image->height), QImage::Format_RGB888);
	this->tempImage = cvCreateImageHeader(cvSize(this->image->width, this->image->height), 8, 3);
	this->tempImage->imageData = (char*)(this->qImage->bits());// tempImage->imageData指向的存放图像实体区域的指针, qlmage->bits() 是QT中 图像实体区域指针，现在让tempImage->imageData 也指向 qlmage->bits() 指向的区域
	cvCopy(this->image, this->tempImage, 0);// 把图像数据从this->image 拷贝到 this->tempImage指向的区域，而this->tempImage此时指向的区域和this->qImage->bits()所指的是同一个区域
	cvCvtColor(this->tempImage, this->tempImage, CV_BGR2RGB); //这个是颜色空间转换
	this->ui->image_label->setPixmap((QPixmap::fromImage(*this->qImage)).scaled(this->ui->image_label->size()));// 这句话就是你想要的 把opencv处理出来的图片放到 qt 指定的方框里了

}
