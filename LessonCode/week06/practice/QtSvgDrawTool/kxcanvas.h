#ifndef __KX_CANVAS_H__
#define __KX_CANVAS_H__

#include "kxshape.h"

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>

class KxCanvas : public QWidget
{
	Q_OBJECT
public:
	enum class KDrawFlag
	{
		NoneDrawFlag = 0,
		MouseDrawFlag,
		PenDrawFlag,
		LineDrawFlag,
		RectDrawFlag,
		CircleDrawFlag,
		TextDrawFlag,
		HexDrawFlag
	};
	
	explicit KxCanvas(QWidget* parent = nullptr);
	~KxCanvas(); 

	void updateDrawingFlag(KDrawFlag drawFlag);

protected:
	virtual void paintEvent(QPaintEvent* event)	override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	

private:
	std::vector<std::unique_ptr<KxShape>> m_shapes;//ͼ������
	std::unique_ptr<KxShape> m_drawingShapePtr;//ͼ�ζ��������ָ��
	QPoint m_lastPos;//��¼����λ��
	bool m_isDrawing = false;//��¼�Ƿ����ڻ�ͼ
	bool m_isDragging = false;//��¼�Ƿ������϶�
	KDrawFlag m_drawingFlag = KDrawFlag::NoneDrawFlag;
	bool m_isLeftButtonPressed = false;//��¼�Ƿ���������
};

#endif