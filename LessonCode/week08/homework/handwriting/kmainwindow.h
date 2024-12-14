#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_kmainwindow.h"

class KMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    KMainWindow(QWidget* parent = Q_NULLPTR);
    void onClickedButton();
private:
    Ui::KMainWindowClass ui;
};
