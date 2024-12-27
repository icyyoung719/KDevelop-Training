#include "scribblearea.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QColor>
#include <QFutureWatcher>

const QStringList ScribbleArea::defaultRecognitionResults = {
    QString::fromWCharArray(L"��"),
    QString::fromWCharArray(L"��"),
    QString::fromWCharArray(L"��"),
    QString::fromWCharArray(L"��"),
    QString::fromWCharArray(L"��"),
    QString::fromWCharArray(L"��"),
    QString::fromWCharArray(L"��"),
    QString::fromWCharArray(L"��"),
    QString::fromWCharArray(L"��")
};


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

    // ��ȡĬ�ϵĻ�ͼ���Բ���¡
    IInkDrawingAttributes* defaultDrawingAttributes = nullptr;
    hr = inkCollector->get_DefaultDrawingAttributes(&defaultDrawingAttributes);
    if (FAILED(hr)) {
        qDebug() << "Failed to get DefaultDrawingAttributes. Error:" << hr;
    }
    else {
        // ֱ��ʹ��Ĭ�ϵĻ�ͼ����
        penAttributes = PenAttributes(defaultDrawingAttributes);
    }
}

ScribbleArea::~ScribbleArea() {
    if (inkCollector) {
        inkCollector->put_Enabled(VARIANT_FALSE); // ����ī���ռ�
        inkCollector->Release();
    }
	// penAttributes �������������Զ��ͷ� IInkDrawingAttributes ����,����Ҫ�ֶ��ͷ�
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

        // ����ǰ�ʴ���ӵ������б�
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
						strokesList.push_back(stroke); // ��ӵ�ǰ�ʴ��������б�
                        //strokesList.append(stroke); // ��ӵ�ǰ�ʴ��������б�
                        emit canUndoChanged(true);  // ���³�����ť״̬
                        emit canClearChanged(true);  // ���³�����ť״̬
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

// �������һ���ʴ�
void ScribbleArea::undo() {
    if (strokesList.empty()) {
        emit canUndoChanged(false); // ���û�п��Գ����ıʴ�������ó�����ť
        return;
    }

    IInkDisp* inkDisp = nullptr;
    HRESULT hr = inkCollector->get_Ink(&inkDisp);
    if (FAILED(hr) || !inkDisp) {
        qDebug() << "Failed to get InkDisp for undo.";
        return;
    }

    // ��ȡ InkStrokes ����
    IInkStrokes* strokes = nullptr;
    hr = inkDisp->get_Strokes(&strokes);
    if (FAILED(hr) || !strokes) {
        qDebug() << "Failed to get strokes collection.";
        inkDisp->Release();
        return;
    }

    // ��ȡ��ǰ�ʴ�����
    long count = 0;
    strokes->get_Count(&count);
    qDebug() << "Number of strokes in InkDisp before undo:" << count << '\n';

    // ��ȡ���һ���ʴ����� InkDisp ��ɾ����
    IInkStrokeDisp* lastStroke = strokesList.back();
    if (lastStroke) {
        hr = strokes->Remove(lastStroke);
        hr = inkDisp->DeleteStroke(lastStroke);
        if (SUCCEEDED(hr)) {
            strokesList.pop_back(); // �ӱ����б����Ƴ��ñʴ�
            emit canUndoChanged(!strokesList.empty()); // ���³�����ť״̬
        }
        else {
            qDebug() << "Failed to delete stroke from InkDisp.";
        }
        lastStroke->Release();
    }

    // ��ȡ���º�ıʴ�����
    strokes->get_Count(&count);
    qDebug() << "Number of strokes in InkDisp after undo:" << count << '\n';

    strokes->Release();
    inkDisp->Release();

    // ǿ��ˢ�½�������ʾ���µ�ī��״̬
    update();
}

void ScribbleArea::clear() {
    // ���� InkCollector���������ī������
    if (inkCollector) {
        inkCollector->put_Enabled(VARIANT_FALSE); // ���� InkCollector
    }

    // ��ȡ InkDisp ����������е����бʴ�
    IInkDisp* inkDisp = nullptr;
    inkCollector->get_Ink(&inkDisp);  // ��ȡ��ǰ InkDisp ����
    if (inkDisp) {
        inkDisp->DeleteStrokes();
		inkDisp->Release();
    }

    // ��ձ��رʴ��б�
    strokesList.clear();

    // �������� InkCollector ��׼���ռ��µ�ī��
    HRESULT hr = inkCollector->put_Enabled(VARIANT_TRUE); // ���� InkCollector
    if (FAILED(hr)) {
        qDebug() << "Failed to re-enable InkCollector after clearing.";
    }

	// ����setPenColor��setPenWidth����
	//setPenColor(QColor(255, 0, 0)); // ���ñʵ���ɫΪ��ɫ
	//setPenWidth(5); // ���ñʵĴ�ϸΪ5

    // �����źŸ��½���״̬
    emit recognitionResults(ScribbleArea::defaultRecognitionResults); // ������ʾ��Ĭ������
    emit canUndoChanged(false); // ���³�����ť״̬
    emit canClearChanged(false); // ������հ�ť״̬
}

void ScribbleArea::setPenColor(const QColor& color) {
    penAttributes.setColor(color);
}

void ScribbleArea::setPenWidth(int width) {
    penAttributes.setWidth(width);
}
