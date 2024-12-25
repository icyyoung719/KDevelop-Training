// scribblearea.cpp
#include "scribblearea.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QFutureWatcher>

ScribbleArea::ScribbleArea(QWidget* parent)
    : QWidget(parent), inkCollector(nullptr) {
    // 初始化 COM 环境
    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr)) {
        qDebug() << "Failed to initialize COM. Error:" << hr;
        return;
    }

    // 创建 InkCollector 实例
    hr = CoCreateInstance(CLSID_InkCollector, nullptr, CLSCTX_INPROC_SERVER, IID_IInkCollector, (void**)&inkCollector);
    if (FAILED(hr)) {
        qDebug() << "Failed to create InkCollector. Error:" << hr;
        return;
    }

    // 将 InkCollector 绑定到窗口
    hr = inkCollector->put_hWnd(static_cast<LONG_PTR>(winId()));
    if (FAILED(hr)) {
        qDebug() << "Failed to attach InkCollector to window. Error:" << hr;
        return;
    }

    // 启用墨迹收集
    hr = inkCollector->put_Enabled(VARIANT_TRUE);
    if (FAILED(hr)) {
        qDebug() << "Failed to enable InkCollector. Error:" << hr;
        return;
    }
}

ScribbleArea::~ScribbleArea() {
    if (inkCollector) {
        inkCollector->put_Enabled(VARIANT_FALSE); // 禁用墨迹收集
        inkCollector->Release();
    }
    CoUninitialize();
}

void ScribbleArea::mousePressEvent(QMouseEvent* event) {
    // Tablet PC SDK 自动处理鼠标事件
    QWidget::mousePressEvent(event);
}

void ScribbleArea::mouseMoveEvent(QMouseEvent* event) {
    // Tablet PC SDK 自动处理鼠标事件
    QWidget::mouseMoveEvent(event);
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        // 开始异步识别墨迹
        QFuture<QStringList> future = recognizeInkAsync();
        QFutureWatcher<QStringList>* watcher = new QFutureWatcher<QStringList>(this);
        connect(watcher, &QFutureWatcher<QStringList>::finished, this, [this, watcher]() {
            QStringList results = watcher->result();
            emit recognitionResults(results);  // 发送识别结果信号
            for (const QString& result : results) {
                qDebug() << "Recognition Result:" << result;
            }
            watcher->deleteLater();
            });
        watcher->setFuture(future);
    }
    QWidget::mouseReleaseEvent(event);
}

void ScribbleArea::paintEvent(QPaintEvent* /* event */) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white); // 背景设置为白色
}

QFuture<QStringList> ScribbleArea::recognizeInkAsync() {
    return QtConcurrent::run([this]() {
        QStringList results;

        // 获取 Ink 数据
        IInkDisp* inkDisp = nullptr;
        HRESULT hr = inkCollector->get_Ink(&inkDisp);
        if (FAILED(hr) || inkDisp == nullptr) {
            results << "Failed to retrieve ink data.";
            return results;
        }

        // 获取笔触集合
        IInkStrokes* strokes = nullptr;
        hr = inkDisp->get_Strokes(&strokes);
        if (FAILED(hr) || strokes == nullptr) {
            results << "No strokes available for recognition.";
            inkDisp->Release();
            return results;
        }

        // 创建识别器上下文
        IInkRecognizerContext* recognizerContext = nullptr;
        hr = CoCreateInstance(CLSID_InkRecognizerContext, nullptr, CLSCTX_INPROC_SERVER, IID_IInkRecognizerContext, (void**)&recognizerContext);
        if (FAILED(hr)) {
            results << "Failed to create InkRecognizerContext.";
            strokes->Release();
            inkDisp->Release();
            return results;
        }

        recognizerContext->putref_Strokes(strokes);

        // 调用 Recognize 方法
        IInkRecognitionResult* recognitionResult = nullptr;
        InkRecognitionStatus status;
        hr = recognizerContext->Recognize(&status, &recognitionResult);
        if (SUCCEEDED(hr) && recognitionResult != nullptr) {
            // 获取识别候选项
            IInkRecognitionAlternates* alternates = nullptr;
            hr = recognitionResult->AlternatesFromSelection(0, -1, 10, &alternates);
            if (SUCCEEDED(hr) && alternates != nullptr) {
                long count = 0;
                alternates->get_Count(&count);

                for (long i = 0; i < count; ++i) {
                    IInkRecognitionAlternate* alternate = nullptr;
                    alternates->Item(i, &alternate);
                    if (alternate) {
                        BSTR text;
                        alternate->get_String(&text);
                        results << QString::fromWCharArray(text);
                        SysFreeString(text);
                        alternate->Release();
                    }
                }

                alternates->Release();
            }
            recognitionResult->Release();
        }

        // 在资源释放前处理结果
        recognizerContext->Release();
        strokes->Release();
        inkDisp->Release();

        return results;
        });
}
