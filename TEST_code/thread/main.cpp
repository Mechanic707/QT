#include "thread_test.h"
#include <QtWidgets/QApplication>

QTextBrowser* output;
QByteArray buffer;
ZMC_HANDLE g_handle;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    thread_test w;
    w.show();
    return a.exec();
}
