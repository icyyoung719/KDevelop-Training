#include "kmainwindow.h"
#include <QPixmap>

KMainWindow::KMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.label->setPixmap(QPixmap(":/image/peiqi.png"));

    // 设置最小大小
    ui.label_2->setMinimumSize(200, 30);
    ui.label_3->setMinimumSize(200, 30);

    // 设置字体大小
    QFont font = ui.label_2->font();
    font.setPointSize(10);
    ui.label_2->setFont(font);

    QFont font2 = ui.label_3->font();
    font2.setPointSize(10);
    ui.label_3->setFont(font2);

    // 启用自动换行
    ui.label_2->setWordWrap(true);
    ui.label_3->setWordWrap(true);

    // 设置布局策略
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui.label_2->setSizePolicy(sizePolicy);
    ui.label_3->setSizePolicy(sizePolicy);

    ui.label_2->setText("Input your circle radius");//半径
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
