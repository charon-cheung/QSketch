#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_scene = new QGraphicsScene(this);
    m_scene->addLine(QLine(QPoint(15,30), QPoint(120,200)));
    m_scene->addEllipse(QRectF(QPointF(90,80), QSizeF(80,110)));
    ui->graphicsView->setScene(m_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}
