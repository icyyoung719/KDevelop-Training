#ifndef __KX_SHAPE_H__
#define __KX_SHAPE_H__
#include <QPoint>
#include <QPainter>
#include <QPaintDevice>


class KxShape
{

public:
	enum class KxShapeType
	{
		None = 0,
		PenShapeType,
		LineShapeType,
		CircleShapeType,
		RectShapeType,
		PentShapeType,
		HexaShapeType,
		TextShapeType,
	};

	KxShape() =default;
	virtual ~KxShape();

	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) = 0;
	//virtual bool containsPoint(const QPoint& point) const = 0;

	QPoint getStartPoint() const;
	QPoint getEndPoint() const;

	void setStartPoint(const QPoint& point);
	void setEndPoint(const QPoint& point);

	bool isShapeVaild() const;

protected:
	QPoint m_startPoint;
	QPoint m_endPoint;
};

#endif