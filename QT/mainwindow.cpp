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

	QString imagefile = QFileDialog::getOpenFileName(this,"��ͼ��",QDir::currentPath(),"Document files (*.jpg *.bmp);;All files(*.*)");
	
	this->image = cvLoadImage(imagefile.toLocal8Bit().data(), 1);

	//	IplImage * test; test = cvLoadImage(filename.toLocal8Bit().data());

	this->qImage = new QImage(QSize(this->image->width, this->image->height), QImage::Format_RGB888);
	this->tempImage = cvCreateImageHeader(cvSize(this->image->width, this->image->height), 8, 3);
	this->tempImage->imageData = (char*)(this->qImage->bits());// tempImage->imageDataָ��Ĵ��ͼ��ʵ�������ָ��, qlmage->bits() ��QT�� ͼ��ʵ������ָ�룬������tempImage->imageData Ҳָ�� qlmage->bits() ָ�������
	cvCopy(this->image, this->tempImage, 0);// ��ͼ�����ݴ�this->image ������ this->tempImageָ������򣬶�this->tempImage��ʱָ��������this->qImage->bits()��ָ����ͬһ������
	cvCvtColor(this->tempImage, this->tempImage, CV_BGR2RGB); //�������ɫ�ռ�ת��
	this->ui->image_label->setPixmap((QPixmap::fromImage(*this->qImage)).scaled(this->ui->image_label->size()));// ��仰��������Ҫ�� ��opencv���������ͼƬ�ŵ� qt ָ���ķ�������

}
