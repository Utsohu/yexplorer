#include "ywindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    YWindow w;
    w.showMaximized();

    return a.exec();
}
