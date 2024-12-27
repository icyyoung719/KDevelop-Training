#include "penattributes.h"
#include <QDebug>

PenAttributes::PenAttributes() {
    HRESULT hr = CoCreateInstance(CLSID_InkDrawingAttributes, nullptr, CLSCTX_INPROC_SERVER, IID_IInkDrawingAttributes, (void**)&drawingAttributes);
    if (FAILED(hr)) {
        qDebug() << "Failed to create InkDrawingAttributes. Error:" << hr;
        drawingAttributes = nullptr;
    }
}

PenAttributes::PenAttributes(IInkDrawingAttributes* attributes)
    : drawingAttributes(attributes) {
    if (drawingAttributes) {
        drawingAttributes->AddRef();
    }
}

PenAttributes::~PenAttributes() {
    if (drawingAttributes) {
        drawingAttributes->Release();
    }
}

void PenAttributes::setColor(const QColor& color) {
    if (!drawingAttributes) return;

    COLORREF rgbColor = RGB(color.red(), color.green(), color.blue());
    HRESULT hr = drawingAttributes->put_Color(rgbColor);
    if (FAILED(hr)) {
        qDebug() << "Failed to set pen color. Error:" << hr;
    }
}

void PenAttributes::setWidth(int width) {
    if (!drawingAttributes) return;

    HRESULT hr = drawingAttributes->put_Width(static_cast<float>(width));
    if (FAILED(hr)) {
        qDebug() << "Failed to set pen width. Error:" << hr;
    }
}

IInkDrawingAttributes* PenAttributes::getDrawingAttributes() const {
    return drawingAttributes;
}