#include "sendtextbutton.h"
#include <QDebug>
#include <QMouseEvent>

SendTextButton::SendTextButton(QWidget* parent)
    : QPushButton(parent) {
}

void SendTextButton::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && this->rect().contains(event->pos())) {
        // ģ�������tsf������ qDebug ����
        qDebug() << "Button clicked:" << this->text();
        emit sendText(this->text());

        // ����Ĭ�ϵ� clicked �ź�
        emit clicked();
    }

    // ���û���� mouseReleaseEvent ȷ��Ĭ����Ϊ��Ȼ����
    QPushButton::mouseReleaseEvent(event);
}

