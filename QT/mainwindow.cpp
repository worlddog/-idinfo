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
	
	if (!imagefile.isNull())
	{
		

		this->image = cvLoadImage(imagefile.toLocal8Bit().data(), 1);
		//	IplImage * test; test = cvLoadImage(filename.toLocal8Bit().data());
		this->qImage = new QImage(QSize(this->image->width, this->image->height), QImage::Format_RGB888);
		this->tempImage = cvCreateImageHeader(cvSize(this->image->width, this->image->height), 8, 3);
		this->tempImage->imageData = (char*)(this->qImage->bits());// tempImage->imageData指向的存放图像实体区域的指针, qlmage->bits() 是QT中 图像实体区域指针，现在让tempImage->imageData 也指向 qlmage->bits() 指向的区域
		cvCopy(this->image, this->tempImage, 0);// 把图像数据从this->image 拷贝到 this->tempImage指向的区域，而this->tempImage此时指向的区域和this->qImage->bits()所指的是同一个区域
		cvCvtColor(this->tempImage, this->tempImage, CV_BGR2RGB); //这个是颜色空间转换
		this->ui->image_label->setPixmap((QPixmap::fromImage(*this->qImage)).scaled(this->ui->image_label->size()));// 这句话就是你想要的 把opencv处理出来的图片放到 qt 指定的方框里了
	
	
		Ada_Thresgold(imagefile.toLocal8Bit().data());
	



	
	
	
	}
	else
	{
		return;
	}




	




}

//1、获取图片R通道
Mat MainWindow::getRplane(const Mat &in)
{
	vector<Mat> splitBGR(in.channels()); //容器大小为通道数3
	split(in, splitBGR);
	//return splitBGR[2];  //R分量

	if (in.cols > 700 | in.cols >600)
	{
		Mat resizeR(450, 600, CV_8UC1);
		cv::resize(splitBGR[2], resizeR, resizeR.size());

		return resizeR;
	}
	else
		return splitBGR[2];

}


//2,自适应二值阈值化
void MainWindow::Ada_Thresgold(const char* filename)
{
	int adaptive_method = CV_ADAPTIVE_THRESH_MEAN_C ;
	int threshold_type = CV_THRESH_BINARY;
	int block_size = 75;
	double offset = 15;
	double param1 = 5;

	IplImage *Igray = 0, *Iat;
	
	//输入单通道图像
	Igray = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);
	//创建输出图像
	Iat = cvCreateImage(cvSize(Igray->width, Igray->height), IPL_DEPTH_8U, 1);
	//二值化
	cvAdaptiveThreshold(Igray, Iat,255, adaptive_method, threshold_type, block_size, offset);
	//显示窗口
	cvNamedWindow("Ada_Th", 1);
	cvShowImage("Ada_Th", Iat);

}    