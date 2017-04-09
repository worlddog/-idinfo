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
#include <fstream>

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

	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_pushButton2_clicked()));

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
	//	get_name(c_src);
	//	get_sex(c_src);
		get_add(c_src);
		/*for (int i = 100; i < 228; i++)
			for (int j = 300; j < 428; j++)
				for (int n = 0; n < c_src.channels(); n++)
					c_src.at<uchar>(i, j*c_src.channels() + n) = 255;
		imshow("ggggg", c_src);*/

     //   findface(c_src);
	/*	int a, b, c, d;
			a = 400;
			b = 100;
			c = 128;
		d = 128;
		delete_roi(c_src, a, b, c, d);*/

   //     find_name_area(g_src);	//ʶ������
	//	find_sex_area(g_src);
	//	find_number_area(g_src);
    //    find_add_area(g_src);


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

    Mat MainWindow::detectAndDraw_face(Mat& img, CascadeClassifier& faceCascade, double scale)
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

		if (faces.size() == 0)
		{
			QMessageBox::information(NULL, "����", "�Ҳ����沿", QMessageBox::Yes);
		}
		else
		{
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
				//	Rect rect(a.x,a.y,50, 50); // (����x, ����y, ����, �߶�)
				img(imgr).copyTo(roi_img); //������������
				//	imshow("cut", roi_img);
				
			}

			cv::imshow("result", img);//��img��ʾ��result����

			//	cv::imshow("cut", roi_img);//��ʾimg
		//	imwrite("d:/face_roi.jpg", roi_img);
			show_img_label2(roi_img);

			return roi_img;
		
		}
    }

    void MainWindow::findface(Mat &src)
    {
        //�沿ʶ��XML�ļ�
        string cascadeName = ".\\haarcascade_frontalface_alt.xml";
      

        CascadeClassifier faceCascade;//���弶����������������ʵ�ּ�⹦��
        double scale = 1;//����СͼƬ�������������׼ȷ��
        if (!faceCascade.load(cascadeName))//����xmlѵ������
        {
            QMessageBox::information(NULL, "����", "����XMLʧ��", QMessageBox::Yes);
			
        }
       
        cvNamedWindow("result", 1);//�������ڣ�����result��idΪ1
        if (!src.empty())
        {
			face_public = detectAndDraw_face(src, faceCascade, scale);//����ʶ��
			
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
	
		if (contours.empty() == true)
		{
			QMessageBox::information(NULL, "����", "�Ҳ�����������", QMessageBox::Yes);
			return;
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
    //    imshow("NAME", roi_img);     //
    //    imwrite("d:/name_roi.jpg", roi_img);

	
		
        tesseract::TessBaseAPI tess;
        tess.Init(NULL, "chi_sim", tesseract::OEM_DEFAULT);
        tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);
        tess.SetImage((uchar*)roi_img.data, roi_img.cols, roi_img.rows, 1, roi_img.cols);

        // Get the text
        char* outt = "";
        outt = tess.GetUTF8Text();
		
        QString aa(outt);
        ui->name_lineEdit->setText(aa);

   //     namedWindow("name", CV_WINDOW_AUTOSIZE);
    //    imshow("name", out);
		
    }

    bool MainWindow::is_name_area(const RotatedRect &candidate) //�ж�����֤��������
    {
        float error = 0.3;
        const float aspect = 3.9; //������
        int min = 60 * aspect * 40; //��С����
        int max = 150 * aspect * 60;  //�������
        float rmin = aspect - aspect*error; //�����������С������
        float rmax = aspect + aspect*error; //�����������󳤿���

        int area = candidate.size.height * candidate.size.width;
        float r = (float)candidate.size.width / (float)candidate.size.height;
        if (r <1)
            r = 1 / r;

        if ((area < min || area > max) || (r< rmin || r > rmax)) //�������������Ϊ��candidateΪ��������
            return false;
        else
            return true;
    }//�ж�����֤��������
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
		if (contours.empty() == true)
		{
			QMessageBox::information(NULL, "����", "�Ҳ�����ַ����", QMessageBox::Yes);
			return;
		}

        //�����Ƿ��ҵ���add����
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
       imshow("add", roi_img);     //
    //    imwrite("d:/add_roi.jpg", roi_img);

        tesseract::TessBaseAPI tess;
        tess.Init(NULL, "chi_sim", tesseract::OEM_DEFAULT);
        tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
        tess.SetImage((uchar*)roi_img.data, roi_img.cols, roi_img.rows, 1, roi_img.cols);


        // Get the text
        char* outt = tess.GetUTF8Text();
        QString aa(outt);
        ui->add_textEdit->setText(aa);

      //  namedWindow("add", CV_WINDOW_AUTOSIZE);
      //  imshow("add", out);
    }

    bool MainWindow::is_add_area(const RotatedRect &candidate) //�ж���ַ����
    {
        float error = 0.4;
        const float aspect = 3.1 / 1; //������
        int min = 80 * aspect * 80; //��С����
        int max = 200 * aspect * 200;  //�������
        float rmin = aspect - aspect*error; //�����������С������
        float rmax = aspect + aspect*error; //�����������󳤿���

        int area = candidate.size.height * candidate.size.width;
        float r = (float)candidate.size.width / (float)candidate.size.height;
        if (r <1)
            r = 1 / r;

        if ((area < min || area > max) || (r< rmin || r > rmax)) //�������������Ϊ��candidateΪ��������
            return false;
        else
            return true;
    }//�ж�����֤��������
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

		if (contours.empty() == true)
		{
			QMessageBox::information(NULL, "����", "�Ҳ����Ա����������", QMessageBox::Yes);
			return;
		}

        //�����Ƿ��ҵ���SEX����
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
     //   imshow("sex", roi_img);     //
        imwrite("d:/sex_roi.jpg", roi_img);

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



      //  namedWindow("name", CV_WINDOW_AUTOSIZE);
      //  imshow("name", out);
    }

    bool MainWindow::is_sex_area(const RotatedRect & candidate)
    {
        float error = 0.2;
        const float aspect = 370 / 60; //������  368/ 57
        int min = 60 * aspect * 50; //��С���� 10 * aspect * 10
        int max = 120 * aspect * 80;  //������� 50 * aspect * 50
        float rmin = aspect - aspect*error; //�����������С������
        float rmax = aspect + aspect*error; //�����������󳤿���

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

		if (contours.empty() == true)
		{
			QMessageBox::information(NULL, "����", "�Ҳ�����������", QMessageBox::Yes);
			return;
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
//        imshow("HUMBER_ROI", roi_img);     
 //       imwrite("d:/number_roi.jpg", roi_img);

        tesseract::TessBaseAPI tess;
        tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
        tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);
        tess.SetImage((uchar*)roi_img.data, roi_img.cols, roi_img.rows, 1, roi_img.cols);


        // Get the text
        char* outt = tess.GetUTF8Text();
        QString aa(outt);
        ui->number_lineEdit->setText(aa);
		

      //  namedWindow("number", CV_WINDOW_AUTOSIZE);
      //  imshow("number", out);
    }

    bool MainWindow::is_number_area(const RotatedRect &candidate) //�ж�����֤��������
    {
        float error = 0.2;
        const float aspect = 320 / 35; //������  4.5 / 0.3
        int min = 20 * aspect * 20; //��С���� 10 * aspect * 10
        int max = 500 * aspect * 500;  //������� 50 * aspect * 50
        float rmin = aspect - aspect*error; //�����������С������
        float rmax = aspect + aspect*error; //�����������󳤿���

        int area = candidate.size.height * candidate.size.width;
        float r = (float)candidate.size.width / (float)candidate.size.height;
        if (r <1)
            r = 1 / r;

        if ((area < min || area > max) || (r< rmin || r > rmax)) //�������������Ϊ��candidateΪ��������
            return false;
        else
            return true;
    }//�ж�����֤��������
    //

	std::string MainWindow::remove_all_bed(string &src)
	{
		int begin = 0;

		begin = src.find("\n", begin);  //���ҿո���str�е�һ�γ��ֵ�λ��

		while (begin != -1)  //��ʾ�ַ����д��ڿո�

		{

			src.replace(begin, 1, "");  // �ÿմ��滻str�д�begin��ʼ��1���ַ�

			begin = src.find("\n", begin);  //���ҿո����滻���str�е�һ�γ��ֵ�λ��

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
	
		std::string aaa = "fdef";

		

		std::string face_filename = "./face/" + id + ".jpg";
		imwrite(face_filename, face_public);

		/*ifstream face_img;
		int length;
		face_img.open("d:\\face_roi.jpg");
		face_img.seekg(0, std::ios::end);
		length = face_img.tellg();
		face_img.seekg(0, std::ios::beg);

		const int max_length = 1024 * 1024;
	
		char* buffer = new char[length];
	
		face_img.read(buffer, length);
		face_img.close();*/

		

		if (id!="")
		{
				sql::Statement* stat = conn->createStatement();
				stat->execute("set names 'gbk'");
				
			
				
				int updateCount = 0;

			//	bool retstatus = stat->execute("INSERT INTO idinfo(id,name,sex,nation,dob,address,img) VALUES ('"+id+"','asdfg', 'Bill1','sss','fdewf','sder','"+face_filename+"')");
				bool retstatus = stat->execute("INSERT INTO idinfo(id,name,sex,nation,dob,address,img) VALUES ('" + id + "','asdfg', 'Bill1','sss','fdewf','sder','" + face_filename + "')");
				if (!retstatus) {
						updateCount = stat->getUpdateCount();
								}
		}

		else
		{
			QMessageBox::information(NULL, "����", "����֤��Ϊ��", QMessageBox::Yes);
		}



		
	}


	Mat MainWindow::delete_roi(Mat &src, int &x, int &y, int &w, int &h)
	{
		for (int i = x; i < x+w; i++)
			for (int j = y; j < y+h; j++)
				for (int n = 0; n < src.channels(); n++)
			{
				src.at<uchar>(i, j*src.channels() + n) = 255;

			}
		imshow("sss", src);
		return src;
	}

	//
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

		cv::rectangle(resize_src, imgr, Scalar(0, 255, 0), 1, 8, 0);//���Ʒ���
	//	imshow("defd", name_src);
		resize_src(imgr).copyTo(roi_img); //������������
	/*	cv::resize(name_src, name_src, Size(856*0.5, 540*0.5));*/
		//imshow("sss",roi_img);
		return roi_img;
	}

	void MainWindow::get_name(const Mat &src)
	{
		Mat name_roi = cut_name_img(src);
	//	imshow("sss", name_roi);
		cvtColor(name_roi, name_roi, CV_RGB2GRAY);
		//ģ������
		blur(name_roi, name_roi, Size(2, 2));
		//imshow("defd", name_roi);

		tesseract::TessBaseAPI tess;
		tess.Init(NULL, "chi_sim", tesseract::OEM_DEFAULT);
		tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);
		tess.SetImage((uchar*)name_roi.data, name_roi.cols, name_roi.rows, 1, name_roi.cols);

		/* Get the text*/
		char* outt = "";
		outt = tess.GetUTF8Text();

		QString aa(outt);
		ui->name_lineEdit->setText(aa);
	}



	Mat MainWindow::cut_sex_img(const Mat &src)
	{
		Mat roi_img;
		Mat sex_src = src;
		Mat resize_src;
		cv::resize(sex_src, resize_src, Size(856, 540));

		int x = 150;
		int y = 115;
		int w = 85;
		int h = 70;
		Rect imgr(x, y, w, h);

		cv::rectangle(resize_src, imgr, Scalar(0, 255, 0), 1, 8, 0);//���Ʒ���
		imshow("defd", resize_src);
		resize_src(imgr).copyTo(roi_img); //������������
		/*	cv::resize(name_src, name_src, Size(856*0.5, 540*0.5));*/
	//	imshow("sss",roi_img);
		return roi_img;
	}

	void MainWindow::get_sex(const Mat &src)
	{
		Mat sex_roi = cut_sex_img(src);
			imshow("sss", sex_roi);
		cvtColor(sex_roi, sex_roi, CV_RGB2GRAY);
		//ģ������
		blur(sex_roi, sex_roi, Size(2, 2));
		//imshow("defd", name_roi);

		tesseract::TessBaseAPI tess;
		tess.Init(NULL, "chi_sim", tesseract::OEM_DEFAULT);
		tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);
		tess.SetImage((uchar*)sex_roi.data,sex_roi.cols, sex_roi.rows, 1, sex_roi.cols);

		/* Get the text*/
		char* outt = "";
		outt = tess.GetUTF8Text();

		QString aa(outt);
		ui->sex_lineEdit->setText(aa);
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

		cv::rectangle(resize_src, imgr, Scalar(0, 255, 0), 1, 8, 0);//���Ʒ���
		imshow("defd", resize_src);
		resize_src(imgr).copyTo(roi_img); //������������
		/*	cv::resize(name_src, name_src, Size(856*0.5, 540*0.5));*/
		//	imshow("sss",roi_img);
		return roi_img;
	}

	void MainWindow::get_add(const Mat &src)
	{
		Mat add_roi = cut_add_img(src);
		imshow("sss", add_roi);
		cvtColor(add_roi, add_roi, CV_RGB2GRAY);
		//ģ������
		blur(add_roi, add_roi, Size(2, 2));
		//imshow("defd", name_roi);

		//tesseract::TessBaseAPI tess;
		//tess.Init(NULL, "chi_sim", tesseract::OEM_DEFAULT);
		//tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
		//tess.SetImage((uchar*)add_roi.data, add_roi.cols, add_roi.rows, 1, add_roi.cols);

		///* Get the text*/
		//char* outt = "";
		//outt = tess.GetUTF8Text();

		//QString aa(outt);
		//ui->add_textEdit->setText(aa);
	}