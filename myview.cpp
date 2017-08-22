#include "myview.h"
#include <QtMath>
#include <QMenu>
#include <crosspt.h>
#include <QMessageBox>
#include <QGraphicsEllipseItem>
#include <QGuiApplication>

MyView::MyView(QWidget *parent):
    QGraphicsView(parent)   // 初始化
{
    mode = NORMAL;
    drawPt = false;
    drawLine = false;
    drawRect = false;
    drawElli = false;

    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    QRect viewport_rect(0, 0, this->viewport()->width(),
                        this->viewport()->height() );
    QRectF visible_scene_rect = this->mapToScene(viewport_rect).boundingRect();
    viewCenter = new QGraphicsRectItem(visible_scene_rect);

    m_scene = new MyScene(0);
    this->setScene(m_scene);
    this->scene()->addItem(viewCenter); //this->scene()是 QGraphicsScene*
    this->viewport()->update();

    connect(this,SIGNAL(customContextMenuRequested(const QPoint&)), this,
            SLOT(ShowContextMenu()) );
}

MyView::~MyView()
{

}

MyScene *MyView::getScene() const
{
    return m_scene;
}

void MyView::mousePressEvent(QMouseEvent *event)
{
    switch(event->button())
    {
    case Qt::MidButton:
        mode = DRAG;
        dragStart = this->mapToScene(event->pos());
//        qDebug()<<"origin:"<<dragStart;
        changeCursor(Qt::ClosedHandCursor);
        event->accept();
        break;
    case Qt::LeftButton:
        // 窗口坐标转为场景坐标
        start = this->mapToScene(event->pos());
        if(!drawLine && !drawPt && !drawRect && !drawElli)
        {
            mode = NORMAL;

        }
        else    mode =EDIT;
        qDebug()<<"mode: "<<mode<<"drawPt"<<drawPt;
//        if(drawLine && !drawLineXY &&!drawLineAH)
        if(0)
        {
        }
        else if(drawPt && drawCirPt)    // 画圆点,start为圆心,pt_size为半径
        {
            m_scene->addEllipse(start.x()-pt_size, start.y()-pt_size, 2*pt_size, 2*pt_size,
                                QPen(QColor(Qt::white)),
                                QBrush(Qt::white,Qt::SolidPattern) )->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
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
    case NORMAL:
         break;  // 处理QGraphicsView::mouseMoveEvent
    case DRAG:
    {
        // Calculate the offset to drag relative to scene coordinates.
        dragEnd = this->mapToScene(event->pos());
//        qDebug()<<"end:"<<dragEnd;
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
//    没有mouseMoveEvent时，能实现橡胶手模式，因为mode=NORMAL,鼠标事件实际是父类的moveEvent
    QGraphicsView::mouseMoveEvent(event);
}

void MyView::mouseReleaseEvent(QMouseEvent *event)
{
    switch(event->button())
    {
    case Qt::MidButton:
        mode = NORMAL;
        changeCursor(Qt::ArrowCursor);
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
        this->Delete();
        break;
    case Qt::Key_C:
        this->Copy();
        break;
    case Qt::Key_V:
        this->Paste();
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
        m_scene->addLine(QLineF(list.at(0), list.at(1)),
                         QPen(QColor(Qt::white)))->
                setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
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
        m_scene->addLine(line, QPen(QColor(Qt::white)))->
                setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    }
}

void MyView::setPt()
{
    drawLine=false;
    drawPt=true;
    drawRect=false;
    drawElli=false;
    changeCursor("cross");
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
                            QBrush(Qt::white,Qt::SolidPattern))->
                setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    }
}

void MyView::setRect()
{
    drawLine=false;
    drawPt=false;
    drawRect=true;
    drawElli=false;
    changeCursor("cross");
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
                value[0],value[1],QPen(QColor(Qt::white)) )->
                setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
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
    changeCursor("cross");
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
                value[0], value[1], QPen(QColor(Qt::white)))->
                setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
        delete[] value;
    }
}

