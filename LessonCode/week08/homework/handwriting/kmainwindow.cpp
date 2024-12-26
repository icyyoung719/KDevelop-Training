#include "kmainwindow.h"

KMainWindow::KMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.scribblearea, &ScribbleArea::recognitionResults, this, &KMainWindow::updateButtonLabels);
    connect(ui.scribblearea, &ScribbleArea::canUndoChanged, this, &KMainWindow::updateUndoButton);
    connect(ui.scribblearea, &ScribbleArea::canClearChanged, this, &KMainWindow::updateClearButton);
    // 连接按钮
    connect(ui.pushButton, &QPushButton::clicked, this, &KMainWindow::onClickedButton);

    connect(ui.pushButton_3, &QPushButton::clicked, ui.scribblearea, &ScribbleArea::undo);
    connect(ui.pushButton_4, &QPushButton::clicked, ui.scribblearea, &ScribbleArea::clear);

    updateButtonLabels(ScribbleArea::defaultRecognitionResults); // 更新显示的默认文字
}

void KMainWindow::updateButtonLabels(const QStringList& labels) {
    if (labels.size() >= 10) {
        ui.pushButton_5->setText(labels.at(0));
        ui.pushButton_6->setText(labels.at(1));
        ui.pushButton_7->setText(labels.at(2));
        ui.pushButton_8->setText(labels.at(3));
        ui.pushButton_9->setText(labels.at(4));
        ui.pushButton_10->setText(labels.at(5));
        ui.pushButton_11->setText(labels.at(6));
        ui.pushButton_12->setText(labels.at(7));
        ui.pushButton_13->setText(labels.at(8));
        ui.pushButton_14->setText(labels.at(9));
    }
}

void KMainWindow::updateUndoButton(bool canUndo) {
    ui.pushButton_3->setEnabled(canUndo); // 更新撤销按钮是否可用
}

void KMainWindow::updateClearButton(bool canClear) {
    ui.pushButton_4->setEnabled(canClear); // 更新清空按钮是否可用
}

void KMainWindow::onClickedButton()
{
    ui.pushButton->setText("Button");
}
