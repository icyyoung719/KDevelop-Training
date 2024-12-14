#ifndef __KMAINWINDOW_H__
#define __KMAINWINDOW_H__

#include <QtWidgets/QMainWindow>
#include "ui_kmainwindow.h"
#include <QLabel>

class KMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    KMainWindow(QWidget* parent = Q_NULLPTR);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    Ui::KMainWindowClass ui;
    QTimer* m_pTimer;


    void KMainWindow::drawSecondHand(QPainter& painter);
    void KMainWindow::drawMinuteHandAndMarks(QPainter& painter);
    void KMainWindow::drawHourHandAndMarks(QPainter& painter);
};

#endif
