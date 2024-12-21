#include "kmainwindow.h"
#include <QtWidgets/QApplication>
#include <winrt/base.h>

int main(int argc, char* argv[])
{
    // 设置 COM 模式为 STA
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    QApplication a(argc, argv);
    KMainWindow w;
    w.show();
    return a.exec();
}
