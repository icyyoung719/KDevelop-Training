#ifndef PENATTRIBUTES_H
#define PENATTRIBUTES_H

#include <QColor>
#include <msinkaut.h>

class PenAttributes {
public:
    PenAttributes();
	PenAttributes(IInkDrawingAttributes* attributes); // 从已有的墨迹绘图属性创建
    ~PenAttributes();

    void setColor(const QColor& color);
    void setWidth(int width);

    IInkDrawingAttributes* getDrawingAttributes() const;

private:
    IInkDrawingAttributes* drawingAttributes; // 墨迹绘图属性
};

#endif // PENATTRIBUTES_H