#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_kmainwindow.h"
#include "tsfmanager.h"

class KMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    KMainWindow(QWidget* parent = Q_NULLPTR);
    ~KMainWindow();

public slots:
    void updateButtonLabels(const QStringList& labels);
	void updateCustomColorButtonColor(); // 用于更新showCustomColorButton的颜色
	void setPenColorFromButton(); // 用于在点击颜色按钮时设置笔的颜色
    void updateCustomWidthLineWidth(); // 用于更新显示自定义笔的粗细的Line

private slots:
    void onClickedButton(); // 示例
    void updateUndoButton(bool canUndo); // 更新undo按钮是否可用 
    void updateClearButton(bool canClear);

private:
    Ui::KMainWindowClass ui;
	TSFManager* tsfManager;
};