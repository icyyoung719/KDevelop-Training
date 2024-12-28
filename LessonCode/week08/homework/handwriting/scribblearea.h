#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <QFuture>
#include <QStringList>
#include <QtConcurrent/QtConcurrent>

#include <msinkaut.h>
#include <msinkaut_i.c> // Tablet PC SDK �� COM �ӿڶ���

#include "penattributes.h" // ī����ͼ����

class ScribbleArea : public QWidget {
    Q_OBJECT

public:
    static const QStringList defaultRecognitionResults; // Ĭ��ʶ����

    explicit ScribbleArea(QWidget* parent = nullptr);
    ~ScribbleArea();

    enum class RecognitionMode { // ʶ��ģʽ
        Auto,
        Word,
        Sentence
    };

    void setPenColor(const QColor& color); // ���ñʵ���ɫ
    void setPenWidth(int width);           // ���ñʵĴ�ϸ
    void setRecognitionMode(RecognitionMode mode); // ����ʶ��ģʽ

signals:
    void recognitionResults(const QStringList& results); // ʶ�����źţ�emit��ˢ��ҳ������ʾ������
    void canUndoChanged(bool canUndo); // �Ƿ���Գ����źţ�emit�Ը��°�ť�Ƿ����
    void canClearChanged(bool canClear); // �Ƿ��������źţ�emit�Ը��°�ť�Ƿ����

public slots:
    void undo(); // ����
    void clear(); // ���

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    IInkCollector* inkCollector; // Tablet PC SDK ��ī���ռ���
    QList<IInkStrokeDisp*> strokesList; // ��¼�ѻ��Ƶ����бʴ������ڳ�����

    PenAttributes penAttributes; // �ʵ�����
    RecognitionMode recognitionMode; // ��ǰʶ��ģʽ

    QFuture<QStringList> recognizeInkAsync(); // �첽��дʶ��
};

#endif // SCRIBBLEAREA_H
