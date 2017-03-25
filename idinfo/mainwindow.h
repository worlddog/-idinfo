#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cv.h>  
#include <highgui.h> 

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

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

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
	
};

#endif // MAINWINDOW_H
