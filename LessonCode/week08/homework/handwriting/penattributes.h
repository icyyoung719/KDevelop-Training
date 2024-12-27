#ifndef PENATTRIBUTES_H
#define PENATTRIBUTES_H

#include <QColor>
#include <msinkaut.h>

class PenAttributes {
public:
    PenAttributes();
	PenAttributes(IInkDrawingAttributes* attributes); // �����е�ī����ͼ���Դ���
    ~PenAttributes();

    void setColor(const QColor& color);
    void setWidth(int width);

    IInkDrawingAttributes* getDrawingAttributes() const;

private:
    IInkDrawingAttributes* drawingAttributes; // ī����ͼ����
};

#endif // PENATTRIBUTES_H