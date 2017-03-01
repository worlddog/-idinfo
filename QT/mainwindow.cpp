#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif
//设置编码 使用中文
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"test_dialog.h"
#include <QFileDialog>
#include"qt_windows.h"
#include "opencv2/objdetect/objdetect.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
#include <cctype>  
#include <iostream>  
#include <iterator>  
#include <stdio.h>  
#include<qmessagebox.h>

using namespace std;
using namespace cv;

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

	QString imagefile = QFileDialog::getOpenFileName(this, "打开图像", QDir::currentPath(), "Document files (*.jpg *.bmp);;All files(*.*)");

	if (!imagefile.isNull())
	{


		this->image = cvLoadImage(imagefile.toLocal8Bit().data(), 1);
	
		this->qImage = new QImage(QSize(this->image->width, this->image->height), QImage::Format_RGB888);
		this->tempImage = cvCreateImageHeader(cvSize(this->image->width, this->image->height), 8, 3);
		this->tempImage->imageData = (char*)(this->qImage->bits());// tempImage->imageData指向的存放图像实体区域的指针, qlmage->bits() 是QT中 图像实体区域指针，现在让tempImage->imageData 也指向 qlmage->bits() 指向的区域
		cvCopy(this->image, this->tempImage, 0);// 把图像数据从this->image 拷贝到 this->tempImage指向的区域，而this->tempImage此时指向的区域和this->qImage->bits()所指的是同一个区域
		cvCvtColor(this->tempImage, this->tempImage, CV_BGR2RGB); //这个是颜色空间转换
		this->ui->image_label->setPixmap((QPixmap::fromImage(*this->qImage)).scaled(this->ui->image_label->size()));// 这句话就是你想要的 把opencv处理出来的图片放到 qt 指定的方框里了


		Ada_Thresgold(imagefile.toLocal8Bit().data());
		findface(imagefile);
		
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



//人脸检测与识别

void MainWindow::detectAndDraw(Mat& img, CascadeClassifier& faceCascade, double scale)
{
	int i = 0;
	double t = 0;
	vector<Rect> faces;//用来存储检测出来的面部数据，我们无法确定个数，因此定义成vector  
	Mat gray, smallImg(cvRound(img.rows / scale), cvRound(img.cols / scale), CV_8UC1);//smallImg已缩放  

	cvtColor(img, gray, CV_BGR2GRAY);//图片颜色格式转化，CV_BGR2GRAY是从BGR到gray，彩色到灰色  
	cv::resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);//将灰色图像适应大小到smallImg中  
	equalizeHist(smallImg, smallImg);//加强对比度，提高检测准确率  

	t = (double)cvGetTickCount();//获取当前时间  
	//使用级联分类器进行识别，参数为灰度图片，面部数组，检测单元的增长率，是否融合检测出的矩形，最小检测单元大小  
	faceCascade.detectMultiScale(smallImg, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
	{
		cv::rectangle(img, *r, Scalar(0, 255, 0), 1, 1, 0);//在img上绘制出检测到的面部矩形框，绿色框  

	}

	cv::imshow("result", img);//将img显示到result窗口  


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


void MainWindow::findface(QString &imagefile)
{
	//面部识别XML文件
	string cascadeName = "D:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	Mat image;//opencv中存储数据的基本单位，存储帧，图片等，代替旧版本的IplImage  
	string inputName(imagefile.toLocal8Bit().data());//示例图片，放在当前目录  
	CascadeClassifier faceCascade;//定义级联分类器，由它们实现检测功能  
	double scale = 1;//不缩小图片，这样可以提高准确率  
	if (!faceCascade.load(cascadeName))//载入xml训练数据  
	{
		QMessageBox::information(NULL, "警告", "载入XML失败",  QMessageBox::Yes);
	}
	image = imread(inputName, CV_LOAD_IMAGE_COLOR);//读取图片，第二个参数说明是彩色图片  
	cvNamedWindow("result", 1);//创建窗口，命名result，id为1  
	if (!image.empty())
	{
		detectAndDraw(image, faceCascade, scale);//进行识别  
	}

}