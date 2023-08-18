#include "cam.h"
#include <QtWidgets/QApplication>


Ui::camClass ui;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cam w;
    w.show();
    return a.exec();
}
