#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif
//���ñ��� ʹ������
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

	QString imagefile = QFileDialog::getOpenFileName(this, "��ͼ��", QDir::currentPath(), "Document files (*.jpg *.bmp);;All files(*.*)");

	if (!imagefile.isNull())
	{


		this->image = cvLoadImage(imagefile.toLocal8Bit().data(), 1);
	
		this->qImage = new QImage(QSize(this->image->width, this->image->height), QImage::Format_RGB888);
		this->tempImage = cvCreateImageHeader(cvSize(this->image->width, this->image->height), 8, 3);
		this->tempImage->imageData = (char*)(this->qImage->bits());// tempImage->imageDataָ��Ĵ��ͼ��ʵ�������ָ��, qlmage->bits() ��QT�� ͼ��ʵ������ָ�룬������tempImage->imageData Ҳָ�� qlmage->bits() ָ�������
		cvCopy(this->image, this->tempImage, 0);// ��ͼ�����ݴ�this->image ������ this->tempImageָ������򣬶�this->tempImage��ʱָ��������this->qImage->bits()��ָ����ͬһ������
		cvCvtColor(this->tempImage, this->tempImage, CV_BGR2RGB); //�������ɫ�ռ�ת��
		this->ui->image_label->setPixmap((QPixmap::fromImage(*this->qImage)).scaled(this->ui->image_label->size()));// ��仰��������Ҫ�� ��opencv���������ͼƬ�ŵ� qt ָ���ķ�������


		Ada_Thresgold(imagefile.toLocal8Bit().data());
		findface(imagefile);
		
	}
	else
	{
		return;
	}

}




//1����ȡͼƬRͨ��
Mat MainWindow::getRplane(const Mat &in)
{
	vector<Mat> splitBGR(in.channels()); //������СΪͨ����3
	split(in, splitBGR);
	//return splitBGR[2];  //R����

	if (in.cols > 700 | in.cols >600)
	{
		Mat resizeR(450, 600, CV_8UC1);
		cv::resize(splitBGR[2], resizeR, resizeR.size());

		return resizeR;
	}
	else
		return splitBGR[2];

}



//���������ʶ��

void MainWindow::detectAndDraw(Mat& img, CascadeClassifier& faceCascade, double scale)
{
	int i = 0;
	double t = 0;
	vector<Rect> faces;//�����洢���������沿���ݣ������޷�ȷ����������˶����vector  
	Mat gray, smallImg(cvRound(img.rows / scale), cvRound(img.cols / scale), CV_8UC1);//smallImg������  

	cvtColor(img, gray, CV_BGR2GRAY);//ͼƬ��ɫ��ʽת����CV_BGR2GRAY�Ǵ�BGR��gray����ɫ����ɫ  
	cv::resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);//����ɫͼ����Ӧ��С��smallImg��  
	equalizeHist(smallImg, smallImg);//��ǿ�Աȶȣ���߼��׼ȷ��  

	t = (double)cvGetTickCount();//��ȡ��ǰʱ��  
	//ʹ�ü�������������ʶ�𣬲���Ϊ�Ҷ�ͼƬ���沿���飬��ⵥԪ�������ʣ��Ƿ��ںϼ����ľ��Σ���С��ⵥԪ��С  
	faceCascade.detectMultiScale(smallImg, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
	{
		cv::rectangle(img, *r, Scalar(0, 255, 0), 1, 1, 0);//��img�ϻ��Ƴ���⵽���沿���ο���ɫ��  

	}

	cv::imshow("result", img);//��img��ʾ��result����  


}


//2,����Ӧ��ֵ��ֵ��
void MainWindow::Ada_Thresgold(const char* filename)
{
	int adaptive_method = CV_ADAPTIVE_THRESH_MEAN_C ;
	int threshold_type = CV_THRESH_BINARY;
	int block_size = 75;
	double offset = 15;
	double param1 = 5;

	IplImage *Igray = 0, *Iat;
	
	//���뵥ͨ��ͼ��
	Igray = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);
	//�������ͼ��
	Iat = cvCreateImage(cvSize(Igray->width, Igray->height), IPL_DEPTH_8U, 1);
	//��ֵ��
	cvAdaptiveThreshold(Igray, Iat,255, adaptive_method, threshold_type, block_size, offset);
	//��ʾ����
	cvNamedWindow("Ada_Th", 1);
	cvShowImage("Ada_Th", Iat);

}    


void MainWindow::findface(QString &imagefile)
{
	//�沿ʶ��XML�ļ�
	string cascadeName = "D:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	Mat image;//opencv�д洢���ݵĻ�����λ���洢֡��ͼƬ�ȣ�����ɰ汾��IplImage  
	string inputName(imagefile.toLocal8Bit().data());//ʾ��ͼƬ�����ڵ�ǰĿ¼  
	CascadeClassifier faceCascade;//���弶����������������ʵ�ּ�⹦��  
	double scale = 1;//����СͼƬ�������������׼ȷ��  
	if (!faceCascade.load(cascadeName))//����xmlѵ������  
	{
		QMessageBox::information(NULL, "����", "����XMLʧ��",  QMessageBox::Yes);
	}
	image = imread(inputName, CV_LOAD_IMAGE_COLOR);//��ȡͼƬ���ڶ�������˵���ǲ�ɫͼƬ  
	cvNamedWindow("result", 1);//�������ڣ�����result��idΪ1  
	if (!image.empty())
	{
		detectAndDraw(image, faceCascade, scale);//����ʶ��  
	}

}