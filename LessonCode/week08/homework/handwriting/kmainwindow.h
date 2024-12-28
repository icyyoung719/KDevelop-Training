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
	void updateCustomColorButtonColor(); // ���ڸ���showCustomColorButton����ɫ
	void setPenColorFromButton(); // �����ڵ����ɫ��ťʱ���ñʵ���ɫ
    void updateCustomWidthLineWidth(); // ���ڸ�����ʾ�Զ���ʵĴ�ϸ��Line

private slots:
    void onClickedButton(); // ʾ��
    void updateUndoButton(bool canUndo); // ����undo��ť�Ƿ���� 
    void updateClearButton(bool canClear);

private:
    Ui::KMainWindowClass ui;
	TSFManager* tsfManager;
};