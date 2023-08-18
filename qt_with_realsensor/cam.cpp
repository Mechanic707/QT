#include "cam.h"


//参考来源：https://www.cnblogs.com/hyb965149985/p/10123453.html
QImage cvMat2QImage(const Mat& mat)    // Mat 改成 QImage
{
	if (mat.type() == CV_8UC1)					// 单通道
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		image.setColorCount(256);				// 灰度级数256
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		uchar* pSrc = mat.data;					// 复制mat数据
		for (int row = 0; row < mat.rows; row++)
		{
			uchar* pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}

	else if (mat.type() == CV_8UC3)				// 3通道
	{
		const uchar* pSrc = (const uchar*)mat.data;			// 复制像素
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);	// R, G, B 对应 0,1,2
		return image.rgbSwapped();				// rgbSwapped是为了显示效果色彩好一些。
	}
	else if (mat.type() == CV_8UC4)
	{
		const uchar* pSrc = (const uchar*)mat.data;			// 复制像素
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);		// B,G,R,A 对应 0,1,2,3
		return image.copy();
	}
	else
	{
		return QImage();
	}
}

//Mat QImage2cvMat(QImage image)			// QImage改成Mat
//{
//	Mat mat;
//	switch (image.format())
//	{
//	case QImage::Format_ARGB32:
//	case QImage::Format_RGB32:
//	case QImage::Format_ARGB32_Premultiplied:
//		mat = Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
//		break;
//	case QImage::Format_RGB888:
//		mat = Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
//		cv::cvtColor(mat, mat, COLOR_BGR2RGB);
//		break;
//	case QImage::Format_Indexed8:
//		mat = Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
//		break;
//	}
//	return mat;
//}



cam::cam(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	connect(ui.start, SIGNAL(clicked()), this, SLOT(start_slot()));
	workThread = new WorkThread();
}

cam::~cam()
{}

void cam::start_slot()
{
	workThread->start();
}

//void cam::on_pushButton_qt_mouse_clicked()
//{
//
//}

void cam::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		//qDebug() << "1";
		/*qDebug() << "x:" << ui.image->width();
		qDebug() << "y:" << ui.image->height();*/

		qDebug() << "x:" << (event->x() - ui.image->x());
		qDebug() << "y:" << (event->y() - ui.image->y());

		/*qDebug() << "x:" << (event->x());
		qDebug() << "y:" << (event->y());*/
	}
}







WorkThread::WorkThread()
{

}

void WorkThread::run()
{
	rs2::colorizer color_map;
	rs2::pipeline pipe;

	pipe.start();
	while (1)
	{
		rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
		rs2::frame depth = data.get_depth_frame().apply_filter(color_map);

		// Query frame size (width and height)
		const int w = depth.as<rs2::video_frame>().get_width();
		const int h = depth.as<rs2::video_frame>().get_height();

		// Create OpenCV matrix of size (w,h) from the colorized depth data
		Mat image1(Size(w, h), CV_8UC3, (void*)depth.get_data(), Mat::AUTO_STEP);
		// Update the window with new data

		//cv::Mat matFrame = cv::imread("12.png", 1);
		cv::resize(image1, image1, Size(640, 480), 0, 0);
		QImage imgg_1 = cvMat2QImage(image1);
		QPixmap qpixmap_1 = QPixmap::fromImage(imgg_1);
		ui.image->setPixmap(qpixmap_1);
	}
}