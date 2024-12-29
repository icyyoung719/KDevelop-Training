#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_kmainwindow.h"
#include "tsfmanager.h"
#include <QMouseEvent>
#include <windows.h>

class KMainWindow : public QMainWindow
{
    Q_OBJECT

private:
	Ui::KMainWindowClass ui;
	TSFManager* tsfManager;

public:
	enum class NonChineseInputMode { // 非中文输入模式
		Number,
		Letter,
		Punctuation
	};


public:
    KMainWindow(QWidget* parent = Q_NULLPTR);
    ~KMainWindow();

protected:
	void mousePressEvent(QMouseEvent* event) override;

private:
	void updateNonChineseWidget(NonChineseInputMode mode); // 更新非中文输入的buttons的显示文字
	uint qHash(const KMainWindow::NonChineseInputMode& key, uint seed = 0) noexcept;


// 有关init的函数
    void init();

public slots:
    void updateButtonLabels(const QStringList& labels);
	void updateCustomColorButtonColor(); // 用于更新showCustomColorButton的颜色
	void setPenColorFromButton(); // 用于在点击颜色按钮时设置笔的颜色
	void updateButtonFontSize(int pattern); // 更新字体大小
    void updateCustomWidthLineWidth(); // 用于更新显示自定义笔的粗细的Line
	void sendText(QString text); // 输出文字到光标位置

private slots:
    void updateUndoButton(bool canUndo); // 更新undo按钮是否可用 
	void updateClearButton(bool canClear); // 更新clear按钮是否可用
	void updateRecognitionMode(); // 更新识别模式
	void showNonChineseInputWidget(); // 显示非中文输入
	void hideNonChineseInputWidget(); // 隐藏非中文输入
};
