#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif
//设置编码 使用中文


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cv.h>
#include <highgui.h>
#include<qfiledialog.h>
#include<qmessagebox.h>
#include <QTextCodec>

#include <QFileDialog>
#include"qt_windows.h"
#include "opencv2/objdetect/objdetect.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
#include <cctype>  
#include <iostream>  
#include <iterator>  
#include <stdio.h>  
#include<qdebug.h>
#include <QLabel>
#include<qprogressbar>

#include <baseapi.h>  
#include <iostream>  

//#include <allheaders.h>

/* MySQL Connector/C++ specific headers */
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/driver.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/metadata.h"
#include "cppconn/exception.h"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     sql::mysql::MySQL_Driver *driver = 0;
    sql::Connection  *conn = 0;

    ui->setupUi(this);

    msgLabel = new QLabel;
    msgLabel->setMinimumSize(msgLabel->sizeHint());
    msgLabel->setAlignment(Qt::AlignHCenter);
//	msgLabel->setText(tr("test"));
    statusBar()->addWidget(msgLabel);
//	statusBar()->showMessage("11111111", 3000);//3s后关闭
    QLabel *per1 = new QLabel("就绪", this);
    statusBar()->addPermanentWidget(per1);
    statusBar()->setSizeGripEnabled(false); //设置是否显示右边的大小控制点

	
	try
	{
		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("tcp://localhost:3306/test", "root", "123456");
		per1->setText("数据库连接成功");
	}
	catch (...)
	{
		per1->setText("数据库连接失败");
	}

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString imagefile = QFileDialog::getOpenFileName(this, "打开图像", QDir::currentPath(), "Document files (*.jpg *.bmp);;All files(*.*)");		//打开图像

    if (!imagefile.isNull())
    {
        Mat c_src = srcimg_color(imagefile);	//载入彩色图像

        Mat g_src = srcimg_gray(imagefile);		//载入灰度图像

        show_img_label(c_src);//显示图像函数

        findface(c_src);

        find_name_area(g_src);
        find_add_area(g_src);

    }
	

}

