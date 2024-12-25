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
    // ��ʼ�� InkManager
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

        // ����ǰ���Ƶ�����ת��Ϊ InkStroke
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

        // ���� QFutureWatcher ������첽����
        QFutureWatcher<QString>* watcher = new QFutureWatcher<QString>(this);
        connect(watcher, &QFutureWatcher<QString>::finished, this, [this, watcher]() {
            QString result = watcher->result();
            emit recognitionResult(result); // ����ʶ�����ź�
            watcher->deleteLater(); // ���� QFutureWatcher ����
            });

        // �첽ִ��ʶ�����
        QFuture<QString> future = recognizeInkAsync(); // ��ȡ QFuture<QString> ����ֵ

        watcher->setFuture(future);
    }
}

void ScribbleArea::paintEvent(QPaintEvent* /* event */) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), Qt::white); // ��ձ���

    for (const auto& line : myLines) {
        painter.setPen(QPen(line.color, line.width));
        painter.drawLine(line.line);
    }
}

void ScribbleArea::drawLineTo(const QPoint& endPoint) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    // ������������
    myLines.append({ Qt::black, myPenWidth, QLine(lastPoint, endPoint) });

    lastPoint = endPoint;
    update(); // �����ػ�
}

QFuture<QString> ScribbleArea::recognizeInkAsync() {
    return QtConcurrent::run([this]() {  // �첽ִ�еĴ���飬lambda ���ʽ
        if (!inkManager) {
            return QString("InkManager not initialized");
        }

        // ����Ƿ��п��õ�ʶ����
        auto recognizerContainer = inkManager.GetRecognizers();
        if (recognizerContainer.Size() == 0) {
            return QString("No recognizers found.");
        }

        // ʹ�õ�һ��ʶ����
        auto recognizer = recognizerContainer.GetAt(0);
        inkManager.SetDefaultRecognizer(recognizer);

        // ��� InkManager �Ƿ������Ч����
        auto strokes = inkManager.GetStrokes();
        if (strokes.Size() == 0) {
            return QString("No strokes available for recognition.");
        }

        // ����ʶ�𷽷�
        auto recognitionResults = inkManager.RecognizeAsync(winrt::Windows::UI::Input::Inking::InkRecognitionTarget::All).get();

        QString recognizedText;
        for (const auto& recognitionResult : recognitionResults) {
            auto textCandidates = recognitionResult.GetTextCandidates();
            if (textCandidates.Size() > 0) {
                recognizedText += QString::fromWCharArray(textCandidates.GetAt(0).c_str()) + " ";
            }
        }

        return recognizedText.trimmed(); // ����ʶ����ı�
        });
}

