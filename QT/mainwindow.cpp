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
	
	if (!imagefile.isNull())
	{
		

		this->image = cvLoadImage(imagefile.toLocal8Bit().data(), 1);
		//	IplImage * test; test = cvLoadImage(filename.toLocal8Bit().data());
		this->qImage = new QImage(QSize(this->image->width, this->image->height), QImage::Format_RGB888);
		this->tempImage = cvCreateImageHeader(cvSize(this->image->width, this->image->height), 8, 3);
		this->tempImage->imageData = (char*)(this->qImage->bits());// tempImage->imageDataָ��Ĵ��ͼ��ʵ�������ָ��, qlmage->bits() ��QT�� ͼ��ʵ������ָ�룬������tempImage->imageData Ҳָ�� qlmage->bits() ָ�������
		cvCopy(this->image, this->tempImage, 0);// ��ͼ�����ݴ�this->image ������ this->tempImageָ������򣬶�this->tempImage��ʱָ��������this->qImage->bits()��ָ����ͬһ������
		cvCvtColor(this->tempImage, this->tempImage, CV_BGR2RGB); //�������ɫ�ռ�ת��
		this->ui->image_label->setPixmap((QPixmap::fromImage(*this->qImage)).scaled(this->ui->image_label->size()));// ��仰��������Ҫ�� ��opencv���������ͼƬ�ŵ� qt ָ���ķ�������
	
	
		Ada_Thresgold(imagefile.toLocal8Bit().data());
	



	
	
	
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