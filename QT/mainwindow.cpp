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
	Mat srcimg=	show_img_label(imagefile);//显示图像函数
	Mat thresgolg_img =	Ada_Thresgold(imagefile.toLocal8Bit().data());
		findface(imagefile);
		bitwise_not(thresgolg_img, thresgolg_img);//图像取反
		imshow("fan", thresgolg_img);
		//区域识别

		Mat imgRplane = getRplane(srcimg); //获得原始图像R分量
		vector <RotatedRect>  rects;
		posDetect(imagefile.toLocal8Bit().data(), imgRplane, rects);  //获得身份证号码区域







		
	}
	else
	{
		return;
	}

}

//show img label
Mat MainWindow::show_img_label(QString &filename)
{

	srcimg = cvLoadImage(filename.toLocal8Bit().data(), CV_LOAD_IMAGE_COLOR);
	display(srcimg);
	return srcimg;
}
void MainWindow::display(cv::Mat mat)
{
	cv::Mat rgb;
	QImage img;
	if (mat.channels() == 3)
	{
		cv::cvtColor(mat, rgb, CV_BGR2RGB);
		img = QImage((const uchar*)(rgb.data), rgb.cols, rgb.rows, rgb.cols*rgb.channels(), QImage::Format_RGB888);
		
	}
	else
	{
		img = QImage((const uchar*)(mat.data), mat.cols, mat.rows, mat.cols*mat.channels(), QImage::Format_Indexed8);
	}
	
	ui->image_label->setPixmap(QPixmap::fromImage(img).scaled(ui->image_label->width(), ui->image_label->height()));
	
	ui->image_label->resize(ui->image_label->pixmap()->size());
	ui->image_label->show();
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
Mat MainWindow::Ada_Thresgold(const char* filename)
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
	return Iat;
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
	faceCascade.detectMultiScale(smallImg, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(50, 100));
	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
	{
	//	cv::rectangle(img, *r, Scalar(0, 255, 0), 1, 8, 0);//在img上绘制出检测到的面部矩形框，绿色框  
		

//控制矩形框位置
		int RectPosition_x = faces[0].x;
		int RectPosition_y = faces[0].y;
		int RectPosition_w = faces[0].width;
		int RectPosition_h = faces[0].height;
		Rect imgr(RectPosition_x-20, RectPosition_y-20, RectPosition_w + 30, RectPosition_h+50);

		cv::rectangle(img, imgr, Scalar(0, 255, 0), 1, 8, 0);//绘制方框
	//控制矩形框位置	
	//	Rect rect(a.x,a.y,50, 50); // (左上x, 左上y, 宽度, 高度)  
		img(imgr).copyTo(roi_img); //拷贝矩形区域  
	//	imshow("cut", roi_img);
	}

	cv::imshow("result", img);//将img显示到result窗口  

//	cv::imshow("cut", roi_img);//显示img

	show_img_label2(roi_img);

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

//显示图像在头像框
void MainWindow::show_img_label2(Mat &src)
{
	ui->image_label2->clear();
	cv::Mat rgb;
	QImage img;
	if (src.channels() == 3)
	{
		cv::cvtColor(src, rgb, CV_BGR2RGB);
		img = QImage((const uchar*)(rgb.data), rgb.cols, rgb.rows, rgb.cols*rgb.channels(), QImage::Format_RGB888);

	}
	else
	{
		img = QImage((const uchar*)(src.data), src.cols, src.rows, src.cols*src.channels(), QImage::Format_Indexed8);
	}

	ui->image_label2->setPixmap(QPixmap::fromImage(img).scaled(ui->image_label2->width(), ui->image_label2->height()));

	ui->image_label2->resize(ui->image_label2->pixmap()->size());
	ui->image_label2->show();


}


//区域识别 //输入彩色原图


void MainWindow::posDetect(const char* filename,const Mat &in, vector<RotatedRect> & rects)

{

//	Mat threshold_R;
//	OstuBeresenThreshold(in, threshold_R); //二值化
	Mat threshold_R = Ada_Thresgold(filename);

	Mat imgInv(threshold_R.size(), threshold_R.type(), cv::Scalar(255));
//	Mat threshold_Inv = imgInv - threshold_R; //黑白色反转，即背景为黑色
	Mat threshold_Inv;
		
	bitwise_not(threshold_R, threshold_Inv);



	Mat element = getStructuringElement(MORPH_RECT, Size(15, 3));  //闭形态学的结构元素
	morphologyEx(threshold_Inv, threshold_Inv, CV_MOP_CLOSE, element);

	vector< vector <Point> > contours;
	findContours(threshold_Inv, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//只检测外轮廓
	//对候选的轮廓进行进一步筛选
	vector< vector <Point> > ::iterator itc = contours.begin();
	vector< vector <Point> > ::iterator itc2 = contours.begin();
	while (itc != contours.end())
	{
		RotatedRect mr = minAreaRect(Mat(*itc)); //返回每个轮廓的最小有界矩形区域
		if (!isEligible(mr))  //判断矩形轮廓是否符合要求
		{
			itc = contours.erase(itc);
			
		}
		else
		{
			rects.push_back(mr);
			++itc;
		}
	}
	
	
	//测试是否找到了号码区域
	    Mat out;
	    in.copyTo(out);

	    Point2f vertices[4];
	    rects[0].points(vertices);
	    for (int i = 0; i < 4; i++)
	    line(out, vertices[i], vertices[(i+1)%4], Scalar(0,0,0));//画黑色线条

	   imshow("Test_Rplane" ,out);
	    waitKey(0);
		

}


bool MainWindow::isEligible(const RotatedRect &candidate)
{
	float error = 0.2;
	const float aspect = 4.5 / 0.3; //长宽比
	int min = 10 * aspect * 10; //最小区域
	int max = 50 * aspect * 50;  //最大区域
	float rmin = aspect - aspect*error; //考虑误差后的最小长宽比
	float rmax = aspect + aspect*error; //考虑误差后的最大长宽比

	int area = candidate.size.height * candidate.size.width;
	float r = (float)candidate.size.width / (float)candidate.size.height;
	if (r <1)
		r = 1 / r;

	if ((area < min || area > max) || (r< rmin || r > rmax)) //满足该条件才认为该candidate为车牌区域
		return false;
	else
		return true;
}