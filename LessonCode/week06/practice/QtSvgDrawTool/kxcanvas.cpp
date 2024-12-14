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
	//��ǰ�汣���ͼ�����»���
	for (const auto& shapePtr : m_shapes)
	{
		if (shapePtr)
			shapePtr->drawShape(this);
	}
	//���Ƶ�ǰ��ͼ��
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

		// �жϵ�ǰ�Ƿ��ڵ���϶�ͼ��
		

		m_drawingShapePtr = KxShapeFactory::createShape(m_drawingFlag);//������Ҫ���Ƶ�ͼ�ζ���
		if (m_drawingShapePtr.get() != nullptr)
			m_drawingShapePtr->setStartPoint(event->pos());//��¼ͼ�ξ��ο�ʼ��λ��
	}
	update();//������ͼ�¼��������»���
}

void KxCanvas::mouseMoveEvent(QMouseEvent* event)
{
	if (m_drawingFlag == KDrawFlag::NoneDrawFlag)
		return;
	if (m_isLeftButtonPressed && !m_isDrawing)
		m_isDrawing = true;

	if (m_drawingShapePtr.get() != nullptr)
		m_drawingShapePtr->setEndPoint(event->pos());// ��¼ͼ�ξ��ο�Ľ���λ��

	update();//������ͼ�¼������»���
}

void KxCanvas::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_isLeftButtonPressed)
	{
		if (m_drawingFlag != KDrawFlag::MouseDrawFlag)//����ƶ���־û�б�����
		{
			if (m_drawingShapePtr.get() != nullptr)
			{
				if (m_drawingShapePtr->isShapeVaild())//�ж�ͼ���Ƿ���Ч
					m_drawingShapePtr->setEndPoint(event->pos());//�������յ�ͼ�ξ��ο����λ��
				m_shapes.push_back(std::move(m_drawingShapePtr));//���µĻ��Ƶ�ͼ�α��浽������
				m_drawingShapePtr.reset();
			}
		}
		m_isLeftButtonPressed = false;
		m_isDrawing = false;
	}
	update();// ������ͼ�¼����������»���
}

void KxCanvas::updateDrawingFlag(KDrawFlag drawFlag)
{
	m_drawingFlag = drawFlag;
}
