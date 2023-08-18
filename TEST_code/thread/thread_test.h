#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QWidget>
#include "ui_thread_test.h"
#include <qthread.h>
#include <QTextBrowser>
#include <QPushButton>
#include <qdebug.h>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "motor.h"



#define window_width 1121
#define window_height 800

extern QTextBrowser* output;
extern QByteArray buffer;
extern ZMC_HANDLE g_handle;

class WorkThread :public QThread
{
    Q_OBJECT
public:

    WorkThread();
    bool test = 0;

protected:
    void run();
};

class thread_test : public QWidget
{
    Q_OBJECT

public:
    thread_test(QWidget *parent = nullptr);
    ~thread_test();
    QSerialPort serial;

    QPushButton* start;
    QPushButton* end;
    
    QString ByteArrayToHexString(QByteArray data);
    
    int32 iresult;
    char ip[16] = "192.168.0.11";
    

private:
    Ui::thread_testClass ui;
    WorkThread* workThread;

public slots:
    void start_slot();
    void end_slot();

    void serial_readyRead();


};
