#include "kxhex.h"
#include <qmath.h>

KxHex::~KxHex() {

}
void KxHex::drawShape(QPaintDevice* parent) {
    QPainter painter(parent);

    // 填充颜色
    QColor color = QColor(Qt::red);
    QBrush brush(color);
    painter.setBrush(brush);
    // 边框颜色
    QPen pen(Qt::black);
    painter.setPen(pen);

    // 计算六边形的六个顶点
    QVector<QPointF> hexagonPoints;
    QPointF center = m_startPoint; // 中心点
    QPointF vertex = m_endPoint;   // 已知顶点

    // 计算已知顶点与中心点之间的距离（即外接圆半径）
    double radius = sqrt(pow(vertex.x() - center.x(), 2) + pow(vertex.y() - center.y(), 2));

    // 计算已知顶点与中心点之间的角度
    double angleToFirstVertex = atan2(vertex.y() - center.y(), vertex.x() - center.x());

    for (int i = 0; i < 6; ++i) {
        // 每个角相隔60度
        double angleDeg = angleToFirstVertex + M_PI * 2.0 / 6 * i;
        QPointF point(
            center.x() + radius * cos(angleDeg),
            center.y() + radius * sin(angleDeg)
        );
        hexagonPoints << point;
    }

    // 使用QPolygonF和painter.drawPolygon来绘制六边形
    QPolygonF hexagon(hexagonPoints);
    painter.drawPolygon(hexagon);
}