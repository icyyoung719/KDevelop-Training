#include "kxhex.h"
#include <qmath.h>

KxHex::~KxHex() {

}
void KxHex::drawShape(QPaintDevice* parent) {
    QPainter painter(parent);

    // �����ɫ
    QColor color = QColor(Qt::red);
    QBrush brush(color);
    painter.setBrush(brush);
    // �߿���ɫ
    QPen pen(Qt::black);
    painter.setPen(pen);

    // ���������ε���������
    QVector<QPointF> hexagonPoints;
    QPointF center = m_startPoint; // ���ĵ�
    QPointF vertex = m_endPoint;   // ��֪����

    // ������֪���������ĵ�֮��ľ��루�����Բ�뾶��
    double radius = sqrt(pow(vertex.x() - center.x(), 2) + pow(vertex.y() - center.y(), 2));

    // ������֪���������ĵ�֮��ĽǶ�
    double angleToFirstVertex = atan2(vertex.y() - center.y(), vertex.x() - center.x());

    for (int i = 0; i < 6; ++i) {
        // ÿ�������60��
        double angleDeg = angleToFirstVertex + M_PI * 2.0 / 6 * i;
        QPointF point(
            center.x() + radius * cos(angleDeg),
            center.y() + radius * sin(angleDeg)
        );
        hexagonPoints << point;
    }

    // ʹ��QPolygonF��painter.drawPolygon������������
    QPolygonF hexagon(hexagonPoints);
    painter.drawPolygon(hexagon);
}