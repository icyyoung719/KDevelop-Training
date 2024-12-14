#include "kxrect.h"

KxRect::~KxRect()
{
}

void KxRect::drawShape(QPaintDevice* parent)
{
	QPainter painter(parent);

	// Ìî³äÑÕÉ«
	QColor color = QColor(Qt::lightGray);
	QBrush brush(color);
    painter.setBrush(brush);
	// ±ß¿òÑÕÉ«
	QPen pen(Qt::black);
	painter.setPen(pen);

	painter.drawRect(QRect(m_startPoint,m_endPoint));
}
