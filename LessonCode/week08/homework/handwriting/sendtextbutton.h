#pragma once

#include <QPushButton>
#include <QString>

class SendTextButton : public QPushButton {
    Q_OBJECT

public:
    SendTextButton(QWidget* parent = nullptr);

protected:
    // 重写鼠标释放事件
    void mouseReleaseEvent(QMouseEvent* event) override;
signals:
    void sendText(const QString& text);
};
