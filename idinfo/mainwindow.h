#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cv.h>
#include <highgui.h> 
#include <QLabel>
#include <iostream>
#include"qlabel.h"
/* MySQL Connector/C++ specific headers ��ע�⣺����ͷ�ļ�������/usr/include/cppconn/���棩*/
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

private slots:
    void on_pushButton_clicked();
	void on_pushButton2_clicked();

private:
    Ui::MainWindow *ui;
	QLabel *msgLabel;

	//����ͼ��
	
	Mat srcimg_color(QString &filename);	//��ɫͼ��
	
	Mat srcimg_gray(QString &filename);		//�Ҷ�ͼ��



	//��ʾͼ��
	void show_img_label(Mat &src);
	void show_img_label2(Mat &src);
	void display(cv::Mat &mat);
	///////////////////////////

	//ͼ����
	Mat Ada_Thresgold(const Mat &src);//����Ӧ��ֵ��ֵ��

	//�沿���۾���Ⲣ���ƾ��ο򣬲���һ��ΪͼƬ���ݣ��沿���۲��ļ��������������ʱ�����ͼƬ��С����  
	void detectAndDraw(Mat& img, CascadeClassifier& faceCascade, double scale);
	void findface(Mat &src);


	void find_name_area(const Mat &in);
	bool is_name_area(const RotatedRect & candidate);//�ж����֤���������Ƿ����Ԥ���С

	//Ѱ��סַ����
	void find_add_area(const Mat &src);
	bool is_add_area(const RotatedRect & candidate);

	//Ѱ�Һ�������
	void find_number_area(const Mat src_gray);
	bool is_number_area(const RotatedRect & candidate);

	//Ѱ���Ա�����
	void find_sex_area(const Mat &src_gray);
	bool is_sex_area(const RotatedRect & candidate);

	//���ݿ�д��
	void write_db();
	
};

#endif // MAINWINDOW_H
