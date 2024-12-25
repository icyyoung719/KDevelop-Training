// scribblearea.cpp
#include "scribblearea.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QFutureWatcher>

ScribbleArea::ScribbleArea(QWidget* parent)
    : QWidget(parent), inkCollector(nullptr) {
    // ��ʼ�� COM ����
    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr)) {
        qDebug() << "Failed to initialize COM. Error:" << hr;
        return;
    }

    // ���� InkCollector ʵ��
    hr = CoCreateInstance(CLSID_InkCollector, nullptr, CLSCTX_INPROC_SERVER, IID_IInkCollector, (void**)&inkCollector);
    if (FAILED(hr)) {
        qDebug() << "Failed to create InkCollector. Error:" << hr;
        return;
    }

    // �� InkCollector �󶨵�����
    hr = inkCollector->put_hWnd(static_cast<LONG_PTR>(winId()));
    if (FAILED(hr)) {
        qDebug() << "Failed to attach InkCollector to window. Error:" << hr;
        return;
    }

    // ����ī���ռ�
    hr = inkCollector->put_Enabled(VARIANT_TRUE);
    if (FAILED(hr)) {
        qDebug() << "Failed to enable InkCollector. Error:" << hr;
        return;
    }
}

ScribbleArea::~ScribbleArea() {
    if (inkCollector) {
        inkCollector->put_Enabled(VARIANT_FALSE); // ����ī���ռ�
        inkCollector->Release();
    }
    CoUninitialize();
}

void ScribbleArea::mousePressEvent(QMouseEvent* event) {
    // Tablet PC SDK �Զ���������¼�
    QWidget::mousePressEvent(event);
}

void ScribbleArea::mouseMoveEvent(QMouseEvent* event) {
    // Tablet PC SDK �Զ���������¼�
    QWidget::mouseMoveEvent(event);
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        // ��ʼ�첽ʶ��ī��
        QFuture<QStringList> future = recognizeInkAsync();
        QFutureWatcher<QStringList>* watcher = new QFutureWatcher<QStringList>(this);
        connect(watcher, &QFutureWatcher<QStringList>::finished, this, [this, watcher]() {
            QStringList results = watcher->result();
            emit recognitionResults(results);  // ����ʶ�����ź�
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
    painter.fillRect(rect(), Qt::white); // ��������Ϊ��ɫ
}

QFuture<QStringList> ScribbleArea::recognizeInkAsync() {
    return QtConcurrent::run([this]() {
        QStringList results;

        // ��ȡ Ink ����
        IInkDisp* inkDisp = nullptr;
        HRESULT hr = inkCollector->get_Ink(&inkDisp);
        if (FAILED(hr) || inkDisp == nullptr) {
            results << "Failed to retrieve ink data.";
            return results;
        }

        // ��ȡ�ʴ�����
        IInkStrokes* strokes = nullptr;
        hr = inkDisp->get_Strokes(&strokes);
        if (FAILED(hr) || strokes == nullptr) {
            results << "No strokes available for recognition.";
            inkDisp->Release();
            return results;
        }

        // ����ʶ����������
        IInkRecognizerContext* recognizerContext = nullptr;
        hr = CoCreateInstance(CLSID_InkRecognizerContext, nullptr, CLSCTX_INPROC_SERVER, IID_IInkRecognizerContext, (void**)&recognizerContext);
        if (FAILED(hr)) {
            results << "Failed to create InkRecognizerContext.";
            strokes->Release();
            inkDisp->Release();
            return results;
        }

        recognizerContext->putref_Strokes(strokes);

        // ���� Recognize ����
        IInkRecognitionResult* recognitionResult = nullptr;
        InkRecognitionStatus status;
        hr = recognizerContext->Recognize(&status, &recognitionResult);
        if (SUCCEEDED(hr) && recognitionResult != nullptr) {
            // ��ȡʶ���ѡ��
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

        // ����Դ�ͷ�ǰ������
        recognizerContext->Release();
        strokes->Release();
        inkDisp->Release();

        return results;
        });
}
