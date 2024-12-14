#include "kmainwindow.h"
#include <QPixmap>
#include <QPainter>
#include <QTime>
#include <QTimer>

KMainWindow::KMainWindow(QWidget* parent)
    : QMainWindow(parent),
    m_pTimer(new QTimer(this))
{
    ui.setupUi(this);

    (void)connect(m_pTimer, &QTimer::timeout, this, QOverload<>::of(&KMainWindow::update));
    m_pTimer->start(1000);
}

void KMainWindow::paintEvent(QPaintEvent* event) {
    int side = qMin(width(), height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 反锯齿 
    painter.translate(width() / 2, height() / 2); // 将坐标系原点移动到窗口中心
    painter.scale(side / 200.0, side / 200.0); // 缩放以适应窗口大小

    drawHourHandAndMarks(painter);
    drawMinuteHandAndMarks(painter);
    drawSecondHand(painter);
}

void KMainWindow::drawHourHandAndMarks(QPainter& painter) {
    QTime time = QTime::currentTime();
    QColor hourColor(127, 0, 127);
    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);

    static const QPoint hourHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -40)
    };

    painter.save();
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();

    painter.setPen(hourColor);
    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }
}

void KMainWindow::drawMinuteHandAndMarks(QPainter& painter) {
    QTime time = QTime::currentTime();
    QColor minuteColor(0, 0, 255);
    painter.setPen(minuteColor);
    painter.setBrush(minuteColor);

    static const QPoint minuteHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };

    painter.save();
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();

    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }
}

void KMainWindow::drawSecondHand(QPainter& painter) {
    QTime time = QTime::currentTime();
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);

    static const QPoint secondHand[3] = {
        QPoint(2, 8),
        QPoint(-2, 8),
        QPoint(0, -90)
    };

    painter.save();
    painter.rotate(6.0 * time.second());
    painter.drawConvexPolygon(secondHand, 3);
    painter.restore();
}