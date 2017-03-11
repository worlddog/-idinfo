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
#include<qdebug.h>
#include <QLabel>



using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	
	QDir run_dir;
	
    ui->setupUi(this);
	ui->statusBar->showMessage(QApplication::applicationDirPath());
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
		Mat c_src = srcimg_color(imagefile);
		Mat g_src = srcimg_gray(imagefile);


		show_img_label(c_src);//��ʾͼ����
		Ada_Thresgold(g_src);
		
		findface(c_src);
	//	bitwise_not(thresgolg_img, thresgolg_img);//ͼ��ȡ��
	
		//����ʶ��

	//	Mat imgRplane = getRplane(srcimg); //���ԭʼͼ��R����
	//	vector <RotatedRect>  rects;
	/*	posDetect( imgRplane, rects);  //������֤��������

		Mat outputMat;
		normalPosArea(imgRplane, rects[0], outputMat); //������֤�����ַ�����
		
		vector<Mat> char_mat;  //����и�õ��ַ�����
		char_segment(outputMat, char_mat);


		
		//getAnnXML();  //�ú���ֻ��ִ��һ�Σ����ѵ������ͱ�ǩ���󣬱�����xml�ļ���

		CvANN_MLP ann;
		ann_train(ann, 10, 24); //10Ϊ�������,Ҳ������������24Ϊ���ز���
		
		vector<int> char_result;
		classify(ann, char_mat, char_result);
		
		getParityBit(char_result); //���һλ�׳���ֱ����ǰ17λ�������һλ

		//���ý�����Ϣ

		QString id = "";
		for (int i = 0; i < char_result.size(); ++i)
		{
			if (char_result[i] == 10)
				id += "X";
			else
				id += QString::number(char_result[i], 10); //��intת��ΪQstring
		}
		ui->lineEdit_6->setText(id);

	
	*/

		
	}
	else
	{
		return;
	}

}

