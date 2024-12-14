#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QtWidgets/QMainWindow>

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QColor>

class ScribbleArea : public QWidget {
    Q_OBJECT

public:
    explicit ScribbleArea(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    struct LineData {
        QColor color;
        int width;
        QLine line;
    };

    QVector<LineData> myLines; // 保存所有的线段数据
    QPoint lastPoint;          // 上一次鼠标位置
    bool scribbling;           // 是否正在绘制
    int myPenWidth;            // 笔触宽度

    void drawLineTo(const QPoint& endPoint);
};

#endif // SCRIBBLEAREA_H