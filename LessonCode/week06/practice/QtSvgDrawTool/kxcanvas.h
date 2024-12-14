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
	std::vector<std::unique_ptr<KxShape>> m_shapes;//图形容器
	std::unique_ptr<KxShape> m_drawingShapePtr;//图形对象的智能指针
	QPoint m_lastPos;//记录最终位置
	bool m_isDrawing = false;//记录是否正在绘图
	bool m_isDragging = false;//记录是否正在拖动
	KDrawFlag m_drawingFlag = KDrawFlag::NoneDrawFlag;
	bool m_isLeftButtonPressed = false;//记录是否按下鼠标左键
};

#endif