#include "scribblearea.h"
#include <QMouseEvent>
#include <QPainter>

ScribbleArea::ScribbleArea(QWidget* parent)
    : QWidget(parent), scribbling(false), myPenWidth(3) {
}

void ScribbleArea::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent* event) {
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
    }
}

void ScribbleArea::paintEvent(QPaintEvent* /* event */) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), Qt::white); // Clear the background with white color

    for (const auto& line : myLines) {
        painter.setPen(QPen(line.color, line.width));
        painter.drawLine(line.line);
    }
}

void ScribbleArea::drawLineTo(const QPoint& endPoint) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    // 更新myLines以持久化线条数据
    myLines.append({ Qt::black, myPenWidth, QLine(lastPoint, endPoint) });

    lastPoint = endPoint;
    update(); // 强制重绘窗口
}