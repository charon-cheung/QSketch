#include "myview.h"
#include <QDebug>
#include <QtMath>
#include <QMenu>

MyView::MyView(QWidget *parent):
    QGraphicsView(parent)   // 初始化
{
//    this->setDragMode(QGraphicsView::ScrollHandDrag);
    mode = NORMAL;
    drawPt = false;
    drawLine = false;

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    m_scene = new MyScene(0);
    this->setScene(m_scene);

    QRect viewport_rect(0, 0, this->viewport()->width(),
                        this->viewport()->height() );
    QRectF visible_scene_rect = this->mapToScene(viewport_rect).boundingRect();
    viewCenter = new QGraphicsRectItem(visible_scene_rect);
    this->scene()->addItem(viewCenter);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,SIGNAL(customContextMenuRequested(const QPoint&)), this,
            SLOT(ShowContextMenu()) );

    line = this->scene()->addLine(QLineF(QPointF(0,0), QPointF(60,60)),QPen(QColor(Qt::blue)));
    line->setFlag(QGraphicsItem::ItemIsSelectable);
//    line->setSelected(true);
}

MyView::~MyView()
{

}

void MyView::mousePressEvent(QMouseEvent *event)
{
    switch(event->button())
    {
    case Qt::MidButton:
        mode = DRAG;
        dragStart = this->mapToScene(event->pos());
        qDebug()<<"origin:"<<dragStart;
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        break;
    case Qt::LeftButton:
        // 窗口坐标转为场景坐标
        start = this->mapToScene(event->pos());
        if(!drawLine && !drawPt)
        {
            mode = NORMAL;
        }
        mode =EDIT;
//        if(drawLine && !drawLineXY)
         if(0)
        {
        }
        else if(drawPt && drawCirPt)    // 画圆点,start为圆心,pt_size为半径
        {
            m_scene->addEllipse(start.x()-pt_size, start.y()-pt_size, 2*pt_size, 2*pt_size,
                                QPen(QColor(Qt::white)),
                                QBrush(Qt::white,Qt::SolidPattern) )->setSelected(true);
        }
        else if(drawPt && drawCross)    // 画X样式的点
        {
            QPointF p1 = start+QPoint(pt_size,pt_size);
            QPointF p2 = start+QPoint(-pt_size,-pt_size);
            QPointF p3 = start+QPoint(-pt_size,pt_size);
            QPointF p4 = start+QPoint(pt_size,-pt_size);
            m_scene->addLine(QLineF(p1, p2), QPen(QColor(Qt::white)))->setSelected(true);
            m_scene->addLine(QLineF(p3, p4), QPen(QColor(Qt::white)))->setSelected(true);
        }
    default:
        event->ignore();
        break;
    }
    QGraphicsView::mousePressEvent(event);
}

void MyView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF dragEnd,dragTrans;
    switch (mode) {
    case DRAG:
    {
        // Calculate the offset to drag relative to scene coordinates.
        dragEnd = this->mapToScene(event->pos());
        qDebug()<<"end:"<<dragEnd;
        dragTrans = dragStart - dragEnd;
        viewCenter->moveBy(dragTrans.x(), dragTrans.y());
        this->centerOn(viewCenter);
        event->accept();
        break;
    }
    case EDIT:
    {
        if(mode!=EDIT)  return;
        //        qDebug()<<"mode:"<<mode;
        //        end = this->mapToScene(event->pos());
        //        qDebug()<<"end:"<<end;
        //        m_scene->addLine(QLineF(start,end) );
        //        start = this->mapToScene(event->pos());

        //        qreal angle1= qAtan2( start.y(),start.x());
        //        qreal angle2= qAtan2( end.y(), end.x());
        //        Line->setRotation((angle2 - angle1)*180 / PI);
        break;
    }
    default:
        event->ignore();
        break;
    }
}

