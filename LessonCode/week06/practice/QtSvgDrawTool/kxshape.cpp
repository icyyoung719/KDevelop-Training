#include "kxshape.h"


KxShape::~KxShape()
{
}

QPoint KxShape::getStartPoint() const
{
	return m_startPoint;
}

QPoint KxShape::getEndPoint() const
{
	return m_endPoint;
}

void KxShape::setStartPoint(const QPoint& point)
{
	m_startPoint = point;
}

void KxShape::setEndPoint(const QPoint& point)
{
	m_endPoint = point;
}

bool KxShape::isShapeVaild() const
{
	return (m_startPoint.x() != m_endPoint.x() || m_startPoint.y() != m_endPoint.y());
}
