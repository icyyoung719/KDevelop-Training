#include "scribblearea.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QColor>
#include <QFutureWatcher>
#include <windows.h> // For GetDC and ReleaseDC

const QStringList ScribbleArea::defaultRecognitionResults = {
    QString::fromWCharArray(L"你"),
    QString::fromWCharArray(L"我"),
    QString::fromWCharArray(L"是"),
    QString::fromWCharArray(L"了"),
    QString::fromWCharArray(L"就"),
    QString::fromWCharArray(L"在"),
    QString::fromWCharArray(L"的"),
    QString::fromWCharArray(L"都"),
    QString::fromWCharArray(L"有"),
	QString::fromWCharArray(L"不")
};


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

    // 获取默认的绘图属性
    IInkDrawingAttributes* defaultDrawingAttributes = nullptr;
    hr = inkCollector->get_DefaultDrawingAttributes(&defaultDrawingAttributes);
    if (FAILED(hr)) {
        qDebug() << "Failed to get DefaultDrawingAttributes. Error:" << hr;
    }
    else {
        // 直接使用默认的绘图属性
        penAttributes = PenAttributes(defaultDrawingAttributes);
    }

    // 
}

ScribbleArea::~ScribbleArea() {
    if (inkCollector) {
        inkCollector->put_Enabled(VARIANT_FALSE); // 禁用墨迹收集
        inkCollector->Release();
    }
	// penAttributes 的析构函数会自动释放 IInkDrawingAttributes 对象,不需要手动释放
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
            watcher->deleteLater();
            });
        watcher->setFuture(future);

        // 将当前笔触添加到撤销列表
        IInkDisp* inkDisp = nullptr;
        inkCollector->get_Ink(&inkDisp);
        if (inkDisp) {
            IInkStrokes* strokes = nullptr;
            inkDisp->get_Strokes(&strokes);
            if (strokes) {
                long count = 0;
                strokes->get_Count(&count);
                if (count > 0) {
                    IInkStrokeDisp* stroke = nullptr;
                    strokes->Item(count - 1, &stroke);
                    if (stroke) {
						strokesList.push_back(stroke); // 添加当前笔触到撤销列表
                        emit canUndoChanged(true);  // 更新撤销按钮状态
                        emit canClearChanged(true);  // 更新撤销按钮状态
                    }
                }
                strokes->Release();
            }
            inkDisp->Release();
        }
    }
    QWidget::mouseReleaseEvent(event);
}

void ScribbleArea::paintEvent(QPaintEvent* /* event */) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white); // 背景设置为白色

    IInkDisp* inkDisp = nullptr;
    HRESULT hr = inkCollector->get_Ink(&inkDisp);
    if (FAILED(hr) || inkDisp == nullptr) {
        qDebug() << "Failed to retrieve InkDisp.";
        return;
    }

    IInkRenderer* renderer = nullptr;
    hr = CoCreateInstance(CLSID_InkRenderer, nullptr, CLSCTX_INPROC_SERVER, IID_IInkRenderer, (void**)&renderer);
    if (FAILED(hr) || renderer == nullptr) {
        qDebug() << "Failed to create InkRenderer.";
        inkDisp->Release();
        return;
    }

    // 获取窗口句柄
    HWND hwnd = (HWND)winId();
    HDC hdc = GetDC(hwnd); // 获取设备上下文
    if (hdc == nullptr) {
        qDebug() << "Failed to get HDC from HWND.";
        renderer->Release();
        inkDisp->Release();
        return;
    }

    IInkStrokes* strokes = nullptr;
    hr = inkDisp->get_Strokes(&strokes);
    if (FAILED(hr) || strokes == nullptr) {
        qDebug() << "Failed to get strokes collection.";
        renderer->Release();
        inkDisp->Release();
        ReleaseDC(hwnd, hdc); // 释放设备上下文
        return;
    }

    hr = renderer->Draw((LONG_PTR)hdc, strokes);
    if (FAILED(hr)) {
        qDebug() << "Failed to draw strokes. Error:" << hr;
    }

    // 释放资源
    strokes->Release();
    renderer->Release();
    inkDisp->Release();
    ReleaseDC(hwnd, hdc); // 释放设备上下文
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

        // 设置识别模式
        if (recognitionMode == RecognitionMode::Word) {
            recognizerContext->put_Factoid(SysAllocString(L"FACTOID_ONECHAR"));// 设置识别模式为单词
        }
        else if (recognitionMode == RecognitionMode::Sentence) {
            recognizerContext->put_Factoid(SysAllocString(L"FACTOID_DEFAULT"));// 设置识别模式为句子
        }
        else {
            recognizerContext->put_Factoid(SysAllocString(L"FACTOID_DEFAULT"));// 设置识别模式为默认
        }
        BSTR factoid;
        recognizerContext->get_Factoid(&factoid);
        // 将 BSTR 转换为 QString
        QString factoidString = QString::fromWCharArray(factoid);

        // 使用 qDebug 输出
        qDebug() << "Recognition Mode:" << factoidString;

        // 释放 BSTR
        SysFreeString(factoid);
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

