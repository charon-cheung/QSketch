#include "myview.h"
#include <QDebug>
#include <QtMath>

MyView::MyView(QWidget *parent):
    QGraphicsView(parent)   // 初始化
{
//    this->setDragMode(QGraphicsView::ScrollHandDrag);

//    m_scene = new MyScene(0);
//    m_scene->setSceneRect(-200,-150,400,300); //场景坐标系,超出view大小加滑条
//    m_scene->setBackgroundBrush(QBrush(Qt::darkCyan));
//    this->setScene(m_scene);
//    this->scale(1, -1);
    int a;
}

MyView::~MyView()
{

}

void MyView::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"my view clicked"<<qrand()%90;
    start = this->mapToScene(event->pos());
    Line = m_scene->addLine(QLineF(QPointF(0,0), start) );

}

void MyView::mouseMoveEvent(QMouseEvent *event)
{
    end = this->mapToScene(event->pos());

    qreal angle1= qAtan2( start.y(),start.x());
    qreal angle2= qAtan2( end.y(), end.x());
    Line->setRotation((angle2 - angle1)*180/3.14159);

//    Line->setScale(event->pos().x());
}

void MyView::mouseReleaseEvent(QMouseEvent *event)
{

}