void MyView::mouseReleaseEvent(QMouseEvent *event)
{
    switch(event->button())
    {
    case Qt::MidButton:
        mode = NORMAL;
        setCursor(Qt::ArrowCursor);
        event->accept();
        break;
    case Qt::LeftButton:
        if(mode==EDIT)
        {
            end = this->mapToScene(event->pos());
            if(drawLine && !drawLineXY)
            {
                Line = m_scene->addLine(QLineF(start,end),QPen(QColor(Qt::white)));
            }
            mode = NORMAL;
        }
    default:
        event->ignore();
        break;
    }
    updateCenterRect();

    QGraphicsView::mouseReleaseEvent(event);
}
//视图放大和缩小
void MyView::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier)
    {
        // 加这句后，放大缩小时，原点会跟随鼠标移动
//        this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        qreal scaleFactor = qPow(2.0, event->delta() / 240.0);  // ???
        this->scale(scaleFactor, scaleFactor);
        this->updateCenterRect();
    }
    QGraphicsView::wheelEvent(event);
}
//捕捉点,鼠标定位到某个点对应的全局坐标
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
        drawLineXY=false;
        drawLineAH = false;
    }
    else if(sender()->objectName() == "actLine_2")
    {
        drawLineXY = true;
        drawLineAH = false;
        dlg = new PosDialog(this);
        dlg->showLineXY();
        if(dlg->exec() != QDialog::Accepted)    return;

        QList<QPointF> list = dlg->getLine();
        if(list.size()!=2)  return;
        m_scene->addLine(QLineF(list.at(0), list.at(1)), QPen(QColor(Qt::white)))->setSelected(true);
    }
    else if(sender()->objectName() == "actLine_3")
    {
        drawLineXY = false;
        drawLineAH = true;
        dlg = new PosDialog(this);
        dlg->showLineAH();
        if(dlg->exec() != QDialog::Accepted)    return;

        QPointF pt = dlg->getPt();
        float angle = dlg->getAngle();
        float length = dlg->getLength();
        QPointF pt2(length/qSqrt(2),length/qSqrt(2));

        m_scene->addLine(QLineF(pt, pt2), QPen(QColor(Qt::white)) )->setSelected(true);
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
        drawPtXY=false;
    }
    else if(sender()->objectName() == "act2")
    {
        drawCirPt=false;
        drawCross=true;
        drawPtXY=false;
    }
    else if(sender()->objectName() == "act3")
    {
        drawCirPt=false;
        drawCross=false;
        drawPtXY=true;
        dlg = new PosDialog(this);
        dlg->showPt();
        if(dlg->exec() != QDialog::Accepted)    return;
        QPointF pt1 = dlg->getPt();
        m_scene->addEllipse(pt1.x()-pt_size, pt1.y()-pt_size, 2*pt_size, 2*pt_size,
                            QPen(QColor(Qt::white)),
                            QBrush(Qt::white,Qt::SolidPattern))->setSelected(true);
    }
}

void MyView::ShowContextMenu()
{
    QMenu m;
    QAction *Origin = m.addAction("定位到原点");
    QAction *Dimension = m.addAction("标注");
    QAction *Delete = m.addAction("删除");
    connect(Origin,SIGNAL(triggered(bool)), this, SLOT(reset()) );
    connect(Dimension,SIGNAL(triggered(bool)), this, SLOT(setDimension()) );
    connect(Delete,SIGNAL(triggered(bool)), this, SLOT(setDimension()) );
    m.exec(QCursor::pos());
}

void MyView::reset()
{
    this->centerOn(0,0);
    this->scale(1, 1);
    this->updateCenterRect();
}

void MyView::setDimension()
{
    this->scene()->removeItem(line);
}

void MyView::updateCenterRect()
{
    QRect viewRect = this->viewport()->rect();
    // This setPos is necessary because we translate the rect on the mouseMove event handler
    // If we update the rect, the shape will still be translated, so we have to return
    // it to the centre of the scene.
    this->viewCenter->setPos(0, 0);
    dynamic_cast<QGraphicsRectItem*>(this->viewCenter)->setRect(mapToScene(viewRect).boundingRect());
}

void MyView::test()
{
    qDebug()<<"test:"<<qrand()%80;
}
