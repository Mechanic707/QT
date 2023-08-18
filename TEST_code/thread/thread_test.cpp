#include "thread_test.h"

thread_test::thread_test(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    setWindowTitle("多线程测试");
	setFixedSize(window_width, window_height);
	//demo按钮
	start = new QPushButton("启动", this);
	start->setStyleSheet("font-size:14px");
	start->setGeometry(150, 30, 60, 20);
	connect(start, SIGNAL(clicked()), this, SLOT(start_slot()));

	end = new QPushButton("停止", this);
	end->setStyleSheet("font-size:14px");
	end->setGeometry(250, 30, 60, 20);
	connect(end, SIGNAL(clicked()), this, SLOT(end_slot()));

	output = new QTextBrowser(this);
	output->setGeometry(650, 60, 450, 300);


	//serial = new QSerialPort;
	serial.setPortName("COM7");
	serial.setBaudRate(QSerialPort::Baud115200);
	serial.setDataBits(QSerialPort::Data8);
	serial.setParity(QSerialPort::NoParity);
	serial.setStopBits(QSerialPort::OneStop);
	serial.setFlowControl(QSerialPort::NoFlowControl);

	serial.open(QIODevice::ReadWrite);
	QObject::connect(&serial, &QSerialPort::readyRead, this, &thread_test::serial_readyRead);
	
	
}

thread_test::~thread_test()
{}


void thread_test::serial_readyRead()
{
	//output->append("111");
	buffer = serial.readAll();
	QString bufferStr = ByteArrayToHexString(buffer);
	if (buffer[0] == '0')
	{
		//output->append(buffer);
		output->append("test");
	}
	output->append(buffer);
	
}

void thread_test::start_slot()
{
	/*output->append("1");
	serial.write("777");*/
	//for (int i = 0; i < 2; i++)
	//{
	//	workThread[i] = new WorkThread();
	//}

	//for (int i = 0; i < 2; i++)
	//{
	//	workThread[i]->start();
	//}
	iresult = ZAux_OpenEth(ip, &g_handle);
	ZAux_Direct_SetDpos(g_handle, 1, 0);
	//iaxis_XYZ_abs(g_handle, 1, 5, -20);
	workThread = new WorkThread();

	workThread->start();



}


QString thread_test::ByteArrayToHexString(QByteArray data)
{
	QString ret(data.toHex().toUpper());
	int len = ret.length() / 2;
	qDebug() << len;
	for (int i = 1; i < len; i++)
	{
		//qDebug()<<i;
		ret.insert(2 * i + i - 1, " ");
	}

	return ret;
}


void thread_test::end_slot()
{
	output->append("2");
}

WorkThread::WorkThread()
{

}

void WorkThread::run()
{
	while (1)
	{
		//emit AppendText("1");

		if (buffer[0] == '0')
		{
			QMetaObject::invokeMethod(qApp, [this] {
				output->append("0000");
				
				});
			//iaxis_XYZ_abs(g_handle, 1, 5, -20);
			ZAux_Direct_Single_Cancel(g_handle, 1, 2);

		}
		if (buffer[0] == '1')
		{
			QMetaObject::invokeMethod(qApp, [this] {
				output->append("1111");

				});
			//iaxis_XYZ_abs(g_handle, 1, 5, -20);
			//ZAux_Direct_Single_Cancel(g_handle, 1, 2);
			iaxis_continuous_move(g_handle, 1, 1, 1);

		}
		if (buffer[0] == '2')
		{
			QMetaObject::invokeMethod(qApp, [this] {
				output->append("2222");

				});
			//iaxis_XYZ_abs(g_handle, 1, 5, -20);
			//ZAux_Direct_Single_Cancel(g_handle, 1, 2);
			iaxis_continuous_move(g_handle, 1, 1, -1);

		}
		
		QThread::msleep(1);
	}
}


