#include "kmainwindow.h"
#include <QPixmap>

KMainWindow::KMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.label->setPixmap(QPixmap(":/image/peiqi.png"));
    ui.label_2->setText("Input your circle radius");//°ë¾¶
    ui.label_3->setText("The result is");
    ui.label_4->setText("Unknown");

    ui.pushButton->setText("get space");
    ui.pushButton_2->setText("get circle");

    //connect(ui.pushButton, &QPushButton::clicked, this, &KMainWindow::showMessage);
    connect(ui.pushButton, &QPushButton::clicked, this, &KMainWindow::calculateSpace);
    connect(ui.pushButton_2, &QPushButton::clicked, this, &KMainWindow::calculateCircle);
}

void KMainWindow::showMessage() {
    ui.label_2->setText("hello");
}
void KMainWindow::calculateSpace() {
    int radius = ui.lineEdit->text().toInt();
    double space = 3.14 * radius * radius;
    ui.label_4->setText(QString::number(space));
}
void KMainWindow::calculateCircle() {
    int radius = ui.lineEdit->text().toInt();
    double circle = 2 * 3.14 * radius;
    ui.label_4->setText(QString::number(circle));
}
