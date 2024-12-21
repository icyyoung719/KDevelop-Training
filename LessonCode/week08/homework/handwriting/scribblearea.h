#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QtWidgets/QWidget>
#include <QVector>
#include <QPoint>
#include <QColor>
#include <QString>

// 引入 WinRT 和 Windows Ink 相关的头文件
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Input.Inking.h>
#include <winrt/Windows.Data.Xml.Dom.h>

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

    QVector<LineData> myLines; // 保存绘制的线条数据
    QPoint lastPoint;          // 上一次鼠标位置
    bool scribbling;           // 是否正在绘制
    int myPenWidth;            // 笔触宽度

    winrt::Windows::UI::Input::Inking::InkManager inkManager; // InkManager 实例

    void drawLineTo(const QPoint& endPoint);
    QString recognizeInk(); // 调用笔迹识别
};

#endif // SCRIBBLEAREA_H
