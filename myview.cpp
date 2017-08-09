#include "myview.h"
#include <QDebug>
#include <QtMath>

MyView::MyView(QWidget *parent):
    QGraphicsView(parent)   // 初始化
{
//    this->setDragMode(QGraphicsView::ScrollHandDrag);
    m_scene = new MyScene(0);
    this->setScene(m_scene);
    drawPt = false;
    drawLine = false;
#if 0
    QRect viewport_rect(0, 0, this->viewport()->width(),
                        this->viewport()->height());
    QRectF visible_scene_rect = this->mapToScene(viewport_rect).boundingRect();
    viewCenter = new QGraphicsRectItem(visible_scene_rect);
    this->scene()->addItem(viewCenter);
#endif
}

MyView::~MyView()
{

}

void MyView::mousePressEvent(QMouseEvent *event)
{
    if(event->button()!=Qt::LeftButton)     return;
    qDebug()<<"my view clicked"<<qrand()%90;
//    窗口坐标转为场景坐标
    start = this->mapToScene(event->pos());
    if(drawPt && drawCirPt)    // 画圆点,start为圆心,pt_size为半径
    {
        m_scene->addEllipse(start.x()-pt_size, start.y()-pt_size, 2*pt_size, 2*pt_size,
                            QPen(QColor(Qt::white)), QBrush(Qt::white,Qt::SolidPattern) );
    }
    else if(drawPt && drawCross)    // 画X样式的点
    {
        QPointF p1 = start+QPoint(pt_size,pt_size);
        QPointF p2 = start+QPoint(-pt_size,-pt_size);
        QPointF p3 = start+QPoint(-pt_size,pt_size);
        QPointF p4 = start+QPoint(pt_size,-pt_size);
        m_scene->addLine(QLineF(p1, p2), QPen(QColor(Qt::white)));
        m_scene->addLine(QLineF(p3, p4), QPen(QColor(Qt::white)));
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
    end = this->mapToScene(event->pos());
    if(drawLine && !drawLinePos)
    {
        Line = m_scene->addLine(QLineF(start,end),QPen(QColor(Qt::white)));
    }
}
//视图放大和缩小
void MyView::wheelEvent(QWheelEvent *event)
{
    // 加这句后，放大缩小时，原点会跟随鼠标
//    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    qreal scaleFactor = qPow(2.0, event->delta() / 240.0);
    this->scale(scaleFactor, scaleFactor);
}
//捕捉点
void MyView::catchPt(QPointF pt)
{
    //场景坐标转全局坐标：先转为视图坐标，再转为全局坐标
    QPointF f= mapFromScene(pt);
    f = mapToGlobal(f.toPoint());
    QCursor::setPos(f.toPoint());
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
        dlg = new PosDialog(this);
        if(dlg->exec() != QDialog::Accepted)    return;
        QList<QPointF> list = dlg->getLine();
        if(list.size()!=2)  return;
        m_scene->addLine(QLineF(list.at(0), list.at(1)) );
    }
}

void MyView::setPt()
{
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
        dlg->showPt();
        if(dlg->exec() != QDialog::Accepted)    return;
        QPointF pt1 = dlg->getPt();
        m_scene->addEllipse(pt1.x()-pt_size, pt1.y()-pt_size, 2*pt_size, 2*pt_size,
                            QPen(QColor(Qt::white)), QBrush(Qt::white,Qt::SolidPattern));
    }
}
