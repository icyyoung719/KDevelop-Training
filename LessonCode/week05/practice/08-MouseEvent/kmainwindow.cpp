#include "kmainwindow.h"

KMainWindow::KMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton, &QPushButton::clicked, this, &KMainWindow::onClickedButton);
}

void KMainWindow::onClickedButton()
{
    ui.pushButton->setText("Button");
}
