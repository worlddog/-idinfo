#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cv.h>
#include <highgui.h> 
#include <QLabel>
#include <iostream>
#include"qlabel.h"
/* MySQL Connector/C++ specific headers （注意：所有头文件都放在/usr/include/cppconn/下面）*/
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/driver.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/metadata.h"
#include "cppconn/exception.h"

using namespace cv;

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	QLabel *per1;
	sql::mysql::MySQL_Driver *driver = 0;
	sql::Connection *conn = 0;
	Mat face_public;

private slots:
    void on_pushButton_clicked();
	void on_pushButton2_clicked();

private:
    Ui::MainWindow *ui;
	QLabel *msgLabel;

	//字符串处理
	std::string remove_all_bed(string &src);

	//载入图像
	
	Mat srcimg_color(QString &filename);	//彩色图像
	
	Mat srcimg_gray(QString &filename);		//灰度图像



	//显示图像
	void show_img_label(Mat &src);
	void show_img_label2(Mat &src);
	void display(cv::Mat &mat);
	///////////////////////////

	//图像处理

	Mat delete_roi(Mat &src, int &x,int &y,int &w,int &h);


	Mat Ada_Thresgold(const Mat &src);//自适应二值阈值化

	//面部和眼睛检测并绘制矩形框，参数一次为图片数据，面部和眼部的级联分类器，检测时允许的图片缩小比例  
	Mat detectAndDraw_face(Mat& img, CascadeClassifier& faceCascade, double scale);
	void findface(Mat &src);

	//寻找身份证
	void find_card_area(const Mat &in);
	bool is_card_area(const RotatedRect & candidate);
	
	//寻找姓名
	void find_name_area(const Mat &in);
	bool is_name_area(const RotatedRect & candidate);//判断身份证号码区域是否符合预设大小

	//寻找住址区域
	void find_add_area(const Mat &src);
	bool is_add_area(const RotatedRect & candidate);

	//寻找号码区域
	void find_number_area(const Mat src_gray);
	bool is_number_area(const RotatedRect & candidate);

	//寻找性别区域
	void find_sex_area(const Mat &src_gray);
	bool is_sex_area(const RotatedRect & candidate);

	//


	//裁剪各区域
	Mat cut_name_img(const Mat &src);
	Mat cut_sex_img(const Mat &src);
	Mat cut_add_img(const Mat &src);
	Mat cut_nation_img(const Mat &src);
	//Mat cut_sex_img(const Mat &src);
	//
	void get_name(const Mat &src);
	void get_sex(const Mat &src);
	void get_add(const Mat &src);
	void get_nation(const Mat &src);

	//数据库写入
	void write_db();

	//tess test
	void tess_test(const Mat& src);
	
};

#endif // MAINWINDOW_H
