#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif
//���ñ��� ʹ������


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
//	statusBar()->showMessage("11111111", 3000);//3s��ر�
    QLabel *per1 = new QLabel("����", this);
    statusBar()->addPermanentWidget(per1);
    statusBar()->setSizeGripEnabled(false); //�����Ƿ���ʾ�ұߵĴ�С���Ƶ�

	
	try
	{
		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("tcp://localhost:3306/test", "root", "123456");
		per1->setText("���ݿ����ӳɹ�");
	}
	catch (...)
	{
		per1->setText("���ݿ�����ʧ��");
	}

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString imagefile = QFileDialog::getOpenFileName(this, "��ͼ��", QDir::currentPath(), "Document files (*.jpg *.bmp);;All files(*.*)");		//��ͼ��

    if (!imagefile.isNull())
    {
        Mat c_src = srcimg_color(imagefile);	//�����ɫͼ��

        Mat g_src = srcimg_gray(imagefile);		//����Ҷ�ͼ��

        show_img_label(c_src);//��ʾͼ����

        findface(c_src);

        find_name_area(g_src);
        find_add_area(g_src);

    }
	

}

void MainWindow::on_pushButton2_clicked()
{ 
	
	write_db();
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
        Mat src_out;
        cv::resize(srcimg, src_out, Size(856 * 0.8, 540 * 0.8));
        return src_out;
    }

    //��ʾͼ����Ԥ����
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

    //����Ӧ��ֵ��ֵ��
    Mat MainWindow::Ada_Thresgold(const Mat &src)
    {
        Mat  out;

        int adaptive_method = CV_ADAPTIVE_THRESH_MEAN_C;
        int threshold_type = CV_THRESH_BINARY;
        int block_size = 25;
        double offset = 15;
    //	double param1 = 5;

        cv::adaptiveThreshold(src, out, 255, adaptive_method, threshold_type, block_size, offset);				//��ֵ��

        return out;
    }

    //���������ʶ��

    void MainWindow::detectAndDraw(Mat& img, CascadeClassifier& faceCascade, double scale)
    {
        Mat roi_img;

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
            Rect imgr(RectPosition_x - 10, RectPosition_y - 10, RectPosition_w + 20, RectPosition_h + 20);

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
        string cascadeName = ".\\haarcascade_frontalface_alt.xml";
        //Mat image;//opencv�д洢���ݵĻ�����λ���洢֡��ͼƬ�ȣ�����ɰ汾��IplImage
        //string inputName(imagefile.toLocal8Bit().data());//ʾ��ͼƬ�����ڵ�ǰĿ¼

        CascadeClassifier faceCascade;//���弶����������������ʵ�ּ�⹦��
        double scale = 1;//����СͼƬ�������������׼ȷ��
        if (!faceCascade.load(cascadeName))//����xmlѵ������
        {
            QMessageBox::information(NULL, "����", "����XMLʧ��", QMessageBox::Yes);
			
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

    //����
    void MainWindow::find_name_area(const Mat &src_gray)
    {
        int thresh = 100;
    //	int max_thresh = 255;
        RNG rng(12345);


        //����Ϊ�Ҷ�ͼ
        //ģ������
        blur(src_gray, src_gray, Size(2, 2));

        Mat canny_output;
        Mat threshold_output;

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        /// ��Canny���Ӽ���Ե
        Canny(src_gray, canny_output, thresh, thresh * 2, 3);

        threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);

        /*
        //�ں�����
        ����: MORPH_RECT
        ������: MORPH_CROSS
        ��Բ��: MORPH_ELLIPSE
        */
        //����ͼ��
        Mat element = getStructuringElement(MORPH_RECT, Size(70, 20));
        dilate(canny_output, canny_output, element);





        /// Ѱ������
        findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        /// �������
        Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
        for (int i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        }


        /// ����αƽ����� + ��ȡ���κ�Բ�α߽��
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
            RotatedRect mr = minAreaRect(Mat(*itc)); //����ÿ����������С�н��������
            if (!is_name_area(mr))  //�жϾ��������Ƿ����Ҫ��
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
        Mat out(src_gray.cols, src_gray.rows, CV_8UC3);

        Mat ada_src = Ada_Thresgold(src_gray);

        ada_src.copyTo(out);
        vector<Rect> numbers;
        Point2f vertices[4];
        rects[0].points(vertices);
        for (int i = 0; i < 4; i++)

        {		//	line(out, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));//����ɫ����
        }

        cv::Rect roi_rect(vertices[1], vertices[3]);

        Mat roi_img;
        cv::rectangle(out, roi_rect, Scalar(255, 255, 0), 1, 8, 0);//���Ʒ���

        out(roi_rect).copyTo(roi_img); //������������
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

    bool MainWindow::is_name_area(const RotatedRect &candidate) //�ж����֤��������
    {
        float error = 0.3;
        const float aspect = 3.9; //�����
        int min = 60 * aspect * 40; //��С����
        int max = 150 * aspect * 60;  //�������
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
    //

    //��ַ
    void MainWindow::find_add_area(const Mat &src_gray)
    {
        int thresh = 100;
        int max_thresh = 255;
        RNG rng(12345);

        //����Ϊ�Ҷ�ͼ
        //ģ������
        blur(src_gray, src_gray, Size(2, 2));

        Mat canny_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        /// ��Canny���Ӽ���Ե
        Canny(src_gray, canny_output, thresh, thresh * 2, 3);


        /*
        //�ں�����
        ����: MORPH_RECT
        ������: MORPH_CROSS
        ��Բ��: MORPH_ELLIPSE
        */
        //����ͼ��
        Mat element = getStructuringElement(MORPH_RECT, Size(15, 16));
        dilate(canny_output, canny_output, element);

        /// Ѱ������
        findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        /// �������
        Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
        for (int i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        }

        /// ����αƽ����� + ��ȡ���κ�Բ�α߽��
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
            RotatedRect mr = minAreaRect(Mat(*itc)); //����ÿ����������С�н��������
            if (!is_add_area(mr))  //�жϾ��������Ƿ����Ҫ��
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
        Mat out(src_gray.cols, src_gray.rows, CV_8UC3);

        Mat ada_src = Ada_Thresgold(src_gray);

        ada_src.copyTo(out);
        vector<Rect> numbers;
        Point2f vertices[4];
        rects[0].points(vertices);
        for (int i = 0; i < 4; i++)

        {		//	line(out, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));//����ɫ����
        }

        cv::Rect roi_rect(vertices[1], vertices[3]);

        Mat roi_img;
        cv::rectangle(out, roi_rect, Scalar(255, 255, 0), 1, 8, 0);//���Ʒ���

        out(roi_rect).copyTo(roi_img); //������������
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

    bool MainWindow::is_add_area(const RotatedRect &candidate) //�ж���ַ����
    {
        float error = 0.5;
        const float aspect = 20 / 10; //�����
        int min = 80 * aspect * 80; //��С����
        int max = 250 * aspect * 250;  //�������
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
    //


    //�Ա�

    void MainWindow::find_sex_area(const Mat &src_gray)
    {
        int thresh = 100;
        int max_thresh = 255;
        RNG rng(12345);


        //����Ϊ�Ҷ�ͼ
        //ģ������
        blur(src_gray, src_gray, Size(2, 2));

        Mat canny_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        /// ��Canny���Ӽ���Ե
        Canny(src_gray, canny_output, thresh, thresh * 2, 3);


        /*
        //�ں�����
        ����: MORPH_RECT
        ������: MORPH_CROSS
        ��Բ��: MORPH_ELLIPSE
        */
        //����ͼ��
        Mat element = getStructuringElement(MORPH_RECT, Size(145, 25));
        dilate(canny_output, canny_output, element);



        /// Ѱ������
        findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        /// �������
        Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
        for (int i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        }


        /// ����αƽ����� + ��ȡ���κ�Բ�α߽��
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
            RotatedRect mr = minAreaRect(Mat(*itc)); //����ÿ����������С�н��������
            if (!is_sex_area(mr))  //�жϾ��������Ƿ����Ҫ��
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
        Mat out(src_gray.cols, src_gray.rows, CV_8UC3);

        Mat ada_src = Ada_Thresgold(src_gray);

        ada_src.copyTo(out);
        vector<Rect> numbers;
        Point2f vertices[4];
        rects[0].points(vertices);
        for (int i = 0; i < 4; i++)

        {		//	line(out, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));//����ɫ����
        }

        cv::Rect roi_rect(vertices[1], vertices[3]);

        Mat roi_img;
        cv::rectangle(out, roi_rect, Scalar(255, 255, 0), 1, 8, 0);//���Ʒ���

        out(roi_rect).copyTo(roi_img); //������������
        imshow("NAME", roi_img);     //
        imwrite("d:/name_roi.jpg", roi_img);

        tesseract::TessBaseAPI tess;
        tess.Init(NULL, "chi_sim", tesseract::OEM_DEFAULT);
        tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);
        tess.SetImage((uchar*)roi_img.data, roi_img.cols, roi_img.rows, 1, roi_img.cols);


        // Get the text
        char* outt = tess.GetUTF8Text();

        QString sex(outt);

        //int i = sex.indexOf("��");

        //	QString sex_result = sex.mid(i, 1);
    //	ui->sex_lineEdit->setText(sex);



        namedWindow("name", CV_WINDOW_AUTOSIZE);
        imshow("name", out);
    }

    bool MainWindow::is_sex_area(const RotatedRect & candidate)
    {
        float error = 0.2;
        const float aspect = 370 / 60; //�����  368/ 57
        int min = 60 * aspect * 50; //��С���� 10 * aspect * 10
        int max = 120 * aspect * 80;  //������� 50 * aspect * 50
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

    }
    //

    //����
    void MainWindow::find_number_area(const Mat src_gray)
    {
        int thresh = 100;
        int max_thresh = 255;
        RNG rng(12345);

        //����Ϊ�Ҷ�ͼ
        //ģ������
        blur(src_gray, src_gray, Size(2, 2));
        //����ͼ��
        Mat canny_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        /// ��Canny���Ӽ���Ե
        Canny(src_gray, canny_output, thresh, thresh * 2, 3);


        /*
        //�ں�����
        ����: MORPH_RECT
        ������: MORPH_CROSS
        ��Բ��: MORPH_ELLIPSE
        */
        Mat element = getStructuringElement(MORPH_RECT, Size(15, 20));
        dilate(canny_output, canny_output, element);



        /// Ѱ������
        findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));



        /// �������
        Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
        for (int i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        }


        /// ����αƽ����� + ��ȡ���κ�Բ�α߽��
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
            RotatedRect mr = minAreaRect(Mat(*itc)); //����ÿ����������С�н��������
            if (!is_number_area(mr))  //�жϾ��������Ƿ����Ҫ��
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
        Mat out(src_gray.cols, src_gray.rows, CV_8UC3);

        Mat ada_src = Ada_Thresgold(src_gray);

        ada_src.copyTo(out);
        vector<Rect> numbers;
        Point2f vertices[4];
        rects[0].points(vertices);
        for (int i = 0; i < 4; i++)

        {		//	line(out, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));//����ɫ����
        }

        cv::Rect roi_rect(vertices[1], vertices[3]);

        Mat roi_img;
        cv::rectangle(out, roi_rect, Scalar(255, 255, 0), 1, 8, 0);//���Ʒ���

        out(roi_rect).copyTo(roi_img); //������������
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

    bool MainWindow::is_number_area(const RotatedRect &candidate) //�ж����֤��������
    {
        float error = 0.2;
        const float aspect = 320 / 35; //�����  4.5 / 0.3
        int min = 20 * aspect * 20; //��С���� 10 * aspect * 10
        int max = 500 * aspect * 500;  //������� 50 * aspect * 50
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
			QMessageBox::information(NULL, "����", "���֤��Ϊ��", QMessageBox::Yes);
		}

		
	}