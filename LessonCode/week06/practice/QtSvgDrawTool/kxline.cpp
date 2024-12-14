#include "kxline.h"


KxLine::~KxLine() {

}
void KxLine::drawShape(QPaintDevice* parent) {
    QPainter painter(parent);

    // Ìî³äÑÕÉ«
    QColor color = QColor(Qt::blue);
    QBrush brush(color);
    painter.setBrush(brush);
    // ±ß¿òÑÕÉ«
    QPen pen(Qt::black);
    painter.setPen(pen);

    painter.drawLine(m_startPoint,m_endPoint);
}