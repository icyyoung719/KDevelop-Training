#include "kmainwindow.h"
#include <QtWidgets/QApplication>
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Input.h>
#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    // Initialize Windows Runtime
    winrt::init_apartment(winrt::apartment_type::single_threaded);

    KMainWindow w;
    w.show();
    return a.exec();
}
