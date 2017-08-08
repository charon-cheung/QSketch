#include "myview.h"
#include <QDebug>
#include <QtMath>

MyView::MyView(QWidget *parent):
    QGraphicsView(parent)   // 初始化
{
    //    this->setDragMode(QGraphicsView::ScrollHandDrag);

    m_scene = new MyScene(0);
    m_scene->setSceneRect(-200,-150,400,300); //场景坐标系,超出view大小加滑条
    m_scene->setBackgroundBrush(QBrush(Qt::darkCyan));
    this->setScene(m_scene);
    drawPt = false;
    drawLine = false;
}

MyView::~MyView()
{

}

void MyView::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"my view clicked"<<qrand()%90;
    start = this->mapToScene(event->pos());
    if(drawLine && !drawLinePos)
    {
        Line = m_scene->addLine(QLineF(QPointF(0,0), start) );
    }
    else if(drawLine && drawLinePos)
    {
        qDebug()<<"draw line pos";
    }
    else if(drawPt && drawCirPt)    // 画圆点,start为圆心,pt_size为半径
    {
        m_scene->addEllipse(start.x()-pt_size, start.y()-pt_size, 2*pt_size, 2*pt_size,
                            QPen(QColor(Qt::black)), QBrush(Qt::SolidPattern));
    }
    else if(drawPt && drawCross)    // 画X样式的点
    {
        QPointF p1 = start+QPoint(pt_size,pt_size);
        QPointF p2 = start+QPoint(-pt_size,-pt_size);
        QPointF p3 = start+QPoint(-pt_size,pt_size);
        QPointF p4 = start+QPoint(pt_size,-pt_size);
        m_scene->addLine(QLineF(p1, p2), QPen(QColor(Qt::black)));
        m_scene->addLine(QLineF(p3, p4), QPen(QColor(Qt::black)));
    }
    else if(drawPt && drawPtPos)
    {
        qDebug()<<"画点的坐标"<<pt1;
//        m_scene->addEllipse(pt1.x()-pt_size, pt1.y()-pt_size, 2*pt_size, 2*pt_size,
//                            QPen(QColor(Qt::black)), QBrush(Qt::SolidPattern));

    }
}

void MyView::mouseMoveEvent(QMouseEvent *event)
{
//    end = this->mapToScene(event->pos());

//    qreal angle1= qAtan2( start.y(),start.x());
//    qreal angle2= qAtan2( end.y(), end.x());
//    Line->setRotation((angle2 - angle1)*180/3.14159);
    //    Line->setScale(event->pos().x());
}

void MyView::mouseReleaseEvent(QMouseEvent *event)
{

}

void MyView::setLine()
{
    drawLine=true;
    drawPt=false;
    if(sender()->objectName() == "actLine_1")
    {
        drawLinePos=false;
    }
    else if(sender()->objectName() == "actLine_2")
    {
        drawLinePos=true;
    }
}

void MyView::setPt()
{
    qDebug()<<sender()->objectName();
    drawLine=false;
    drawPt=true;
    if(sender()->objectName() == "act1")
    {
        drawCirPt=true;
        drawCross=false;
        drawPtPos=false;
    }
    else if(sender()->objectName() == "act2")
    {
        drawCirPt=false;
        drawCross=true;
        drawPtPos=false;
    }
    else if(sender()->objectName() == "act3")
    {
        drawCirPt=false;
        drawCross=false;
        drawPtPos=true;
        dlg = new PosDialog(this);
        if(dlg->exec() != QDialog::Accepted)    return;
        pt1 = dlg->getPt();
        m_scene->addEllipse(pt1.x()-pt_size, pt1.y()-pt_size, 2*pt_size, 2*pt_size,
                            QPen(QColor(Qt::black)), QBrush(Qt::SolidPattern));
    }
}
