#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCursor>
#include <QDebug>
#include <QGraphicsItem>
#include <QTransform>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->InitUi();

    m_view = new MyView(this);
    m_view->setGeometry(0,0,600,400);
    m_view->scale(1, -1);
    m_view->show();

    QPen p;
    p.setWidth(5);
    p.setColor(Qt::blue);

//    ui->centralWidget->setMouseTracking(true);
//    this->setMouseTracking(true);   //鼠标不按下的移动也能捕捉到MouseMoveEvent

    foreach(QAction* act, ptActions)
    connect(act, &QAction::triggered,m_view, &MyView::setPt);
    foreach(QAction* act, lineActions)
    connect(act, &QAction::triggered, m_view, &MyView::setLine );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitUi()
{
    QMenu* ptMenu = new QMenu(this);
    ptActions<< ui->act1 << ui->act2 << ui->act3;
    ptMenu->addActions(ptActions);
    ui->DrawPt->setMenu(ptMenu);

    QMenu* lineMenu = new QMenu(this);
    lineActions<< ui->actLine_1 <<ui->actLine_2;
    lineMenu->addActions(lineActions);
    ui->DrawLine->setMenu(lineMenu);
}
