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
    m_view->setGeometry(0,0,600,500);
    // 坐标放大倍数,倍数为1时,1个单位坐标就是1个像素
    m_view->scale(6, -6);
    m_view->show();

//    ui->centralWidget->setMouseTracking(true);
//    this->setMouseTracking(true);   //鼠标不按下的移动也能捕捉到MouseMoveEvent
    foreach(QAction* act, ptActions)
        connect(act, &QAction::triggered,m_view, &MyView::setPt);
    foreach(QAction* act, lineActions)
        connect(act, &QAction::triggered, m_view, &MyView::setLine );
    foreach(QAction* act, rectActions)
        connect(act, &QAction::triggered, m_view, &MyView::setRect );
    foreach(QAction* act, ellipseActions)
        connect(act, &QAction::triggered, m_view, &MyView::setEllipse );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitUi()
{
    this->setWindowTitle("QSketch");
    this->setWindowIcon(QIcon(":/Icon/Icon/ruler.png"));
    ui->DrawPt->setFocusPolicy(Qt::NoFocus);
    QMenu* ptMenu = new QMenu(this);
    ptActions<< ui->act1 << ui->act2 << ui->act3;
    ptMenu->addActions(ptActions);
    ui->DrawPt->setMenu(ptMenu);

    QMenu* lineMenu = new QMenu(this);
    lineActions<< ui->actLine_1 << ui->actLine_2 << ui->actLine_3;
    lineMenu->addActions(lineActions);
    ui->DrawLine->setMenu(lineMenu);

    QMenu* rectMenu = new QMenu(this);
    rectActions<< ui->actRect_1<<ui->actRect_2<<ui->actRect_3;
    rectMenu->addActions(rectActions);
    ui->DrawRect->setMenu(rectMenu);

    QMenu* ellipseMenu = new QMenu(this);
    ellipseActions<< ui->actEllipse_1<< ui->actEllipse_2;
    ellipseMenu->addActions(ellipseActions);
    ui->DrawEllipse->setMenu(ellipseMenu);
}
