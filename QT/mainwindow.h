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
class QLabel;
using namespace cv; //ʹ��OPENCV�����ռ�

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
	QLabel *statusLabel;
	


	QImage * qImage; // qt �е�ͼƬ����ָ��
	IplImage * image;  // opencv���ͼƬ����ָ��
	IplImage * tempImage;
	IplImage * image2;

	IplImage *Igray, *Iat;

	IplImage * detect_img;
	/////////////////////////////////////
	Mat imgSrc;

	Mat getRplane(const Mat & in);  //���ͼƬ��Rͨ��
	Mat Ada_Thresgold(const Mat &src);//����Ӧ��ֵ��ֵ��
	void OstuBeresenThreshold(const Mat & in, Mat & out);
	//


	//
	void normalPosArea(const Mat &intputImg, RotatedRect &rects_optimal, Mat& output_area);
	void char_segment(const Mat & inputImg, vector <Mat>& dst_mat); //�õ�16*20�ı�׼�ַ��ָ�ͼ��
	void getAnnXML();  //����������ѵ������ͱ�ǩ����
	bool is_face_area(const RotatedRect &candidate);

	void calcGradientFeat(const Mat& imgSrc, Mat& out);
	float sumMatValue(const Mat& image);
	Mat projectHistogram(const Mat& img, int t);

	void ann_train(CvANN_MLP &ann, int numCharacters, int nlayers);
	void classify(CvANN_MLP& ann, vector<Mat> &char_Mat, vector<int> & char_result);

	void getParityBit(vector<int> & char_result);  //���У��λ
	//
	Mat resize_img(Mat &src);
	/////
	//��ʾͼƬ��label
	Mat srcimg;
	Mat tempmat;
	void show_img_label(Mat &src);
	
	void display(cv::Mat mat);
	//��ʾͼƬ��ͷ���
	Mat roi_img;
	void show_img_label2(Mat &src);
		///////////////////////////////////////
	//�沿���۾���Ⲣ���ƾ��ο򣬲���һ��ΪͼƬ���ݣ��沿���۲��ļ��������������ʱ�����ͼƬ��С����  
	void detectAndDraw(Mat& img, CascadeClassifier& faceCascade,double scale);
	void findface(Mat &src);
	
	//////////////////////
	//ͼ�������븯ʴ 
	void posDetect(const char *filename, const Mat &in, vector<RotatedRect> & rects);
	
	//��ȡͼƬ���� ��ɫ
	Mat srcimg_color(QString &filename);
	//��ȡͼƬ �Ҷ�
	Mat srcimg_gray(QString &filename);

	void find_area(Mat src_gray);
	//

	//Ѱ�����֤����
	void find_card_area(const Mat &in, vector<RotatedRect> & rects);
	//Ѱ�����֤������ 1
	void posDetect(const Mat &in, vector<RotatedRect> & rects);
	//Ѱ�����֤������ 2
	void find_number_area(const Mat src);
	//Ѱ����������
	void find_name_area(const Mat &in, vector<RotatedRect> & rects);
	//Ѱ���Ա�����
	void find_sex_area(const Mat &in);
	//Ѱ��סַ����
	void find_add_area(const Mat src);


	//�ж����֤����
	bool is_card_area(const RotatedRect & candidate);
	//�жϺ�������
	bool is_number_area(const RotatedRect & candidate);//�ж����֤���������Ƿ����Ԥ���С
	//�ж���������
	bool is_name_area(const RotatedRect & candidate);//�ж����֤���������Ƿ����Ԥ���С
	//
	bool is_sex_area(const RotatedRect & candidate);//�ж����֤���������Ƿ����Ԥ���С
	//
	bool is_add_area(const RotatedRect & candidate);
};

#endif // MAINWINDOW_H
