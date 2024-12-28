#include "sendtextbutton.h"
#include <QDebug>
#include <QMouseEvent>

SendTextButton::SendTextButton(QWidget* parent)
    : QPushButton(parent) {
}

void SendTextButton::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && this->rect().contains(event->pos())) {
        // 模拟 clicked 信号的行为
        qDebug() << "Button clicked:" << this->text();

        // 触发默认的 clicked 信号
        emit clicked();
    }

    // 调用基类的 mouseReleaseEvent 确保默认行为仍然发生
    QPushButton::mouseReleaseEvent(event);
}

