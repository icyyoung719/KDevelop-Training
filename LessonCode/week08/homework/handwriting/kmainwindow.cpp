#include "kmainwindow.h"

KMainWindow::KMainWindow(QWidget* parent)
	: QMainWindow(parent), tsfManager(new TSFManager())
{
    ui.setupUi(this);

    connect(ui.scribblearea, &ScribbleArea::recognitionResults, this, &KMainWindow::updateButtonLabels);
    connect(ui.scribblearea, &ScribbleArea::canUndoChanged, this, &KMainWindow::updateUndoButton);
    connect(ui.scribblearea, &ScribbleArea::canClearChanged, this, &KMainWindow::updateClearButton);

    // ���Ӱ�ť
    connect(ui.singleWordPushButton, &QPushButton::clicked, this, &KMainWindow::onClickedButton);
    connect(ui.undoOneStrokeButton, &QPushButton::pressed, ui.scribblearea, &ScribbleArea::undo);
    connect(ui.clearStrokesButton, &QPushButton::pressed, ui.scribblearea, &ScribbleArea::clear);

	// ����ʾ�ı��İ�ť��clicked�ź����ӵ�kmainwindow��onClickedButton�ۺ���
    connect(ui.pushButton_5, &QPushButton::clicked, this, [this]() {
        tsfManager->InsertTextAtCaret(ui.pushButton_5->text().toStdWString().c_str());
        });


    // �� R G B ������������ valueChanged �ź����ӵ� updateCustomColorButtonColor �ۺ��� 
    connect(ui.RValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCustomColorButtonColor()));
    connect(ui.GValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCustomColorButtonColor()));
    connect(ui.BValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCustomColorButtonColor()));

	// ��lineWidthSpinBox��valueChanged�ź����ӵ�updateCustomWidthLineWidth�ۺ���
	connect(ui.lineWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCustomWidthLineWidth()));

	// ��colorButton��clicked�ź����ӵ�ScribbleArea��setPenColor�ۺ���
    connect(ui.greyColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
    connect(ui.redColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
    connect(ui.yellowColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
    connect(ui.greenColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
	connect(ui.showCustomColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
    connect(ui.chooseCustomColorButton, &QPushButton::clicked, this, [this]() { // chooseCustomColorButton����showCustomColorButton
		ui.showCustomColorButton->click();
		});

    // ��settingPushButton��returnHandwritingPushButton��clicked�ź����ӵ�kmainwindow��ҳ���л�
	connect(ui.settingPushButton, &QPushButton::clicked, this, [this]() {
		ui.tabWidget->setCurrentIndex(1);
		});
    connect(ui.returnHandwritingPushButton, &QPushButton::clicked, this, [this]() {
        ui.tabWidget->setCurrentIndex(0);
        });

	// ���Դ���
	ui.punctuationModeButton->setDisabled(true);

	// init���Ĭ��״̬
    updateButtonLabels(ScribbleArea::defaultRecognitionResults); // ������ʾ��Ĭ������
    updateUndoButton(false); // ���³�����ť״̬
    updateClearButton(false); // ������հ�ť״̬
    updateCustomColorButtonColor(); // ����showCustomColorButton����ɫ
	ui.tabWidget->tabBar()->hide(); // ����tabBar

    // ���ô���ʼ�������ϲ�
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
    ui.undoOneStrokeButton->setEnabled(canUndo); // ���³�����ť�Ƿ����
}

void KMainWindow::updateClearButton(bool canClear) {
    ui.clearStrokesButton->setEnabled(canClear); // ������հ�ť�Ƿ����
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
	// ��ȡ��������ֵ
	int r = ui.RValueSpinBox->value();
	int g = ui.GValueSpinBox->value();
	int b = ui.BValueSpinBox->value();

	// ���ð�ť�ı�����ɫ
	ui.showCustomColorButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b));
}

void KMainWindow::setPenColorFromButton() {
    // ��ȡ�����źŵİ�ť
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        // ��ȡ��ť�ı�����ɫ
        QColor color = button->palette().color(QPalette::Button);
        // ���ñʼ���ɫ
        ui.scribblearea->setPenColor(color);
    }
}

void KMainWindow::updateCustomWidthLineWidth() {
	// ��ȡ��������ֵ
	int width = ui.lineWidthSpinBox->value();
	// ������ʾ���ߵĴ�ϸ
	ui.showCustomWidthLine->setMidLineWidth(width);
	// ���±ʵĴ�ϸ
	ui.scribblearea->setPenWidth(width);
    // TODO!!!!!
}