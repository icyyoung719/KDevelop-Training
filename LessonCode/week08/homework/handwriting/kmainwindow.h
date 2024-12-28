#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_kmainwindow.h"
#include "tsfmanager.h"

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

private:
	void updateNonChineseWidget(NonChineseInputMode mode); // 更新非中文输入的buttons的显示文字
	uint qHash(const KMainWindow::NonChineseInputMode& key, uint seed = 0) noexcept;

public slots:
    void updateButtonLabels(const QStringList& labels);
	void updateCustomColorButtonColor(); // 用于更新showCustomColorButton的颜色
	void setPenColorFromButton(); // 用于在点击颜色按钮时设置笔的颜色
    void updateCustomWidthLineWidth(); // 用于更新显示自定义笔的粗细的Line

private slots:
    void onClickedButton(); // 示例
    void updateUndoButton(bool canUndo); // 更新undo按钮是否可用 
	void updateClearButton(bool canClear); // 更新clear按钮是否可用
	void updateRecognitionMode(); // 更新识别模式
	void showNonChineseInputWidget(); // 显示非中文输入
};
