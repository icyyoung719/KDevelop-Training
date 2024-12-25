#include "scribblearea.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QFuture>
#include <QtConcurrent>
#include <winrt/Windows.Foundation.Collections.h>
#include <QFutureWatcher>

ScribbleArea::ScribbleArea(QWidget* parent)
    : QWidget(parent), scribbling(false), myPenWidth(3) {
    // 初始化 InkManager
    inkManager = winrt::Windows::UI::Input::Inking::InkManager();
}

void ScribbleArea::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent* event) {
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;

        // 将当前绘制的线条转换为 InkStroke
        auto inkPoints = winrt::single_threaded_vector<winrt::Windows::UI::Input::Inking::InkPoint>();

        for (const auto& line : myLines) {
            inkPoints.Append(winrt::Windows::UI::Input::Inking::InkPoint(
                winrt::Windows::Foundation::Point(line.line.x1(), line.line.y1()), 1.0f));
            inkPoints.Append(winrt::Windows::UI::Input::Inking::InkPoint(
                winrt::Windows::Foundation::Point(line.line.x2(), line.line.y2()), 1.0f));
        }

        auto strokeBuilder = winrt::Windows::UI::Input::Inking::InkStrokeBuilder();
        auto stroke = strokeBuilder.CreateStrokeFromInkPoints(inkPoints, winrt::Windows::Foundation::Numerics::float3x2::identity());
        inkManager.AddStroke(stroke);

        // 创建 QFutureWatcher 来监控异步任务
        QFutureWatcher<QString>* watcher = new QFutureWatcher<QString>(this);
        connect(watcher, &QFutureWatcher<QString>::finished, this, [this, watcher]() {
            QString result = watcher->result();
            emit recognitionResult(result); // 发射识别结果信号
            watcher->deleteLater(); // 清理 QFutureWatcher 对象
            });

        // 异步执行识别操作
        QFuture<QString> future = recognizeInkAsync(); // 获取 QFuture<QString> 返回值

        watcher->setFuture(future);
    }
}

void ScribbleArea::paintEvent(QPaintEvent* /* event */) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), Qt::white); // 清空背景

    for (const auto& line : myLines) {
        painter.setPen(QPen(line.color, line.width));
        painter.drawLine(line.line);
    }
}

void ScribbleArea::drawLineTo(const QPoint& endPoint) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    // 更新线条数据
    myLines.append({ Qt::black, myPenWidth, QLine(lastPoint, endPoint) });

    lastPoint = endPoint;
    update(); // 触发重绘
}

QFuture<QString> ScribbleArea::recognizeInkAsync() {
    return QtConcurrent::run([this]() {  // 异步执行的代码块，lambda 表达式
        if (!inkManager) {
            return QString("InkManager not initialized");
        }

        // 检查是否有可用的识别器
        auto recognizerContainer = inkManager.GetRecognizers();
        if (recognizerContainer.Size() == 0) {
            return QString("No recognizers found.");
        }

        // 使用第一个识别器
        auto recognizer = recognizerContainer.GetAt(0);
        inkManager.SetDefaultRecognizer(recognizer);

        // 检查 InkManager 是否包含有效数据
        auto strokes = inkManager.GetStrokes();
        if (strokes.Size() == 0) {
            return QString("No strokes available for recognition.");
        }

        // 调用识别方法
        auto recognitionResults = inkManager.RecognizeAsync(winrt::Windows::UI::Input::Inking::InkRecognitionTarget::All).get();

        QString recognizedText;
        for (const auto& recognitionResult : recognitionResults) {
            auto textCandidates = recognitionResult.GetTextCandidates();
            if (textCandidates.Size() > 0) {
                recognizedText += QString::fromWCharArray(textCandidates.GetAt(0).c_str()) + " ";
            }
        }

        return recognizedText.trimmed(); // 返回识别的文本
        });
}

