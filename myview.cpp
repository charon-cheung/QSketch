#include "myview.h"
#include <QDebug>
#include <QtMath>
#include <QMenu>
#include <crosspt.h>
#include <QApplication>
#include <QDateTime>
#include <QMessageBox>

MyView::MyView(QWidget *parent):
    QGraphicsView(parent)   // 初始化
{
//    this->setDragMode(QGraphicsView::ScrollHandDrag);
    mode = NORMAL;
    drawPt = false;
    drawLine = false;
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    m_scene = new MyScene(0);
    this->setScene(m_scene);

    QRect viewport_rect(0, 0, this->viewport()->width(),
                        this->viewport()->height() );
    QRectF visible_scene_rect = this->mapToScene(viewport_rect).boundingRect();
    viewCenter = new QGraphicsRectItem(visible_scene_rect);
    this->scene()->addItem(viewCenter); //this->scene()是 QGraphicsScene*

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,SIGNAL(customContextMenuRequested(const QPoint&)), this,
            SLOT(ShowContextMenu()) );

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
//        if(drawLine && !drawLineXY &&!drawLineAH)
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
            CrossPt *pt = new CrossPt();
            pt->setRect(QRect(-10, -10, 20, 20));
            pt->setPos(start);
            m_scene->addItem(pt);
         }
         else if(drawLineXY || drawLineAH || drawRectXY || drawElliXY)
         {
             mode = NORMAL;
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
        qDebug()<<event->delta();
        this->scale(scaleFactor, scaleFactor);
        this->updateCenterRect();
    }
    QGraphicsView::wheelEvent(event);
}

void MyView::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->setNormal();
        break;
    case Qt::Key_Home:
        this->Locate();
        break;
    case Qt::Key_Plus:
        this->scale(1.41421, 1.41421);
        this->updateCenterRect();
        break;
    case Qt::Key_Minus:
        this->scale(0.70711, 0.70711);
        this->updateCenterRect();
        break;
    case Qt::Key_Delete:
        m_scene->Delete();
        break;
    default:
        event->ignore();
    }
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
    drawRect=false;
    drawElli=false;
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
        if(list.at(0)==list.at(1))
        {
            QMessageBox::warning(0,"出错了","两个点的坐标不能相同!");
            return;
        }
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

        QLineF line;
        line.setP1(pt);
        line.setAngle(360-angle);
        line.setLength(length);
        m_scene->addLine(line, QPen(QColor(Qt::white)) );
    }
}

void MyView::setPt()
{
    drawLine=false;
    drawPt=true;
    drawRect=false;
    drawElli=false;
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

void MyView::setRect()
{
    drawLine=false;
    drawPt=false;
    drawRect=true;
    drawElli=false;
    if(sender()->objectName()=="actRect_1")
    {
        drawRectXY=false;
        drawRounded=false;
    }
    else if(sender()->objectName()=="actRect_2")
    {
        drawRectXY=true;
        drawRounded=false;
        dlg = new PosDialog(this);
        dlg->showRectXY();
        if(dlg->exec() != QDialog::Accepted)    return;

        QPointF pt= dlg->getPt();
        float *value= dlg->getWH();
//        因为视图对y轴镜像，直接绘图pt是矩形的左下顶点，需要变换
        m_scene->addRect(pt.x(),pt.y()-value[1],
                value[0],value[1],QPen(QColor(Qt::white)));
        delete[] value;
    }
    else if(sender()->objectName()=="actRect_3")
    {
        drawRectXY=false;
        drawRounded=true;

    }
}

void MyView::setEllipse()
{
    drawLine=false;
    drawPt=false;
    drawRect=false;
    drawElli=true;
    if(sender()->objectName()=="actEllipse_1")
    {
        drawElliXY=false;
    }
    else if(sender()->objectName()=="actEllipse_2")
    {
        drawElliXY=true;
        dlg = new PosDialog(this);
        dlg->showEllipse();
        if(dlg->exec() != QDialog::Accepted)    return;
        QPointF pt= dlg->getPt();
        float *value= dlg->getWH();
//        因为视图对y轴镜像，所以pt是矩形的左下顶点,需要变换
        m_scene->addEllipse(pt.x()-value[0]/2, pt.y()-value[1]/2,
                value[0], value[1], QPen(QColor(Qt::white)));
        delete[] value;
    }
}

void MyView::ShowContextMenu()
{
    QMenu m;
    QAction *Normal = m.addAction("重置为普通模式");
    QAction *Locate = m.addAction("定位到原点");
    QAction *Measure = m.addAction("标注");
    QAction *Delete = m.addAction("删除");
    QAction *SaveImage = m.addAction("保存为图片");

    Normal->setIcon(QIcon(":/Icon/Icon/normal.png"));
    Locate->setIcon(QIcon(":/Icon/Icon/locate.png"));
    Measure->setIcon(QIcon(":/Icon/Icon/measure.png"));
    Delete->setIcon(QIcon(":/Icon/Icon/delete.png"));
    SaveImage->setIcon(QIcon(":/Icon/Icon/save.png"));

    connect(Normal,SIGNAL(triggered(bool)), this, SLOT(setNormal()) );
    connect(Locate,SIGNAL(triggered(bool)), this, SLOT(Locate()) );
    connect(Measure,SIGNAL(triggered(bool)), this, SLOT(setMeasure()) );

//    删除失败，报错item 0x1fc270a8's scene (0x0) is different from this scene,
//    但键盘m_scene->Delete()成功
    connect(Delete, &QAction::triggered,  m_scene, &MyScene::Delete);
//    connect(SaveImage, &QAction::triggered, m_scene, &MyScene::SaveImage);
//    这种方式报错：No such slot QGraphicsScene::SaveImage()
    connect(SaveImage,SIGNAL(triggered(bool)), m_scene, SLOT(SaveImage()) );
    m.exec(QCursor::pos());
}

void MyView::setNormal()
{
    mode = NORMAL;
    drawPt=false;
    drawLine=false;
}

void MyView::Locate()
{
    this->centerOn(0,0);
    this->scale(1, 1);
    this->updateCenterRect();
}

void MyView::setMeasure()
{

}

void MyView::BreakDown()
{

}

void MyView::Chamfer()
{

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