void MyView::ShowContextMenu()
{
    QMenu m;
    QAction *Normal = m.addAction("重置为普通模式");
    QAction *Locate = m.addAction("定位到原点");
    QAction *Reset = m.addAction("重置视图");
    QAction *Delete = m.addAction("删除");
    QAction *Redraw = m.addAction("清空重画");
    QAction *Paste = m.addAction("黏贴");

    Normal->setIcon(QIcon(":/Icon/Icon/normal.png"));
    Locate->setIcon(QIcon(":/Icon/Icon/locate.png"));
    Reset->setIcon(QIcon(":/Icon/Icon/reset.png"));
    Delete->setIcon(QIcon(":/Icon/Icon/delete.png"));
    Redraw->setIcon(QIcon(":/Icon/Icon/redraw.png"));

    connect(Normal,SIGNAL(triggered(bool)), this, SLOT(setNormal()) );
    connect(Locate,SIGNAL(triggered(bool)), this, SLOT(Locate()) );
    connect(Reset,SIGNAL(triggered(bool)), this,  SLOT(Reset()) );
    connect(Delete,SIGNAL(triggered(bool)), this, SLOT(Delete()) );
    connect(Redraw,SIGNAL(triggered(bool)), this, SLOT(Redraw()) );
    connect(Paste,SIGNAL(triggered(bool)), this,  SLOT(Paste()) );
    foreach(QGraphicsItem *item, m_scene->selectedItems())
    {
        item->setFocus();
        item->setSelected(true);
    }
    m.exec(QCursor::pos());
}

void MyView::setNormal()
{
    mode = NORMAL;
    drawPt=false;
    drawLine=false;
    drawElli = false;
    drawRect = false;
    changeCursor(Qt::ArrowCursor);
}

void MyView::Locate()
{
    this->centerOn(0,0);
    this->scale(1, 1);
    this->updateCenterRect();
}

void MyView::Reset()
{
    this->resetMatrix();
    this->scale(1,1);
}

void MyView::Copy()
{
    QByteArray ba;
    QDataStream s(&ba,QIODevice::WriteOnly);
    s<<m_scene->selectedItems().size();
    foreach(QGraphicsItem *item, m_scene->selectedItems())
    {
//        s<<item->rect();
//        s<<item->pos();

//        s<<item->rotation();
//        s<<item->transformOriginPoint();
//        s<<item->scale();
//        s<<item->scenePos();
//        s<<item->boundingRect();
        QGraphicsEllipseItem *e = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
        s<<e->rect();
        qDebug()<<"copy:"<<e->rect();
    }

    QMimeData * md = new QMimeData();
    md->setData("QGraphicsItem",ba);
//  Qt5.9 MinGW编译器不识别此静态函数和qApp->clipboard(), bug?
    QClipboard *cb = QApplication::clipboard();
    cb->setMimeData(md);

}

void MyView::Paste()
{
    QClipboard *cb = QApplication::clipboard();
    const QMimeData * md = cb->mimeData();
    QByteArray ba = md->data("QGraphicsItem");
    if(ba.isEmpty()){
        qDebug()<<"没有复制图元数据";
        return;
    }
    m_scene->clearSelection();
    QDataStream s(ba);
    int count;
    s>>count;
    int type;

    QPointF pos,sceneP;
    QRectF rec;
    for(int i=0;i<count;i++)
    {
        s>>rec;
//        s>>pos;
//        s>>sceneP;
//        s>>rec;
    }
    qDebug()<<"paste"<<rec;
    QGraphicsEllipseItem *test = new QGraphicsEllipseItem();
    m_scene->addEllipse(rec,QPen(QColor(Qt::yellow)),QBrush(QColor(Qt::yellow)))->setPos(-80,80);
    m_scene->addLine(QLineF(QPointF(0,0), QPointF(20,30)));
}

void MyView::Delete()
{
//   不是this->scene(),它是QGraphicsScene.  为什么用selectedItems()不行?
    chosenItems = m_scene->selectedItems();
    if(!chosenItems.size())   return;
    foreach(QGraphicsItem* item, chosenItems)
    {
        if(!item)
        {
            QMessageBox::warning(0,"删除失败","图元不存在或不完整");
            return;
        }
        m_scene->removeItem(item);  //删除item及其子item
    }
}

void MyView::Redraw()
{
    m_scene->clear();
    m_scene->InitScene();
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

void MyView::changeCursor(const QString& shape)
{
    QPixmap p;
    p.load(":/Shape/Shape/"+shape+".png");
    this->viewport()->setCursor(p);
}

void MyView::changeCursor(Qt::CursorShape shape)
{
    QCursor c;
    c.setShape(shape);
    this->viewport()->setCursor(c);
}

void MyView::test()
{
    qDebug()<<"test:"<<qrand()%80;
}