void MainWindow::on_pushButton2_clicked()
{ 
	
	write_db();
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
        Mat src_out;
        cv::resize(srcimg, src_out, Size(856 * 0.8, 540 * 0.8));
        return src_out;
    }

    //显示图像在预览框
    void MainWindow::show_img_label(Mat &mat)
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

        ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->width(), ui->label->height()));

        ui->label->resize(ui->label->pixmap()->size());
        ui->label->show();
    }		//

    //自适应二值阈值化
    Mat MainWindow::Ada_Thresgold(const Mat &src)
    {
        Mat  out;

        int adaptive_method = CV_ADAPTIVE_THRESH_MEAN_C;
        int threshold_type = CV_THRESH_BINARY;
        int block_size = 25;
        double offset = 15;
    //	double param1 = 5;

        cv::adaptiveThreshold(src, out, 255, adaptive_method, threshold_type, block_size, offset);				//二值化

        return out;
    }

    //人脸检测与识别

    void MainWindow::detectAndDraw(Mat& img, CascadeClassifier& faceCascade, double scale)
    {
        Mat roi_img;

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
            Rect imgr(RectPosition_x - 10, RectPosition_y - 10, RectPosition_w + 20, RectPosition_h + 20);

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
        string cascadeName = ".\\haarcascade_frontalface_alt.xml";
        //Mat image;//opencv中存储数据的基本单位，存储帧，图片等，代替旧版本的IplImage
        //string inputName(imagefile.toLocal8Bit().data());//示例图片，放在当前目录

        CascadeClassifier faceCascade;//定义级联分类器，由它们实现检测功能
        double scale = 1;//不缩小图片，这样可以提高准确率
        if (!faceCascade.load(cascadeName))//载入xml训练数据
        {
            QMessageBox::information(NULL, "警告", "载入XML失败", QMessageBox::Yes);
			
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
		
        ui->label_2->clear();
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

        ui->label_2->setPixmap(QPixmap::fromImage(img).scaled(ui->label_2->width(), ui->label_2->height()));
	
        ui->label_2->resize(ui->label_2->pixmap()->size());
        ui->label_2->show();


    }

    //姓名
    void MainWindow::find_name_area(const Mat &src_gray)
    {
        int thresh = 100;
    //	int max_thresh = 255;
        RNG rng(12345);


        //输入为灰度图
        //模糊降噪
        blur(src_gray, src_gray, Size(2, 2));

        Mat canny_output;
        Mat threshold_output;

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        /// 用Canny算子检测边缘
        Canny(src_gray, canny_output, thresh, thresh * 2, 3);

        threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);

        /*
        //内核设置
        矩形: MORPH_RECT
        交叉形: MORPH_CROSS
        椭圆形: MORPH_ELLIPSE
        */
        //膨胀图像
        Mat element = getStructuringElement(MORPH_RECT, Size(70, 20));
        dilate(canny_output, canny_output, element);





        /// 寻找轮廓
        findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        /// 绘出轮廓
        Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
        for (int i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        }


        /// 多边形逼近轮廓 + 获取矩形和圆形边界框
        vector<vector<Point> > contours_poly(contours.size());
        vector<Rect> boundRect(contours.size());


        for (int i = 0; i < contours.size(); i++)
        {
            approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
            boundRect[i] = boundingRect(Mat(contours_poly[i]));

        }


        for (int i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar(255, 255, 255);

            rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);

        }
        imshow("name_roi", drawing);


        vector< vector <Point> > ::iterator itc = contours.begin();
        vector< vector <Point> > ::iterator itc2 = contours.begin();
        vector <RotatedRect>  rects;


        while (itc != contours.end())
        {
            RotatedRect mr = minAreaRect(Mat(*itc)); //返回每个轮廓的最小有界矩形区域
            if (!is_name_area(mr))  //判断矩形轮廓是否符合要求
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
        Mat out(src_gray.cols, src_gray.rows, CV_8UC3);

        Mat ada_src = Ada_Thresgold(src_gray);

        ada_src.copyTo(out);
        vector<Rect> numbers;
        Point2f vertices[4];
        rects[0].points(vertices);
        for (int i = 0; i < 4; i++)

        {		//	line(out, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));//画黑色线条
        }

        cv::Rect roi_rect(vertices[1], vertices[3]);

        Mat roi_img;
        cv::rectangle(out, roi_rect, Scalar(255, 255, 0), 1, 8, 0);//绘制方框

        out(roi_rect).copyTo(roi_img); //拷贝矩形区域
        imshow("NAME", roi_img);     //
        imwrite("d:/name_roi.jpg", roi_img);

	
		
        tesseract::TessBaseAPI tess;
        tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
        tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
        tess.SetImage((uchar*)roi_img.data, roi_img.cols, roi_img.rows, 1, roi_img.cols);

        // Get the text
        char* outt = "";
        outt = tess.GetUTF8Text();
		
        QString aa(outt);
        ui->name_lineEdit->setText(aa);

        namedWindow("name", CV_WINDOW_AUTOSIZE);
        imshow("name", out);
		
    }

    bool MainWindow::is_name_area(const RotatedRect &candidate) //判定身份证号码区域
    {
        float error = 0.3;
        const float aspect = 3.9; //长宽比
        int min = 60 * aspect * 40; //最小区域
        int max = 150 * aspect * 60;  //最大区域
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
    //

    //地址
    void MainWindow::find_add_area(const Mat &src_gray)
    {
        int thresh = 100;
        int max_thresh = 255;
        RNG rng(12345);

        //输入为灰度图
        //模糊降噪
        blur(src_gray, src_gray, Size(2, 2));

        Mat canny_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        /// 用Canny算子检测边缘
        Canny(src_gray, canny_output, thresh, thresh * 2, 3);


        /*
        //内核设置
        矩形: MORPH_RECT
        交叉形: MORPH_CROSS
        椭圆形: MORPH_ELLIPSE
        */
        //膨胀图像
        Mat element = getStructuringElement(MORPH_RECT, Size(15, 16));
        dilate(canny_output, canny_output, element);

        /// 寻找轮廓
        findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        /// 绘出轮廓
        Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
        for (int i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        }

        /// 多边形逼近轮廓 + 获取矩形和圆形边界框
        vector<vector<Point> > contours_poly(contours.size());
        vector<Rect> boundRect(contours.size());


        for (int i = 0; i < contours.size(); i++)
        {
            approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
            boundRect[i] = boundingRect(Mat(contours_poly[i]));

        }


        for (int i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar(255, 255, 255);

            rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);

        }
        imshow("add_roi", drawing);


        vector< vector <Point> > ::iterator itc = contours.begin();
        vector< vector <Point> > ::iterator itc2 = contours.begin();


        vector <RotatedRect>  rects;

        while (itc != contours.end())
        {
            RotatedRect mr = minAreaRect(Mat(*itc)); //返回每个轮廓的最小有界矩形区域
            if (!is_add_area(mr))  //判断矩形轮廓是否符合要求
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
        Mat out(src_gray.cols, src_gray.rows, CV_8UC3);

        Mat ada_src = Ada_Thresgold(src_gray);

        ada_src.copyTo(out);
        vector<Rect> numbers;
        Point2f vertices[4];
        rects[0].points(vertices);
        for (int i = 0; i < 4; i++)

        {		//	line(out, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));//画黑色线条
        }

        cv::Rect roi_rect(vertices[1], vertices[3]);

        Mat roi_img;
        cv::rectangle(out, roi_rect, Scalar(255, 255, 0), 1, 8, 0);//绘制方框

        out(roi_rect).copyTo(roi_img); //拷贝矩形区域
        imshow("NAME", roi_img);     //
        imwrite("d:/add_roi.jpg", roi_img);

        tesseract::TessBaseAPI tess;
        tess.Init(NULL, "chi_sim", tesseract::OEM_DEFAULT);
        tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
        tess.SetImage((uchar*)roi_img.data, roi_img.cols, roi_img.rows, 1, roi_img.cols);


        // Get the text
        char* outt = tess.GetUTF8Text();
        QString aa(outt);
        ui->add_textEdit->setText(aa);

        namedWindow("name", CV_WINDOW_AUTOSIZE);
        imshow("name", out);
    }

    bool MainWindow::is_add_area(const RotatedRect &candidate) //判定地址区域
    {
        float error = 0.5;
        const float aspect = 20 / 10; //长宽比
        int min = 80 * aspect * 80; //最小区域
        int max = 250 * aspect * 250;  //最大区域
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
    //


    //性别

    void MainWindow::find_sex_area(const Mat &src_gray)
    {
        int thresh = 100;
        int max_thresh = 255;
        RNG rng(12345);


        //输入为灰度图
        //模糊降噪
        blur(src_gray, src_gray, Size(2, 2));

        Mat canny_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        /// 用Canny算子检测边缘
        Canny(src_gray, canny_output, thresh, thresh * 2, 3);


        /*
        //内核设置
        矩形: MORPH_RECT
        交叉形: MORPH_CROSS
        椭圆形: MORPH_ELLIPSE
        */
        //膨胀图像
        Mat element = getStructuringElement(MORPH_RECT, Size(145, 25));
        dilate(canny_output, canny_output, element);



        /// 寻找轮廓
        findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        /// 绘出轮廓
        Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
        for (int i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        }


        /// 多边形逼近轮廓 + 获取矩形和圆形边界框
        vector<vector<Point> > contours_poly(contours.size());
        vector<Rect> boundRect(contours.size());


        for (int i = 0; i < contours.size(); i++)
        {
            approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
            boundRect[i] = boundingRect(Mat(contours_poly[i]));

        }


        for (int i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar(255, 255, 255);

            rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);

        }
        imshow("test", drawing);


        vector< vector <Point> > ::iterator itc = contours.begin();
        vector< vector <Point> > ::iterator itc2 = contours.begin();

        vector<RotatedRect>  rects;

        while (itc != contours.end())
        {
            RotatedRect mr = minAreaRect(Mat(*itc)); //返回每个轮廓的最小有界矩形区域
            if (!is_sex_area(mr))  //判断矩形轮廓是否符合要求
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
        Mat out(src_gray.cols, src_gray.rows, CV_8UC3);

        Mat ada_src = Ada_Thresgold(src_gray);

        ada_src.copyTo(out);
        vector<Rect> numbers;
        Point2f vertices[4];
        rects[0].points(vertices);
        for (int i = 0; i < 4; i++)

        {		//	line(out, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));//画黑色线条
        }

        cv::Rect roi_rect(vertices[1], vertices[3]);

        Mat roi_img;
        cv::rectangle(out, roi_rect, Scalar(255, 255, 0), 1, 8, 0);//绘制方框

        out(roi_rect).copyTo(roi_img); //拷贝矩形区域
        imshow("NAME", roi_img);     //
        imwrite("d:/name_roi.jpg", roi_img);

        tesseract::TessBaseAPI tess;
        tess.Init(NULL, "chi_sim", tesseract::OEM_DEFAULT);
        tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);
        tess.SetImage((uchar*)roi_img.data, roi_img.cols, roi_img.rows, 1, roi_img.cols);


        // Get the text
        char* outt = tess.GetUTF8Text();

        QString sex(outt);

        //int i = sex.indexOf("男");

        //	QString sex_result = sex.mid(i, 1);
    //	ui->sex_lineEdit->setText(sex);



        namedWindow("name", CV_WINDOW_AUTOSIZE);
        imshow("name", out);
    }

    bool MainWindow::is_sex_area(const RotatedRect & candidate)
    {
        float error = 0.2;
        const float aspect = 370 / 60; //长宽比  368/ 57
        int min = 60 * aspect * 50; //最小区域 10 * aspect * 10
        int max = 120 * aspect * 80;  //最大区域 50 * aspect * 50
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
    //

    //号码
    void MainWindow::find_number_area(const Mat src_gray)
    {
        int thresh = 100;
        int max_thresh = 255;
        RNG rng(12345);

        //输入为灰度图
        //模糊降噪
        blur(src_gray, src_gray, Size(2, 2));
        //膨胀图像
        Mat canny_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        /// 用Canny算子检测边缘
        Canny(src_gray, canny_output, thresh, thresh * 2, 3);


        /*
        //内核设置
        矩形: MORPH_RECT
        交叉形: MORPH_CROSS
        椭圆形: MORPH_ELLIPSE
        */
        Mat element = getStructuringElement(MORPH_RECT, Size(15, 20));
        dilate(canny_output, canny_output, element);



        /// 寻找轮廓
        findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));



        /// 绘出轮廓
        Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
        for (int i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        }


        /// 多边形逼近轮廓 + 获取矩形和圆形边界框
        vector<vector<Point> > contours_poly(contours.size());
        vector<Rect> boundRect(contours.size());


        for (int i = 0; i < contours.size(); i++)
        {
            approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
            boundRect[i] = boundingRect(Mat(contours_poly[i]));

        }


        for (int i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar(255, 255, 255);

            rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);

        }
        imshow("number_roi", drawing);

        vector< vector <Point> > ::iterator itc = contours.begin();
        vector< vector <Point> > ::iterator itc2 = contours.begin();


        vector <RotatedRect>  rects;

        while (itc != contours.end())
        {
            RotatedRect mr = minAreaRect(Mat(*itc)); //返回每个轮廓的最小有界矩形区域
            if (!is_number_area(mr))  //判断矩形轮廓是否符合要求
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
        Mat out(src_gray.cols, src_gray.rows, CV_8UC3);

        Mat ada_src = Ada_Thresgold(src_gray);

        ada_src.copyTo(out);
        vector<Rect> numbers;
        Point2f vertices[4];
        rects[0].points(vertices);
        for (int i = 0; i < 4; i++)

        {		//	line(out, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));//画黑色线条
        }

        cv::Rect roi_rect(vertices[1], vertices[3]);

        Mat roi_img;
        cv::rectangle(out, roi_rect, Scalar(255, 255, 0), 1, 8, 0);//绘制方框

        out(roi_rect).copyTo(roi_img); //拷贝矩形区域
        imshow("NAME", roi_img);     //
        imwrite("d:/add_roi.jpg", roi_img);

        tesseract::TessBaseAPI tess;
        tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
        tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);
        tess.SetImage((uchar*)roi_img.data, roi_img.cols, roi_img.rows, 1, roi_img.cols);


        // Get the text
        char* outt = tess.GetUTF8Text();
        QString aa(outt);
        ui->number_lineEdit->setText(aa);
		

        namedWindow("name", CV_WINDOW_AUTOSIZE);
        imshow("name", out);
    }

    bool MainWindow::is_number_area(const RotatedRect &candidate) //判定身份证号码区域
    {
        float error = 0.2;
        const float aspect = 320 / 35; //长宽比  4.5 / 0.3
        int min = 20 * aspect * 20; //最小区域 10 * aspect * 10
        int max = 500 * aspect * 500;  //最大区域 50 * aspect * 50
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
    //
	void MainWindow::write_db()
	{
		try
		{
			driver = sql::mysql::get_mysql_driver_instance();
			conn = driver->connect("tcp://localhost:3306/test", "root", "123456");
			
		}
		catch (...)
		{
			
		}

		std::string id = ui->number_lineEdit->text().toStdString();
		std::string name = ui->name_lineEdit->text().toStdString();
		

		if (id!="")
		{
				sql::Statement* stat = conn->createStatement();
				stat->execute("set names 'gbk'");

				//	res = stat->executeQuery("INSERT INTO book(bookname,size) VALUES ('1222234', 'Bill1')");
				int updateCount = 0;

				bool retstatus = stat->execute("INSERT INTO book(name,size) VALUES ('" + name + ", 'Bill1')");

				if (!retstatus) {
						updateCount = stat->getUpdateCount();
								}
		}

		else
		{
			QMessageBox::information(NULL, "警告", "身份证号为空", QMessageBox::Yes);
		}

		
	}