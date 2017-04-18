#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif
//设置编码 使用中文


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cv.h>
#include <highgui.h>


#include<qmessagebox.h>
#include <QTextCodec>
#include <QFileDialog>
#include"qt_windows.h"
#include<qdebug.h>
#include <QLabel>


#include "opencv2/objdetect/objdetect.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
#include <cctype>  
#include <iostream>  
#include <iterator>  
#include <stdio.h>  

#include <fstream>

#include <baseapi.h>  
#include <iostream>  


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

	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_pushButton2_clicked()));
	//格式限制
	//id
	QRegExp id_regx("[a-zA-Z0-9]+$");
	QValidator *validator = new QRegExpValidator(id_regx, ui->number_lineEdit);
	ui->number_lineEdit->setValidator(validator);

	// BIR
	QRegExp bir_regx("[\\x4e00-\\x9fa5]+");
	QValidator *bir_validator = new QRegExpValidator(bir_regx, ui->date_lineEdit);
	ui->date_lineEdit->setValidator(bir_validator);
	//NATION
	QRegExp nation_regx("[a-zA-Z0-9]+$");
	QValidator *nation_validator = new QRegExpValidator(nation_regx, ui->mz_lineEdit);
	ui->mz_lineEdit->setValidator(nation_validator);


    msgLabel = new QLabel;
    msgLabel->setMinimumSize(msgLabel->sizeHint());
    msgLabel->setAlignment(Qt::AlignHCenter);
    statusBar()->addWidget(msgLabel);
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
		get_name(c_src);
		get_add(c_src);
		get_nation(c_src);
        findface(c_src);
		find_number_area(g_src);
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
		cv::resize(srcimg, srcimg, Size(856, 540));
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

    Mat MainWindow::detectAndDraw_face(Mat& img, CascadeClassifier& faceCascade, double scale)
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

		if (faces.size() == 0)
		{
			QMessageBox::information(NULL, "警告", "找不到面部", QMessageBox::Yes);
		}
		else
		{
			for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
			{
			


				//控制矩形框位置
				int RectPosition_x = faces[0].x;
				int RectPosition_y = faces[0].y;
	// 358*441
				Rect imgr(RectPosition_x - 40, RectPosition_y - 60,358*0.71, 441*0.71);
				cv::rectangle(img, imgr, Scalar(0, 255, 0), 1, 8, 0);//绘制方框
				//控制矩形框位置
				img(imgr).copyTo(roi_img); //拷贝矩形区域				
			}

			imwrite("d:/face_roi.jpg", roi_img);
			show_img_label2(roi_img);

			return roi_img;
		
		}
    }

    void MainWindow::findface(Mat &src)
    {
        //面部识别XML文件
        string cascadeName = ".\\haarcascade_frontalface_alt.xml";
      

        CascadeClassifier faceCascade;//定义级联分类器，由它们实现检测功能
        double scale = 1;//不缩小图片，这样可以提高准确率
        if (!faceCascade.load(cascadeName))//载入xml训练数据
        {
            QMessageBox::information(NULL, "警告", "载入XML失败", QMessageBox::Yes);
        }
       
        if (!src.empty())
        {
			face_public = detectAndDraw_face(src, faceCascade, scale);//进行识别
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

    //
    //号码
    void MainWindow::find_number_area(const Mat src_gray)
    {
        int thresh = 100;
       
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

		if (contours.empty() == true)
		{
			QMessageBox::information(NULL, "警告", "找不到号码区域", QMessageBox::Yes);
			return;
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

        tesseract::TessBaseAPI tess;
        tess.Init(NULL, "num+eng", tesseract::OEM_DEFAULT);
        tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);
        tess.SetImage((uchar*)roi_img.data, roi_img.cols, roi_img.rows, 1, roi_img.cols);


        // Get the text
        char* number_out = tess.GetUTF8Text();
		char dob[9]="";
		dob[8] = 0;
		strncpy(dob, number_out + 6, 8);
		char sexbit[1] = "";
		sexbit[1] = 0;
		strncpy(sexbit, number_out + 16, 1);
		QString id_number(number_out);
		QString birth(dob);
		id_number = id_number.simplified();
		ui->number_lineEdit->setText(id_number);
		ui->date_lineEdit->setText(birth);
	
		switch (*sexbit)
		{
		case  '0':
		{
			ui->sex_lineEdit->setText("女");
			break;
		}
		case  '1':
		{
			ui->sex_lineEdit->setText("男");
			break;
		}
		case  '2':
		{
			ui->sex_lineEdit->setText("女");
			break;
		}
		case  '3':
		{
			ui->sex_lineEdit->setText("男");
			break;
		}
		case '4':
		{
			ui->sex_lineEdit->setText("女");
			break;
		}
		case '5':
		{
			ui->sex_lineEdit->setText("男");
			break;
		}
		case '6':
		{
			ui->sex_lineEdit->setText("女");
			break;
		}
		case '7':
		{
			ui->sex_lineEdit->setText("男");
			break;
		}
		case '8':
		{
			ui->sex_lineEdit->setText("女");
			break;
		}
		case '9':
		{
			ui->sex_lineEdit->setText("男");
			break;
		}
		
		default:
		{
			ui->sex_lineEdit->setText("ERROR");
			break; }
		}
		
		
    }

    bool MainWindow::is_number_area(const RotatedRect &candidate) //判定身份证号码区域
    {
        float error = 0.2;
        const double aspect = 320 / 35; //长宽比  4.5 / 0.3
        int min = 20 * aspect * 20; //最小区域 10 * aspect * 10
        int max = 500 * aspect * 500;  //最大区域 50 * aspect * 50
        float rmin = aspect - aspect*error; //考虑误差后的最小长宽比
        float rmax = aspect + aspect*error; //考虑误差后的最大长宽比

        int area = candidate.size.height * candidate.size.width;
        float r = (float)candidate.size.width / (float)candidate.size.height;
        if (r <1)
            r = 1 / r;

        if ((area < min || area > max) || (r< rmin || r > rmax)) //满足该条件
            return false;
        else
            return true;
    }//判定身份证号码区域
    

	std::string MainWindow::remove_all_bed(string &src)
	{
		int begin = 0;

		begin = src.find("\n", begin);  //查找空格在str中第一次出现的位置

		while (begin != -1)  //表示字符串中存在空格

		{
			src.replace(begin, 1, "");  // 用空串替换str中从begin开始的1个字符
			begin = src.find("\n", begin);  //查找空格在替换后的str中第一次出现的位置

		}
		return src;
	}

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

		id = remove_all_bed(id);

		std::string name = ui->name_lineEdit->text().toStdString();
	
		std::string dob = ui->date_lineEdit->text().toStdString();

		std::string add = ui->add_textEdit->toPlainText().toStdString();

		std::string sex = ui->sex_lineEdit->text().toStdString();

		std::string nation = ui->mz_lineEdit->text().toStdString();

		std::string face_filename = "./face/" + id + ".jpg";
		imwrite(face_filename, face_public);


		if (id!="")
		{
				sql::Statement* stat = conn->createStatement();
				stat->execute("set names 'utf8'");
				
				int updateCount = 0;
				bool retstatus = stat->execute("INSERT INTO idinfo(id,name,sex,nation,dob,address,img) VALUES ('" + id + "','" + name + "', '"+ sex +"','"+ nation +"','"+ dob +"','"+ add +"','" + face_filename + "')");
				if (!retstatus) {
						updateCount = stat->getUpdateCount();
						QMessageBox::information(NULL, "成功", "写入成功", QMessageBox::Yes);
								}
		}

		else
		{
			QMessageBox::information(NULL, "警告", "身份证号为空", QMessageBox::Yes);
		}	
	}


	Mat MainWindow::cut_name_img(const Mat &src)
	{
		Mat roi_img;
		Mat name_src = src;
		Mat resize_src;
		cv::resize(name_src, resize_src, Size(856, 540));

		int x = 150;
		int y = 40;
		int w = 200;
		int h =	65;
		Rect imgr(x,y,w,h);

		cv::rectangle(resize_src, imgr, Scalar(0, 255, 0), 1, 8, 0);//绘制方框
		resize_src(imgr).copyTo(roi_img); //拷贝矩形区域
		return roi_img;
	}

	void MainWindow::get_name(const Mat &src)
	{
		Mat name_roi = cut_name_img(src);
	
		cvtColor(name_roi, name_roi, CV_RGB2GRAY);
		//模糊降噪
		blur(name_roi, name_roi, Size(2, 2));
		tesseract::TessBaseAPI tess;
		tess.Init(NULL, "chi_sim", tesseract::OEM_DEFAULT);
		tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);
		tess.SetImage((uchar*)name_roi.data, name_roi.cols, name_roi.rows, 1, name_roi.cols);

		/* Get the text*/
		char* outt = "";
		outt = tess.GetUTF8Text();

		QString name(outt);
		name = name.simplified();
		
		ui->name_lineEdit->setText(name);
	}

	Mat MainWindow::cut_add_img(const Mat &src)
	{
		Mat roi_img;
		Mat add_src = src;
		Mat resize_src;
		cv::resize(add_src, resize_src, Size(856, 540));

		int x = 150;
		int y = 265;
		int w = 382;
		int h = 145;

		Rect imgr(x, y, w, h);

		cv::rectangle(resize_src, imgr, Scalar(0, 255, 0), 1, 8, 0);//绘制方框
		resize_src(imgr).copyTo(roi_img); //拷贝矩形区域
		return roi_img;
	}

	void MainWindow::get_add(const Mat &src)
	{
		Mat add_roi = cut_add_img(src);
		
		int w = add_roi.cols*0.8;
		int h = add_roi.rows*0.8;
		cv::resize(add_roi, add_roi,Size(w,h));
		cvtColor(add_roi, add_roi, CV_RGB2GRAY);
		add_roi = Ada_Thresgold(add_roi);
		//模糊降噪
		blur(add_roi, add_roi, Size(2, 2));
		
		tesseract::TessBaseAPI tess;
		tess.Init(NULL, "add+chi_sim", tesseract::OEM_DEFAULT);
		tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
		tess.SetImage((uchar*)add_roi.data, add_roi.cols, add_roi.rows, 1, add_roi.cols);

		/* Get the text*/
		char* outt = "";
		outt = tess.GetUTF8Text();

		QString aa(outt);
		ui->add_textEdit->setText(aa);
	}

	Mat MainWindow::cut_nation_img(const Mat &src)
	{
		Mat roi_img;
		Mat add_src = src;
		Mat resize_src;
		cv::resize(add_src, resize_src, Size(856, 540));

		int x = 335;
		int y = 115;
		int w = 85;
		int h = 70;

		Rect imgr(x, y, w, h);
		cv::rectangle(resize_src, imgr, Scalar(0, 255, 0), 1, 8, 0);//绘制方框
		resize_src(imgr).copyTo(roi_img); //拷贝矩形区域
		return roi_img;
	}

	void MainWindow::get_nation(const Mat &src)
	{
		Mat nation_roi = cut_nation_img(src);
		cvtColor(nation_roi, nation_roi, CV_RGB2GRAY);
		//模糊降噪
		blur(nation_roi, nation_roi, Size(2, 2));

		tesseract::TessBaseAPI tess;
		tess.Init(NULL, "chi_sim", tesseract::OEM_DEFAULT);
		tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);
		tess.SetImage((uchar*)nation_roi.data, nation_roi.cols, nation_roi.rows, 1, nation_roi.cols);

		/* Get the text*/
		char* outt = "";
		outt = tess.GetUTF8Text();

		QString aa(outt);
		ui->mz_lineEdit->setText(aa);
	}

	void MainWindow::tess_test(const Mat& src){
	
		tesseract::TessBaseAPI tess;
		tess.Init(NULL, "chi_sim + eng", tesseract::OEM_DEFAULT);
		tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);
		tess.SetImage((uchar*)src.data, src.cols, src.rows, 1,src.cols);

		// Get the text
		char* number_out = tess.GetUTF8Text();
		QString id_number(number_out);
	
		ui->number_lineEdit->setText(id_number);
	}