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

	QString imagefile = QFileDialog::getOpenFileName(this, "打开图像", QDir::currentPath(), "Document files (*.jpg *.bmp);;All files(*.*)");

	if (!imagefile.isNull())
	{ 
		Mat c_src = srcimg_color(imagefile);
		Mat g_src = srcimg_gray(imagefile);


		show_img_label(c_src);//显示图像函数
		Ada_Thresgold(g_src);
		
		findface(c_src);
	//	bitwise_not(thresgolg_img, thresgolg_img);//图像取反
	
		//区域识别

	//	Mat imgRplane = getRplane(srcimg); //获得原始图像R分量
	//	vector <RotatedRect>  rects;
	/*	posDetect( imgRplane, rects);  //获得身份证号码区域

		Mat outputMat;
		normalPosArea(imgRplane, rects[0], outputMat); //获得身份证号码字符矩阵
		
		vector<Mat> char_mat;  //获得切割得的字符矩阵
		char_segment(outputMat, char_mat);


		
		//getAnnXML();  //该函数只需执行一次，获得训练矩阵和标签矩阵，保存于xml文件中

		CvANN_MLP ann;
		ann_train(ann, 10, 24); //10为输出层结点,也等于输出的类别，24为隐藏层结点
		
		vector<int> char_result;
		classify(ann, char_mat, char_result);
		
		getParityBit(char_result); //最后一位易出错，直接由前17位计算最后一位

		//设置界面信息

		QString id = "";
		for (int i = 0; i < char_result.size(); ++i)
		{
			if (char_result[i] == 10)
				id += "X";
			else
				id += QString::number(char_result[i], 10); //将int转换为Qstring
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
/*
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
}    */
Mat MainWindow::Ada_Thresgold(const Mat &src)
{
	int adaptive_method = CV_ADAPTIVE_THRESH_MEAN_C;
	int threshold_type = CV_THRESH_BINARY;
	int block_size = 75;
	double offset = 15;
	double param1 = 5;

	Mat  out;

	
	//二值化

	cv::adaptiveThreshold(src, out, 255, adaptive_method, threshold_type, block_size, offset);
		//显示窗口
		imshow("ff", out);
	return out;
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


void MainWindow::findface(Mat &src)
{
	//面部识别XML文件
	string cascadeName = "D:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	//Mat image;//opencv中存储数据的基本单位，存储帧，图片等，代替旧版本的IplImage  
	//string inputName(imagefile.toLocal8Bit().data());//示例图片，放在当前目录  

	CascadeClassifier faceCascade;//定义级联分类器，由它们实现检测功能  
	double scale = 1;//不缩小图片，这样可以提高准确率  
	if (!faceCascade.load(cascadeName))//载入xml训练数据  
	{
		QMessageBox::information(NULL, "警告", "载入XML失败",  QMessageBox::Yes);
	}
	//image = imread(inputName, CV_LOAD_IMAGE_COLOR);//读取图片，第二个参数说明是彩色图片  
	cvNamedWindow("result", 1);//创建窗口，命名result，id为1  
	if (!src.empty())
	{
		detectAndDraw(src, faceCascade, scale);//进行识别  
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


void MainWindow::posDetect(const Mat &in, vector<RotatedRect> & rects) //寻找号码区域

{

//	Mat threshold_R;
//	OstuBeresenThreshold(in, threshold_R); //二值化
	Mat threshold_R = Ada_Thresgold(in);

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
	Mat out, roi_img;
	    in.copyTo(out);
		vector<Rect> numbers;
	    Point2f vertices[4];
	    rects[0].points(vertices);
		for (int i = 0; i < 4; i++)
			//  line(out, vertices[i], vertices[(i+1)%4], Scalar(0,0,0));//画黑色线条

			//	Point2f size = (10, 80);
			//0 左下角 1 左上角 2 右上角 3 右下角
			int px, y, pw, ph = 0;
		int px = vertices[1].x-5;
		int py = vertices[1].y-5;
		int pw = (vertices[2].x - vertices[0].x)+10;
		int ph = (vertices[3].y - vertices[2].y)+10;
		Rect imgr(px, py, pw, ph);

		cv::rectangle(out, imgr, Scalar(255, 255, 0), 1, 8, 0);//绘制方框
		out(imgr).copyTo(roi_img); //拷贝矩形区域  
		imshow("NUMBER", roi_img);
		imwrite("d:/number_roi.jpg", roi_img);
	 //  imshow("NUMBER", out);
	



	   
}


bool MainWindow::isEligible(const RotatedRect &candidate) //判定身份证号码区域
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
}//判定身份证号码区域

void MainWindow::normalPosArea(const Mat &intputImg, RotatedRect &rects_optimal, Mat& output_area)
{
	float r, angle;

	angle = rects_optimal.angle;
	r = (float)rects_optimal.size.width / (float)(float)rects_optimal.size.height;
	if (r<1)
		angle = 90 + angle;
	Mat rotmat = getRotationMatrix2D(rects_optimal.center, angle, 1);//获得变形矩阵对象
	Mat img_rotated;
	warpAffine(intputImg, img_rotated, rotmat, intputImg.size(), CV_INTER_CUBIC);

	//裁剪图像
	Size rect_size = rects_optimal.size;

	if (r<1)
		std::swap(rect_size.width, rect_size.height);
	Mat  img_crop;
	getRectSubPix(img_rotated, rect_size, rects_optimal.center, img_crop);

	//用光照直方图调整所有裁剪得到的图像，使具有相同宽度和高度，适用于训练和分类
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

	// threshold(in_Inv ,img_threshold , 140,255 ,CV_THRESH_BINARY ); //反转黑白色
	threshold(in_Inv, img_threshold, 0, 255, CV_THRESH_OTSU); //大津法二值化

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


void MainWindow::getAnnXML() // 此函数仅需运行一次，目的是获得训练矩阵和标签矩阵，保存于ann_xml.xml中
{
	FileStorage fs("C:\\Users\\weiyi\\Desktop\\Id_recognition\\ann_xml.xml", FileStorage::WRITE);
	if (!fs.isOpened())
	{
		qDebug() << "failed to open " << "/n";
	}

	Mat  trainData;
	Mat classes = Mat::zeros(1, 550, CV_8UC1);   //1700*48维，也即每个样本有48个特征值
	char path[60];
	Mat img_read;
	for (int i = 0; i<10; i++)  //第i类
	{
		for (int j = 1; j< 51; ++j)  //i类中第j个，即总共有11类字符，每类有50个样本
		{
			sprintf(path, "C:\\Users\\weiyi\\Desktop\\Id_recognition\\Number_char\\%d\\%d (%d).png", i, i, j);
			img_read = imread(path, 0);
			Mat dst_feature;
			calcGradientFeat(img_read, dst_feature); //计算每个样本的特征矢量
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
	cv::resize(img, lowData, Size(8, 16)); //缩放到8*16

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

	Mat layerSizes(1, 3, CV_32SC1);     //3层神经网络
	layerSizes.at<int>(0) = trainData.cols;   //输入层的神经元结点数，设置为24
	layerSizes.at<int>(1) = nlayers; //1个隐藏层的神经元结点数，设置为24
	layerSizes.at<int>(2) = numCharacters; //输出层的神经元结点数为:10
	ann.create(layerSizes, CvANN_MLP::SIGMOID_SYM, 1, 1);  //初始化ann
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
		Mat output(1, 10, CV_32FC1); //1*10矩阵

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
		mod += char_result[i] * wights[i];//乘相应系数求和

	mod = mod % 11; //对11求余

	int value[11] = { 1, 0, 10, 9, 8, 7, 6, 5, 4, 3, 2 };
	char_result[17] = value[mod];

}


void MainWindow::calcGradientFeat(const Mat &imgSrc, Mat &out)
{
	vector <float>  feat;
	Mat image;

	//cvtColor(imgSrc,image,CV_BGR2GRAY);
	cv::resize(imgSrc, image, Size(8, 16));

	// 计算x方向和y方向上的滤波
	float mask[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

	Mat y_mask = Mat(3, 3, CV_32F, mask) / 8;
	Mat x_mask = y_mask.t(); // 转置
	Mat sobelX, sobelY;

	filter2D(image, sobelX, CV_32F, x_mask);
	filter2D(image, sobelY, CV_32F, y_mask);

	sobelX = abs(sobelX);
	sobelY = abs(sobelY);

	float totleValueX = sumMatValue(sobelX);
	float totleValueY = sumMatValue(sobelY);

	// 将图像划分为4*2共8个格子，计算每个格子里灰度值总和的百分比
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

	//计算第2个特征
	Mat imageGray;
	//cvtColor(imgSrc,imageGray,CV_BGR2GRAY);
	cv::resize(imgSrc, imageGray, Size(4, 8));
	Mat p = imageGray.reshape(1, 1);
	p.convertTo(p, CV_32FC1);
	for (int i = 0; i<p.cols; i++)
	{
		feat.push_back(p.at<float>(i));
	}

	//增加水平直方图和垂直直方图
	Mat vhist = projectHistogram(imgSrc, 1); //水平直方图
	Mat hhist = projectHistogram(imgSrc, 0);  //垂直直方图
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


//读取图像函数 返回彩色MAT
Mat MainWindow::srcimg_color(QString &filename)
{
	Mat srcimg = imread(filename.toLocal8Bit().data(), CV_LOAD_IMAGE_COLOR);
	return srcimg;

}
//读取图片 灰度
Mat MainWindow::srcimg_gray(QString &filename)
{
	Mat srcimg = imread(filename.toLocal8Bit().data(), CV_LOAD_IMAGE_GRAYSCALE);
	return srcimg;
}