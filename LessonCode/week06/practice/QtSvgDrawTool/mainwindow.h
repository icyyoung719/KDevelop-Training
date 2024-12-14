#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "kxcanvas.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

	void onDrawRectBtnClicked();
	void onDrawLineBtnClicked();
	void onDrawEllipseBtnClicked();
    void onDrawHexBtnClicked();

signals:
    void onDrawingFlagChanged(KxCanvas::KDrawFlag drawFlag);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H