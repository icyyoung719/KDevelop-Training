#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <QFuture>
#include <QStringList>
#include <QtConcurrent/QtConcurrent>

#include <msinkaut.h>
#include <msinkaut_i.c> // Tablet PC SDK 的 COM 接口定义

class ScribbleArea : public QWidget {
    Q_OBJECT

public:
    explicit ScribbleArea(QWidget* parent = nullptr);
    ~ScribbleArea();

signals:
    void recognitionResults(const QStringList& results); // 识别结果信号

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    IInkCollector* inkCollector; // Tablet PC SDK 的墨迹收集器

    QFuture<QStringList> recognizeInkAsync(); // 异步手写识别
    void onRecognitionFinished();            // 处理识别结果
};

#endif // SCRIBBLEAREA_H
