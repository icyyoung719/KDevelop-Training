#include "kmainwindow.h"
#include <QtWidgets/QApplication>
#include <winrt/base.h>

int main(int argc, char* argv[])
{
    // ���� COM ģʽΪ STA
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    QApplication a(argc, argv);
    KMainWindow w;
    w.show();
    return a.exec();
}
