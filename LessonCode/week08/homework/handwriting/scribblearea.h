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

    QVector<LineData> myLines; // �������е��߶�����
    QPoint lastPoint;          // ��һ�����λ��
    bool scribbling;           // �Ƿ����ڻ���
    int myPenWidth;            // �ʴ����

    void drawLineTo(const QPoint& endPoint);
};

#endif // SCRIBBLEAREA_H