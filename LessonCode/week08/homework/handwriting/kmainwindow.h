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
	enum class NonChineseInputMode { // ����������ģʽ
		Number,
		Letter,
		Punctuation
	};


public:
    KMainWindow(QWidget* parent = Q_NULLPTR);
    ~KMainWindow();

private:
	void updateNonChineseWidget(NonChineseInputMode mode); // ���·����������buttons����ʾ����
	uint qHash(const KMainWindow::NonChineseInputMode& key, uint seed = 0) noexcept;

public slots:
    void updateButtonLabels(const QStringList& labels);
	void updateCustomColorButtonColor(); // ���ڸ���showCustomColorButton����ɫ
	void setPenColorFromButton(); // �����ڵ����ɫ��ťʱ���ñʵ���ɫ
    void updateCustomWidthLineWidth(); // ���ڸ�����ʾ�Զ���ʵĴ�ϸ��Line

private slots:
    void onClickedButton(); // ʾ��
    void updateUndoButton(bool canUndo); // ����undo��ť�Ƿ���� 
	void updateClearButton(bool canClear); // ����clear��ť�Ƿ����
	void updateRecognitionMode(); // ����ʶ��ģʽ
	void showNonChineseInputWidget(); // ��ʾ����������
};
