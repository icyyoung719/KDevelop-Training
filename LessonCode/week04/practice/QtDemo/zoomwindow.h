#ifndef ZOOMWINDOW_H
#define ZOOMWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <qevent.h>
#include <qlogging.h>

class MyWidget : public QWidget {
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent* event) override {
        std::cout << "Key pressed:" ;
    }

    void mousePressEvent(QMouseEvent* event) override {
        std::cout << "Mouse button pressed:" ;
    }
};

#endif // ZOOMWINDOW_H
