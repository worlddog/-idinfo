#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cv.h>  
#include <highgui.h> 
#include<Qgraphicsview>

#include <QWidget>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml/ml.hpp"
using namespace cv; //使用OPENCV命名空间

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
    void on_testaction_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

	QImage * qImage; // qt 中的图片类型指针
	IplImage * image;  // opencv里的图片类型指针
	IplImage * tempImage;
	IplImage * image2;

	IplImage *Igray, *Iat;

	/////////////////////////////////////
	Mat imgSrc;

	Mat getRplane(const Mat & in);  //获得图片的R通道
	void Ada_Thresgold(const char* filename);//自适应二值阈值化
	void OstuBeresenThreshold(const Mat & in, Mat & out);
	void posDetect(const Mat &in, vector<RotatedRect> & rects);
	bool isEligible(const RotatedRect & candidate);
	void normalPosArea(const Mat &intputImg, RotatedRect &rects_optimal, Mat& output_area);
	void char_segment(const Mat & inputImg, vector <Mat>& dst_mat); //得到16*20的标准字符分割图像
	void getAnnXML();  //获得神经网络的训练矩阵和标签矩阵，

	void calcGradientFeat(const Mat& imgSrc, Mat& out);
	float sumMatValue(const Mat& image);
	Mat projectHistogram(const Mat& img, int t);

	void ann_train(CvANN_MLP &ann, int numCharacters, int nlayers);
	void classify(CvANN_MLP& ann, vector<Mat> &char_Mat, vector<int> & char_result);

	void getParityBit(vector<int> & char_result);  //获得校验位

	///////////////////////////////////////



};

#endif // MAINWINDOW_H
