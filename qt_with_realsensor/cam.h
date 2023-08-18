#pragma once

#include <QtWidgets/QWidget>
#include "ui_cam.h"
#include <QMouseEvent>
#include <qDebug>
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include <opencv2/imgcodecs.hpp>
#include<QtGui>

//#include <opencv2/opencv.hpp>
#include <qthread.h>
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

using namespace cv;

extern Ui::camClass ui;
class WorkThread :public QThread
{
    Q_OBJECT
public:

    WorkThread();

    
protected:
    void run();
    //void stop();
};




class cam : public QWidget
{
    Q_OBJECT

public:
    cam(QWidget *parent = nullptr);
    ~cam();

    WorkThread* workThread;

    void mousePressEvent(QMouseEvent* event);

private:
    

public slots:
    void start_slot();
    //void on_pushButton_qt_mouse_clicked();
};
