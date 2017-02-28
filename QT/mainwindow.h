#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cv.h>  
#include <highgui.h> 
#include<Qgraphicsview>

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

	QImage * qImage; // qt �е�ͼƬ����ָ��
	IplImage * image;  // opencv���ͼƬ����ָ��
	IplImage * tempImage;
};

#endif // MAINWINDOW_H
