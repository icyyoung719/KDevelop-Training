#include "kmainwindow.h"

KMainWindow::KMainWindow(QWidget* parent)
	: QMainWindow(parent), tsfManager(new TSFManager())
{
    ui.setupUi(this);

    connect(ui.scribblearea, &ScribbleArea::recognitionResults, this, &KMainWindow::updateButtonLabels);
    connect(ui.scribblearea, &ScribbleArea::canUndoChanged, this, &KMainWindow::updateUndoButton);
    connect(ui.scribblearea, &ScribbleArea::canClearChanged, this, &KMainWindow::updateClearButton);

    // 连接按钮
    connect(ui.singleWordPushButton, &QPushButton::clicked, this, &KMainWindow::onClickedButton);
    connect(ui.undoOneStrokeButton, &QPushButton::pressed, ui.scribblearea, &ScribbleArea::undo);
    connect(ui.clearStrokesButton, &QPushButton::pressed, ui.scribblearea, &ScribbleArea::clear);

	// 将显示文本的按钮的clicked信号连接到kmainwindow的onClickedButton槽函数
    connect(ui.pushButton_5, &QPushButton::clicked, this, [this]() {
        tsfManager->InsertTextAtCaret(ui.pushButton_5->text().toStdWString().c_str());
        });


    // 将 R G B 三个滑动条的 valueChanged 信号连接到 updateCustomColorButtonColor 槽函数 
    connect(ui.RValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCustomColorButtonColor()));
    connect(ui.GValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCustomColorButtonColor()));
    connect(ui.BValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCustomColorButtonColor()));

	// 将lineWidthSpinBox的valueChanged信号连接到updateCustomWidthLineWidth槽函数
	connect(ui.lineWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCustomWidthLineWidth()));

	// 将colorButton的clicked信号连接到ScribbleArea的setPenColor槽函数
    connect(ui.greyColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
    connect(ui.redColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
    connect(ui.yellowColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
    connect(ui.greenColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
	connect(ui.showCustomColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
    connect(ui.chooseCustomColorButton, &QPushButton::clicked, this, [this]() { // chooseCustomColorButton借用showCustomColorButton
		ui.showCustomColorButton->click();
		});

    // 将settingPushButton和returnHandwritingPushButton的clicked信号连接到kmainwindow的页面切换
	connect(ui.settingPushButton, &QPushButton::clicked, this, [this]() {
		ui.tabWidget->setCurrentIndex(1);
		});
    connect(ui.returnHandwritingPushButton, &QPushButton::clicked, this, [this]() {
        ui.tabWidget->setCurrentIndex(0);
        });

	// 测试代码
	ui.punctuationModeButton->setDisabled(true);

	// init后的默认状态
    updateButtonLabels(ScribbleArea::defaultRecognitionResults); // 更新显示的默认文字
    updateUndoButton(false); // 更新撤销按钮状态
    updateClearButton(false); // 更新清空按钮状态
    updateCustomColorButtonColor(); // 更新showCustomColorButton的颜色
	ui.tabWidget->tabBar()->hide(); // 隐藏tabBar

    // 设置窗口始终在最上层
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

KMainWindow::~KMainWindow() {
    delete tsfManager;
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
    ui.undoOneStrokeButton->setEnabled(canUndo); // 更新撤销按钮是否可用
}

void KMainWindow::updateClearButton(bool canClear) {
    ui.clearStrokesButton->setEnabled(canClear); // 更新清空按钮是否可用
}

void KMainWindow::onClickedButton()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString buttonText = button->text();
        tsfManager->InsertTextAtCaret(buttonText.toStdWString().c_str());
    }
}

void KMainWindow::updateCustomColorButtonColor()
{
	// 获取滑动条的值
	int r = ui.RValueSpinBox->value();
	int g = ui.GValueSpinBox->value();
	int b = ui.BValueSpinBox->value();

	// 设置按钮的背景颜色
	ui.showCustomColorButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b));
}

void KMainWindow::setPenColorFromButton() {
    // 获取发送信号的按钮
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        // 获取按钮的背景颜色
        QColor color = button->palette().color(QPalette::Button);
        // 设置笔迹颜色
        ui.scribblearea->setPenColor(color);
    }
}

void KMainWindow::updateCustomWidthLineWidth() {
	// 获取滑动条的值
	int width = ui.lineWidthSpinBox->value();
	// 设置显示的线的粗细
	ui.showCustomWidthLine->setMidLineWidth(width);
	// 更新笔的粗细
	ui.scribblearea->setPenWidth(width);
    // TODO!!!!!
}