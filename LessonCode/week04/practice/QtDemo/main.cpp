#include "mainwindow.h"
#include "zoomwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MyWidget widget;
    widget.show();

    return a.exec();
}