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
	enum class NonChineseInputMode { // ����������ģʽ
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
	void updateNonChineseWidget(NonChineseInputMode mode); // ���·����������buttons����ʾ����
	uint qHash(const KMainWindow::NonChineseInputMode& key, uint seed = 0) noexcept;


// �й�init�ĺ���
    void init();

public slots:
    void updateButtonLabels(const QStringList& labels);
	void updateCustomColorButtonColor(); // ���ڸ���showCustomColorButton����ɫ
	void setPenColorFromButton(); // �����ڵ����ɫ��ťʱ���ñʵ���ɫ
	void updateButtonFontSize(int pattern); // ���������С
    void updateCustomWidthLineWidth(); // ���ڸ�����ʾ�Զ���ʵĴ�ϸ��Line
	void sendText(QString text); // ������ֵ����λ��

private slots:
    void updateUndoButton(bool canUndo); // ����undo��ť�Ƿ���� 
	void updateClearButton(bool canClear); // ����clear��ť�Ƿ����
	void updateRecognitionMode(); // ����ʶ��ģʽ
	void showNonChineseInputWidget(); // ��ʾ����������
	void hideNonChineseInputWidget(); // ���ط���������
};
