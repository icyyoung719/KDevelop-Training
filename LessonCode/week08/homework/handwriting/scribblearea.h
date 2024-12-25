#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <QFuture>
#include <QStringList>
#include <QtConcurrent/QtConcurrent>

#include <msinkaut.h>
#include <msinkaut_i.c> // Tablet PC SDK �� COM �ӿڶ���

class ScribbleArea : public QWidget {
    Q_OBJECT

public:
    explicit ScribbleArea(QWidget* parent = nullptr);
    ~ScribbleArea();

signals:
    void recognitionResults(const QStringList& results); // ʶ�����ź�

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    IInkCollector* inkCollector; // Tablet PC SDK ��ī���ռ���

    QFuture<QStringList> recognizeInkAsync(); // �첽��дʶ��
    void onRecognitionFinished();            // ����ʶ����
};

#endif // SCRIBBLEAREA_H