//show img label
void MainWindow::show_img_label(Mat &src)
{
	display(srcimg);
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
/*
Mat MainWindow::Ada_Thresgold(const char* filename)
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
	return Iat;
}    */
Mat MainWindow::Ada_Thresgold(const Mat &src)
{
	int adaptive_method = CV_ADAPTIVE_THRESH_MEAN_C;
	int threshold_type = CV_THRESH_BINARY;
	int block_size = 75;
	double offset = 15;
	double param1 = 5;

	Mat  out;

	
	//��ֵ��

	cv::adaptiveThreshold(src, out, 255, adaptive_method, threshold_type, block_size, offset);
		//��ʾ����
		imshow("ff", out);
	return out;
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
	faceCascade.detectMultiScale(smallImg, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(50, 100));
	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
	{
	//	cv::rectangle(img, *r, Scalar(0, 255, 0), 1, 8, 0);//��img�ϻ��Ƴ���⵽���沿���ο���ɫ��  
		

//���ƾ��ο�λ��
		int RectPosition_x = faces[0].x;
		int RectPosition_y = faces[0].y;
		int RectPosition_w = faces[0].width;
		int RectPosition_h = faces[0].height;
		Rect imgr(RectPosition_x-20, RectPosition_y-20, RectPosition_w + 30, RectPosition_h+50);

		cv::rectangle(img, imgr, Scalar(0, 255, 0), 1, 8, 0);//���Ʒ���
	//���ƾ��ο�λ��	
	//	Rect rect(a.x,a.y,50, 50); // (����x, ����y, ���, �߶�)  
		img(imgr).copyTo(roi_img); //������������  
	//	imshow("cut", roi_img);
	}

	cv::imshow("result", img);//��img��ʾ��result����  

//	cv::imshow("cut", roi_img);//��ʾimg

	show_img_label2(roi_img);

}


void MainWindow::findface(Mat &src)
{
	//�沿ʶ��XML�ļ�
	string cascadeName = "D:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	//Mat image;//opencv�д洢���ݵĻ�����λ���洢֡��ͼƬ�ȣ�����ɰ汾��IplImage  
	//string inputName(imagefile.toLocal8Bit().data());//ʾ��ͼƬ�����ڵ�ǰĿ¼  

	CascadeClassifier faceCascade;//���弶����������������ʵ�ּ�⹦��  
	double scale = 1;//����СͼƬ�������������׼ȷ��  
	if (!faceCascade.load(cascadeName))//����xmlѵ������  
	{
		QMessageBox::information(NULL, "����", "����XMLʧ��",  QMessageBox::Yes);
	}
	//image = imread(inputName, CV_LOAD_IMAGE_COLOR);//��ȡͼƬ���ڶ�������˵���ǲ�ɫͼƬ  
	cvNamedWindow("result", 1);//�������ڣ�����result��idΪ1  
	if (!src.empty())
	{
		detectAndDraw(src, faceCascade, scale);//����ʶ��  
	}



}

//��ʾͼ����ͷ���
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


//����ʶ�� //�����ɫԭͼ


void MainWindow::posDetect(const Mat &in, vector<RotatedRect> & rects) //Ѱ�Һ�������

{

//	Mat threshold_R;
//	OstuBeresenThreshold(in, threshold_R); //��ֵ��
	Mat threshold_R = Ada_Thresgold(in);

	Mat imgInv(threshold_R.size(), threshold_R.type(), cv::Scalar(255));
//	Mat threshold_Inv = imgInv - threshold_R; //�ڰ�ɫ��ת��������Ϊ��ɫ
	Mat threshold_Inv;
		
	bitwise_not(threshold_R, threshold_Inv);



	Mat element = getStructuringElement(MORPH_RECT, Size(15, 3));  //����̬ѧ�ĽṹԪ��
	morphologyEx(threshold_Inv, threshold_Inv, CV_MOP_CLOSE, element);

	vector< vector <Point> > contours;
	findContours(threshold_Inv, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//ֻ���������
	//�Ժ�ѡ���������н�һ��ɸѡ
	vector< vector <Point> > ::iterator itc = contours.begin();
	vector< vector <Point> > ::iterator itc2 = contours.begin();
	while (itc != contours.end())
	{
		RotatedRect mr = minAreaRect(Mat(*itc)); //����ÿ����������С�н��������
		if (!isEligible(mr))  //�жϾ��������Ƿ����Ҫ��
		{
			itc = contours.erase(itc);
			
		}
		else
		{
			rects.push_back(mr);
			++itc;
		}
	}
	
	
	//�����Ƿ��ҵ��˺�������
	Mat out, roi_img;
	    in.copyTo(out);
		vector<Rect> numbers;
	    Point2f vertices[4];
	    rects[0].points(vertices);
		for (int i = 0; i < 4; i++)
			//  line(out, vertices[i], vertices[(i+1)%4], Scalar(0,0,0));//����ɫ����

			//	Point2f size = (10, 80);
			//0 ���½� 1 ���Ͻ� 2 ���Ͻ� 3 ���½�
			int px, y, pw, ph = 0;
		int px = vertices[1].x-5;
		int py = vertices[1].y-5;
		int pw = (vertices[2].x - vertices[0].x)+10;
		int ph = (vertices[3].y - vertices[2].y)+10;
		Rect imgr(px, py, pw, ph);

		cv::rectangle(out, imgr, Scalar(255, 255, 0), 1, 8, 0);//���Ʒ���
		out(imgr).copyTo(roi_img); //������������  
		imshow("NUMBER", roi_img);
		imwrite("d:/number_roi.jpg", roi_img);
	 //  imshow("NUMBER", out);
	



	   
}


bool MainWindow::isEligible(const RotatedRect &candidate) //�ж����֤��������
{
	float error = 0.2;
	const float aspect = 4.5 / 0.3; //�����
	int min = 10 * aspect * 10; //��С����
	int max = 50 * aspect * 50;  //�������
	float rmin = aspect - aspect*error; //�����������С�����
	float rmax = aspect + aspect*error; //�����������󳤿��

	int area = candidate.size.height * candidate.size.width;
	float r = (float)candidate.size.width / (float)candidate.size.height;
	if (r <1)
		r = 1 / r;

	if ((area < min || area > max) || (r< rmin || r > rmax)) //�������������Ϊ��candidateΪ��������
		return false;
	else
		return true;
}//�ж����֤��������

void MainWindow::normalPosArea(const Mat &intputImg, RotatedRect &rects_optimal, Mat& output_area)
{
	float r, angle;

	angle = rects_optimal.angle;
	r = (float)rects_optimal.size.width / (float)(float)rects_optimal.size.height;
	if (r<1)
		angle = 90 + angle;
	Mat rotmat = getRotationMatrix2D(rects_optimal.center, angle, 1);//��ñ��ξ������
	Mat img_rotated;
	warpAffine(intputImg, img_rotated, rotmat, intputImg.size(), CV_INTER_CUBIC);

	//�ü�ͼ��
	Size rect_size = rects_optimal.size;

	if (r<1)
		std::swap(rect_size.width, rect_size.height);
	Mat  img_crop;
	getRectSubPix(img_rotated, rect_size, rects_optimal.center, img_crop);

	//�ù���ֱ��ͼ�������вü��õ���ͼ��ʹ������ͬ��Ⱥ͸߶ȣ�������ѵ���ͷ���
	Mat resultResized;
	resultResized.create(20, 300, CV_8UC1);
	cv::resize(img_crop, resultResized, resultResized.size(), 0, 0, INTER_CUBIC);

	resultResized.copyTo(output_area);



}

void MainWindow::char_segment(const Mat &inputImg, vector<Mat> &dst_mat)
{
	Mat img_threshold;

Mat whiteImg(inputImg.size(), inputImg.type(), cv::Scalar(255));
	Mat in_Inv = whiteImg - inputImg;
//Mat in_Inv;
	//bitwise_not(in_Inv, whiteImg);

	// threshold(in_Inv ,img_threshold , 140,255 ,CV_THRESH_BINARY ); //��ת�ڰ�ɫ
	threshold(in_Inv, img_threshold, 0, 255, CV_THRESH_OTSU); //��򷨶�ֵ��

	int x_char[19] = { 0 };
	short counter = 1;
	short num = 0;
	//bool flag[img_threshold.cols];
	bool flag[65535];

	for (int j = 0; j < img_threshold.cols; ++j)
	{
		flag[j] = true;
		for (int i = 0; i < img_threshold.rows; ++i)
		{

			if (img_threshold.at<uchar>(i, j) != 0)
			{
				flag[j] = false;
				break;
			}

		}
	}

	for (int i = 0; i < img_threshold.cols - 2; ++i)
	{
		if (flag[i] == true)
		{
			x_char[counter] += i;
			num++;
			if (flag[i + 1] == false && flag[i + 2] == false)
			{
				x_char[counter] = x_char[counter] / num;
				num = 0;
				counter++;
			}
		}
	}
	x_char[18] = img_threshold.cols;

	for (int i = 0; i < 18; i++)
	{
		dst_mat.push_back(Mat(in_Inv, Rect(x_char[i], 0, x_char[i + 1] - x_char[i], img_threshold.rows)));
	}


	// imwrite("b.jpg" , img_threshold);

}


void MainWindow::getAnnXML() // �˺�����������һ�Σ�Ŀ���ǻ��ѵ������ͱ�ǩ���󣬱�����ann_xml.xml��
{
	FileStorage fs("C:\\Users\\weiyi\\Desktop\\Id_recognition\\ann_xml.xml", FileStorage::WRITE);
	if (!fs.isOpened())
	{
		qDebug() << "failed to open " << "/n";
	}

	Mat  trainData;
	Mat classes = Mat::zeros(1, 550, CV_8UC1);   //1700*48ά��Ҳ��ÿ��������48������ֵ
	char path[60];
	Mat img_read;
	for (int i = 0; i<10; i++)  //��i��
	{
		for (int j = 1; j< 51; ++j)  //i���е�j�������ܹ���11���ַ���ÿ����50������
		{
			sprintf(path, "C:\\Users\\weiyi\\Desktop\\Id_recognition\\Number_char\\%d\\%d (%d).png", i, i, j);
			img_read = imread(path, 0);
			Mat dst_feature;
			calcGradientFeat(img_read, dst_feature); //����ÿ������������ʸ��
			trainData.push_back(dst_feature);

			classes.at<uchar>(i * 50 + j - 1) = i;
		}
	}
	fs << "TrainingData" << trainData;
	fs << "classes" << classes;
	fs.release();

}



float MainWindow::sumMatValue(const Mat &image)
{
	float sumValue = 0;
	int r = image.rows;
	int c = image.cols;
	if (image.isContinuous())
	{
		c = r*c;
		r = 1;
	}
	for (int i = 0; i < r; i++)
	{
		const uchar* linePtr = image.ptr<uchar>(i);
		for (int j = 0; j < c; j++)
		{
			sumValue += linePtr[j];
		}
	}
	return sumValue;
}

Mat MainWindow::projectHistogram(const Mat &img, int t)
{
	Mat lowData;
	cv::resize(img, lowData, Size(8, 16)); //���ŵ�8*16

	int sz = (t) ? lowData.rows : lowData.cols;
	Mat mhist = Mat::zeros(1, sz, CV_32F);

	for (int j = 0; j < sz; j++)
	{
		Mat data = (t) ? lowData.row(j) : lowData.col(j);
		mhist.at<float>(j) = countNonZero(data);
	}

	double min, max;
	minMaxLoc(mhist, &min, &max);

	if (max > 0)
		mhist.convertTo(mhist, -1, 1.0f / max, 0);

	return mhist;
}

void MainWindow::ann_train(CvANN_MLP &ann, int numCharacters, int nlayers)
{
	Mat trainData, classes;
	FileStorage fs;
	fs.open("C:\\Users\\weiyi\\Desktop\\Id_recognition\\ann_xml.xml", FileStorage::READ);

	fs["TrainingData"] >> trainData;
	fs["classes"] >> classes;

	Mat layerSizes(1, 3, CV_32SC1);     //3��������
	layerSizes.at<int>(0) = trainData.cols;   //��������Ԫ�����������Ϊ24
	layerSizes.at<int>(1) = nlayers; //1�����ز����Ԫ�����������Ϊ24
	layerSizes.at<int>(2) = numCharacters; //��������Ԫ�����Ϊ:10
	ann.create(layerSizes, CvANN_MLP::SIGMOID_SYM, 1, 1);  //��ʼ��ann
	CvANN_MLP_TrainParams param;
	param.term_crit = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 5000, 0.01);


	Mat trainClasses;
	trainClasses.create(trainData.rows, numCharacters, CV_32FC1);
	for (int i = 0; i< trainData.rows; i++)
	{
		for (int k = 0; k< trainClasses.cols; k++)
		{
			if (k == (int)classes.at<uchar>(i))
			{
				trainClasses.at<float>(i, k) = 1;
			}
			else
				trainClasses.at<float>(i, k) = 0;

		}

	}

	//Mat weights(1 , trainData.rows , CV_32FC1 ,Scalar::all(1) );
	ann.train(trainData, trainClasses, Mat(), Mat(), param);
}

void MainWindow::classify(CvANN_MLP &ann, vector<Mat> &char_Mat, vector<int> &char_result)
{
	char_result.resize(char_Mat.size());
	for (int i = 0; i<char_Mat.size(); ++i)
	{
		Mat output(1, 10, CV_32FC1); //1*10����

		Mat char_feature;
		calcGradientFeat(char_Mat[i], char_feature);
		ann.predict(char_feature, output);
		Point maxLoc;
		double maxVal;
		minMaxLoc(output, 0, &maxVal, 0, &maxLoc);

		char_result[i] = maxLoc.x;

	}
}

void MainWindow::getParityBit(vector<int> &char_result)
{
	int mod = 0;
	int wights[17] = { 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 };
	for (int i = 0; i < 17; ++i)
		mod += char_result[i] * wights[i];//����Ӧϵ�����

	mod = mod % 11; //��11����

	int value[11] = { 1, 0, 10, 9, 8, 7, 6, 5, 4, 3, 2 };
	char_result[17] = value[mod];

}


void MainWindow::calcGradientFeat(const Mat &imgSrc, Mat &out)
{
	vector <float>  feat;
	Mat image;

	//cvtColor(imgSrc,image,CV_BGR2GRAY);
	cv::resize(imgSrc, image, Size(8, 16));

	// ����x�����y�����ϵ��˲�
	float mask[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

	Mat y_mask = Mat(3, 3, CV_32F, mask) / 8;
	Mat x_mask = y_mask.t(); // ת��
	Mat sobelX, sobelY;

	filter2D(image, sobelX, CV_32F, x_mask);
	filter2D(image, sobelY, CV_32F, y_mask);

	sobelX = abs(sobelX);
	sobelY = abs(sobelY);

	float totleValueX = sumMatValue(sobelX);
	float totleValueY = sumMatValue(sobelY);

	// ��ͼ�񻮷�Ϊ4*2��8�����ӣ�����ÿ��������Ҷ�ֵ�ܺ͵İٷֱ�
	for (int i = 0; i < image.rows; i = i + 4)
	{
		for (int j = 0; j < image.cols; j = j + 4)
		{
			Mat subImageX = sobelX(Rect(j, i, 4, 4));
			feat.push_back(sumMatValue(subImageX) / totleValueX);
			Mat subImageY = sobelY(Rect(j, i, 4, 4));
			feat.push_back(sumMatValue(subImageY) / totleValueY);
		}
	}

	//�����2������
	Mat imageGray;
	//cvtColor(imgSrc,imageGray,CV_BGR2GRAY);
	cv::resize(imgSrc, imageGray, Size(4, 8));
	Mat p = imageGray.reshape(1, 1);
	p.convertTo(p, CV_32FC1);
	for (int i = 0; i<p.cols; i++)
	{
		feat.push_back(p.at<float>(i));
	}

	//����ˮƽֱ��ͼ�ʹ�ֱֱ��ͼ
	Mat vhist = projectHistogram(imgSrc, 1); //ˮƽֱ��ͼ
	Mat hhist = projectHistogram(imgSrc, 0);  //��ֱֱ��ͼ
	for (int i = 0; i<vhist.cols; i++)
	{
		feat.push_back(vhist.at<float>(i));
	}
	for (int i = 0; i<hhist.cols; i++)
	{
		feat.push_back(hhist.at<float>(i));
	}


	out = Mat::zeros(1, feat.size(), CV_32F);
	for (int i = 0; i<feat.size(); i++)
	{
		out.at<float>(i) = feat[i];
	}
}


//��ȡͼ���� ���ز�ɫMAT
Mat MainWindow::srcimg_color(QString &filename)
{
	Mat srcimg = imread(filename.toLocal8Bit().data(), CV_LOAD_IMAGE_COLOR);
	return srcimg;

}
//��ȡͼƬ �Ҷ�
Mat MainWindow::srcimg_gray(QString &filename)
{
	Mat srcimg = imread(filename.toLocal8Bit().data(), CV_LOAD_IMAGE_GRAYSCALE);
	return srcimg;
}