void ScribbleArea::undo() {
    if (strokesList.empty()) {
        emit canUndoChanged(false); // 如果没有可以撤销的笔触，则禁用撤销按钮
        return;
    }

    IInkDisp* inkDisp = nullptr;
    HRESULT hr = inkCollector->get_Ink(&inkDisp);
    if (FAILED(hr) || !inkDisp) {
        qDebug() << "Failed to get InkDisp for undo.";
        return;
    }

    // 获取 InkStrokes 集合
    IInkStrokes* strokes = nullptr;
    hr = inkDisp->get_Strokes(&strokes);
    if (FAILED(hr) || !strokes) {
        qDebug() << "Failed to get strokes collection.";
        inkDisp->Release();
        return;
    }

    // 获取当前笔触数量
    long count = 0;
    strokes->get_Count(&count);
    qDebug() << "Number of strokes in InkDisp before undo:" << count << '\n';

    // 获取最后一个笔触并从 InkDisp 中删除它
    IInkStrokeDisp* lastStroke = strokesList.back();
    if (lastStroke) {
        hr = strokes->Remove(lastStroke);
        hr = inkDisp->DeleteStroke(lastStroke);
        if (SUCCEEDED(hr)) {
            strokesList.pop_back(); // 从本地列表中移除该笔触
            emit canUndoChanged(!strokesList.empty()); // 更新撤销按钮状态
			emit canClearChanged(!strokesList.empty()); // 更新清空按钮状态
        }
        else {
            qDebug() << "Failed to delete stroke from InkDisp.";
        }
        lastStroke->Release();
    }

    // 获取更新后的笔触数量
    strokes->get_Count(&count);
	if (count == 0) {
        emit recognitionResults(ScribbleArea::defaultRecognitionResults); // 更新显示的默认文字
	}
    qDebug() << "Number of strokes in InkDisp after undo:" << count << '\n';

    strokes->Release();
    inkDisp->Release();

    // 将剩余的笔触重新识别
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

    // 强制刷新界面以显示最新的墨迹状态
    repaint(); 
}

void ScribbleArea::clear() {
    // 禁用 InkCollector，清除所有墨迹数据
    if (inkCollector) {
        inkCollector->put_Enabled(VARIANT_FALSE); // 禁用 InkCollector
    }

    // 获取 InkDisp 对象并清除其中的所有笔触
    IInkDisp* inkDisp = nullptr;
    inkCollector->get_Ink(&inkDisp);  // 获取当前 InkDisp 对象
    if (inkDisp) {
        inkDisp->DeleteStrokes();
		inkDisp->Release();
    }

    // 清空本地笔触列表
    strokesList.clear();

    // 重新启用 InkCollector 并准备收集新的墨迹
    HRESULT hr = inkCollector->put_Enabled(VARIANT_TRUE); // 启用 InkCollector
    if (FAILED(hr)) {
        qDebug() << "Failed to re-enable InkCollector after clearing.";
    }

    // 发射信号更新界面状态
    emit recognitionResults(ScribbleArea::defaultRecognitionResults); // 更新显示的默认文字
    emit canUndoChanged(false); // 更新撤销按钮状态
    emit canClearChanged(false); // 更新清空按钮状态

    // 强制刷新界面以显示最新的墨迹状态
    repaint();
}

void ScribbleArea::setPenColor(const QColor& color) {
    penAttributes.setColor(color);
}

void ScribbleArea::setPenWidth(int width) {
    penAttributes.setWidth(width);
}

void ScribbleArea::setRecognitionMode(RecognitionMode mode) {
    recognitionMode = mode;
}