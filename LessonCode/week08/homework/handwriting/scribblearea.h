#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <QFuture>
#include <QStringList>
#include <QtConcurrent/QtConcurrent>

#include <msinkaut.h>
#include <msinkaut_i.c> // Tablet PC SDK 的 COM 接口定义

#include "penattributes.h" // 墨迹绘图属性

class ScribbleArea : public QWidget {
    Q_OBJECT

public:
    static const QStringList defaultRecognitionResults; // 默认识别结果

    explicit ScribbleArea(QWidget* parent = nullptr);
    ~ScribbleArea();

    enum class RecognitionMode { // 识别模式
        Auto,
        Word,
        Sentence
    };

    void setPenColor(const QColor& color); // 设置笔的颜色
    void setPenWidth(int width);           // 设置笔的粗细
    void setRecognitionMode(RecognitionMode mode); // 设置识别模式

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

    PenAttributes penAttributes; // 笔的属性
    RecognitionMode recognitionMode; // 当前识别模式

    QFuture<QStringList> recognizeInkAsync(); // 异步手写识别
};

#endif // SCRIBBLEAREA_H
