#include "scribblearea.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <winrt/Windows.Foundation.Collections.h>

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

        // ����ʶ���ܲ�������
        qDebug() << "Recognized Text:" << recognizeInk();
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

QString ScribbleArea::recognizeInk() {
    if (!inkManager) {
        return QString("InkManager not initialized");
    }

    try {
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

    }
    catch (const winrt::hresult_error& ex) {
        // ���� WinRT �쳣�������ϸ������Ϣ
        qDebug() << "Recognition failed with error:" << QString::fromWCharArray(ex.message().c_str());
        return QString("Recognition failed.");
    }
    catch (...) {
        // ���������쳣
        qDebug() << "Unknown error occurred during recognition.";
        return QString("Recognition failed due to an unknown error.");
    }
}
