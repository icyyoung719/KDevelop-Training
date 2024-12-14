#include "kxcanvas.h"
#include "kxshapefactory.h"

KxCanvas::KxCanvas(QWidget* parent /* = nullptr */)
	:QWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	setMouseTracking(true);
	setFocusPolicy(Qt::ClickFocus);
}

KxCanvas::~KxCanvas()
{
}

void KxCanvas::paintEvent(QPaintEvent* event)
{
	//将前面保存的图形重新绘制
	for (const auto& shapePtr : m_shapes)
	{
		if (shapePtr)
			shapePtr->drawShape(this);
	}
	//绘制当前的图形
	if (m_drawingShapePtr)
		if (m_isDrawing)
			m_drawingShapePtr->drawShape(this);
}

void KxCanvas::mousePressEvent(QMouseEvent* event)
{
	if (m_drawingFlag == KDrawFlag::NoneDrawFlag)
		return;
	
	if (event->buttons() == Qt::LeftButton)
	{
		m_isLeftButtonPressed = true;

		// 判断当前是否在点击拖动图形
		

		m_drawingShapePtr = KxShapeFactory::createShape(m_drawingFlag);//创建需要绘制的图形对象
		if (m_drawingShapePtr.get() != nullptr)
			m_drawingShapePtr->setStartPoint(event->pos());//记录图形矩形框开始的位置
	}
	update();//触发绘图事件进行重新绘制
}

void KxCanvas::mouseMoveEvent(QMouseEvent* event)
{
	if (m_drawingFlag == KDrawFlag::NoneDrawFlag)
		return;
	if (m_isLeftButtonPressed && !m_isDrawing)
		m_isDrawing = true;

	if (m_drawingShapePtr.get() != nullptr)
		m_drawingShapePtr->setEndPoint(event->pos());// 记录图形矩形框的结束位置

	update();//触发绘图事件，重新绘制
}

void KxCanvas::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_isLeftButtonPressed)
	{
		if (m_drawingFlag != KDrawFlag::MouseDrawFlag)//鼠标移动标志没有被设置
		{
			if (m_drawingShapePtr.get() != nullptr)
			{
				if (m_drawingShapePtr->isShapeVaild())//判断图形是否有效
					m_drawingShapePtr->setEndPoint(event->pos());//设置最终的图形矩形框结束位置
				m_shapes.push_back(std::move(m_drawingShapePtr));//将新的绘制的图形保存到容器内
				m_drawingShapePtr.reset();
			}
		}
		m_isLeftButtonPressed = false;
		m_isDrawing = false;
	}
	update();// 触发绘图事件，进行重新绘制
}

void KxCanvas::updateDrawingFlag(KDrawFlag drawFlag)
{
	m_drawingFlag = drawFlag;
}
