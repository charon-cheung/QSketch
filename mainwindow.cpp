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
    m_view = new MyView(this);
    m_view->setGeometry(0,0,600,400);

    m_scene = new MyScene();
    m_scene->setSceneRect(-200,-150,400,300); //场景坐标系,超出view大小加滑条
    m_scene->setBackgroundBrush(QBrush(Qt::darkCyan));

    m_view->setScene(m_scene);
    m_view->scale(1, -1);
    m_view->show();

    QPen p;
    p.setWidth(5);
    p.setColor(Qt::blue);

//    ui->centralWidget->setMouseTracking(true);
//    this->setMouseTracking(true);   //鼠标不按下的移动也能捕捉到MouseMoveEvent

    drawPt = false;
    drawLine = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}


#if 0
void MainWindow::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    qDebug()<<"------------------";
    QCursor cur;
    qDebug()<<"全局坐标:"<<cur.pos();    //鼠标的全局位置
    double pix = pt_size;

    if(drawPt)
    {

        qDebug()<<"窗口坐标:"<<e->pos();
        qDebug()<<"场景坐标:"<<QPoint(e->pos().x()-300, 200-e->pos().y());

        qDebug()<<"映射到场景的坐标:"<<mapPressPt;
        // 画圆点,mapPressPt为圆心,pix为半径
//        m_scene->addEllipse(mapPressPt.x()-pix, mapPressPt.y()-pix, 2*pix, 2*pix,
//                            QPen(QColor(Qt::black)), QBrush(Qt::SolidPattern));
        // 画×样式的点
        QPointF p1 = mapPressPt+QPoint(pt_size,pt_size);
        QPointF p2 = mapPressPt+QPoint(-pt_size,-pt_size);
        QPointF p3 = mapPressPt+QPoint(-pt_size,pt_size);
        QPointF p4 = mapPressPt+QPoint(pt_size,-pt_size);
        m_scene->addLine(QLineF(p1, p2), QPen(QColor(Qt::black)));
        m_scene->addLine(QLineF(p3, p4), QPen(QColor(Qt::black)));
    }

}

#endif

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{

}

void MainWindow::on_DrawLine_clicked()
{
    drawLine = true;
}


void MainWindow::on_DrawPtPos_clicked()
{

}
