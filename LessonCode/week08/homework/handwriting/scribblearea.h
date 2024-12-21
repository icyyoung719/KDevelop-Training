#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QtWidgets/QWidget>
#include <QVector>
#include <QPoint>
#include <QColor>
#include <QString>

// ���� WinRT �� Windows Ink ��ص�ͷ�ļ�
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Input.Inking.h>
#include <winrt/Windows.Data.Xml.Dom.h>

class ScribbleArea : public QWidget {
    Q_OBJECT

public:
    explicit ScribbleArea(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    struct LineData {
        QColor color;
        int width;
        QLine line;
    };

    QVector<LineData> myLines; // ������Ƶ���������
    QPoint lastPoint;          // ��һ�����λ��
    bool scribbling;           // �Ƿ����ڻ���
    int myPenWidth;            // �ʴ����

    winrt::Windows::UI::Input::Inking::InkManager inkManager; // InkManager ʵ��

    void drawLineTo(const QPoint& endPoint);
    QString recognizeInk(); // ���ñʼ�ʶ��
};

#endif // SCRIBBLEAREA_H
