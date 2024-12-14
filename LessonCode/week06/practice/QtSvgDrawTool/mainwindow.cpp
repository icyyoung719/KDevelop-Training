#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    (void)connect(ui->m_pRectBtn,&QPushButton::clicked,this,&MainWindow::onDrawRectBtnClicked);
    (void)connect(ui->m_pLineBtn, &QPushButton::clicked,this, &MainWindow::onDrawLineBtnClicked);
    (void)connect(ui->m_pCircleBtn, &QPushButton::clicked, this,&MainWindow::onDrawEllipseBtnClicked);
    (void)connect(ui->m_pHexBtn, &QPushButton::clicked, this, &MainWindow::onDrawHexBtnClicked);
    (void)connect(this,&MainWindow::onDrawingFlagChanged,ui->m_pcanvas,&KxCanvas::updateDrawingFlag);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onDrawRectBtnClicked()
{
    emit onDrawingFlagChanged(KxCanvas::KDrawFlag::RectDrawFlag);
}

void MainWindow::onDrawLineBtnClicked()
{
    emit onDrawingFlagChanged(KxCanvas::KDrawFlag::LineDrawFlag);
}

void MainWindow::onDrawEllipseBtnClicked()
{
    emit onDrawingFlagChanged(KxCanvas::KDrawFlag::CircleDrawFlag);
}
void MainWindow::onDrawHexBtnClicked()
{
    emit onDrawingFlagChanged(KxCanvas::KDrawFlag::HexDrawFlag);
}


