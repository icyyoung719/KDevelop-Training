#include "kxrect.h"

KxRect::~KxRect()
{
}

void KxRect::drawShape(QPaintDevice* parent)
{
	QPainter painter(parent);

	// �����ɫ
	QColor color = QColor(Qt::lightGray);
	QBrush brush(color);
    painter.setBrush(brush);
	// �߿���ɫ
	QPen pen(Qt::black);
	painter.setPen(pen);

	painter.drawRect(QRect(m_startPoint,m_endPoint));
}
