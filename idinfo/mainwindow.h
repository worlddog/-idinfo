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

	//载入图像
	
	Mat srcimg_color(QString &filename);	//彩色图像
	
	Mat srcimg_gray(QString &filename);		//灰度图像



	//显示图像
	void show_img_label(Mat &src);
	void show_img_label2(Mat &src);
	void display(cv::Mat &mat);
	///////////////////////////

	//图像处理
	Mat Ada_Thresgold(const Mat &src);//自适应二值阈值化

	//面部和眼睛检测并绘制矩形框，参数一次为图片数据，面部和眼部的级联分类器，检测时允许的图片缩小比例  
	void detectAndDraw(Mat& img, CascadeClassifier& faceCascade, double scale);
	void findface(Mat &src);


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
	
};

#endif // MAINWINDOW_H
