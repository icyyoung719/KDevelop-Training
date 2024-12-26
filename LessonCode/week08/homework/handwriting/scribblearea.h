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
    static const QStringList defaultRecognitionResults; // 默认识别结果

    explicit ScribbleArea(QWidget* parent = nullptr);
    ~ScribbleArea();

signals:
    void recognitionResults(const QStringList& results); // 识别结果信号，emit以刷新页面中显示的文字
    void canUndoChanged(bool canUndo); // 是否可以撤销信号，emit以更新按钮是否可用
    void canClearChanged(bool canClear); // 是否可以清空信号，emit以更新按钮是否可用

public slots:
    void undo(); // 撤销
    void clear(); // 清空

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:

    IInkCollector* inkCollector; // Tablet PC SDK 的墨迹收集器
    QList<IInkStrokeDisp*> strokesList; // 记录已绘制的所有笔触（用于撤销）

    QFuture<QStringList> recognizeInkAsync(); // 异步手写识别
    void onRecognitionFinished();            // 处理识别结果
};

#endif // SCRIBBLEAREA_H
