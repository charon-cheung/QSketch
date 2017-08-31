#include "myview.h"
#include <QtMath>
#include <QMenu>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>
#include <QStatusBar>
#include <QInputDialog>
MyView::MyView(QWidget *parent):
    QGraphicsView(parent)   // 初始化
{
    InitView();
    InitViewRect();
    m_scene = new MyScene(0);   //注意this->scene()是 QGraphicsScene*
    this->setScene(m_scene);

//    Cmd = new Command(m_scene);
    m_main = qobject_cast<MainWindow*>(this->topLevelWidget());
    connect(this,SIGNAL(customContextMenuRequested(const QPoint&)), this,
            SLOT(ShowContextMenu()) );
}

MyView::~MyView()
{

}

MyScene* MyView::getScene()
{
    return m_scene;
}

void MyView::mousePressEvent(QMouseEvent *event)
{
    switch(event->button())
    {
    case Qt::MidButton:
        mode = DRAG;
        dragBegin = this->mapToScene(event->pos());
        changeCursor("movable");
        showStatus("当前为平移模式");
        event->accept();
        break;
    case Qt::LeftButton:
        // 窗口坐标转为场景坐标
        start = this->mapToScene(event->pos());
        if(!drawLine && !drawPt && !drawRect && !drawElli && !drawText)
        {
            mode = NORMAL;
            showStatus("当前为普通模式");
        }
        else
        {
            mode =EDIT;
            showStatus("当前为编辑模式");
            MyScene * press_scene = this->getScene();
//            if(drawLine && !drawLineXY &&!drawLineAH
            if(0)
            {
            }
            else if(drawPt && drawCirPt)    // 画圆点,start为圆心,pt_size为半径
            {
                CirclePt *pt = new CirclePt();
                pt->setPos(start);  //这里不是图元坐标，是场景坐标
                press_scene->addItem(pt);
            }
            else if(drawPt && drawCross)    // 画X样式的点
            {
                CrossPt *pt = new CrossPt();
                pt->setPos(start);
                press_scene->addItem(pt);
            }
            else if(drawElli && drawRing)    // 画圆环
            {
                Ring *r = new Ring();
                r->setPos(start);
                press_scene->addItem(r);
            }
            else if(drawText)
            {
                Text = press_scene->addSimpleText((drawMulti ? inputText(true): inputText(false) ), TextFont);
                Text->setPos(start);
                Text->setTransform(QTransform::fromScale(1,-1));
                Text->setBrush(QBrush(Qt::darkCyan,Qt::SolidPattern));
            }
//            else if(drawLineXY || drawLineAH || drawRectXY || drawElliXY ||drawRing)
//            {
//                mode = NORMAL;
//            }
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
    {
        break;  // 需要处理QGraphicsView::mouseMoveEvent
    }
    case DRAG:
    {
        // Calculate the offset to drag relative to scene coordinates.
        dragEnd = this->mapToScene(event->pos());
        dragTrans = dragBegin - dragEnd;
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
        showStatus("当前为普通模式");
        break;
    case Qt::LeftButton:
        if(mode==EDIT)
        {
//            end = this->mapToScene(event->pos());
//            if(drawLine && !drawLineXY)
//            {
//                Line = m_scene->addLine(QLineF(start,end),QPen(QColor(Qt::white)));
//            }
//            mode = NORMAL;
        }
    default:
        event->ignore();
        break;
    }
    updateCenterRect();
    QGraphicsView::mouseReleaseEvent(event);
}
//视图放大和缩小,可跟随鼠标
void MyView::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier)
    {
        qreal scaleFactor = qPow(2.0, event->delta() / 240.0);  // ???
//        qDebug()<<event->delta();     为正负120
        this->scale(scaleFactor, scaleFactor);
        this->updateCenterRect();
    }
    else
        QGraphicsView::wheelEvent(event);
}

void MyView::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier && event->key()==Qt::Key_C)
        this->Copy();
    if(event->modifiers() == Qt::ControlModifier && event->key()==Qt::Key_X)
        this->Cut();
    else if(event->modifiers() == Qt::ControlModifier && event->key()==Qt::Key_V)
        this->Paste();
    else if(event->modifiers() == Qt::ControlModifier && event->key()==Qt::Key_A)
    {
        this->selectAll(true);
        showStatus("已经选择所有的图元");
    }

    switch(event->key())
    {
    case Qt::Key_Escape:
        this->setNormal();
        this->selectAll(false);
        break;
    case Qt::Key_Home:
    {
        this->Locate();
        this->Reset();
        showStatus("重置");
    }
        break;
    case Qt::Key_Plus:
        this->scale(1.41421, 1.41421);
        this->updateCenterRect();
        break;
    case Qt::Key_Minus:
        this->scale(0.70711, 0.70711);
        this->updateCenterRect();
        break;
    case Qt::Key_M :
        if(m_movable)
            this->SetMovable(false);
        else
            this->SetMovable(true);
        break;
    case Qt::Key_R :
        this->showItemInfo();
        break;
    case Qt::Key_Delete:
        this->Delete();
        break;
    case Qt::Key_Up:
    {
        this->Translate(Command::UP);
        showStatus("向上平移5个单位");
    }
        break;
    case Qt::Key_Down:
    {
        this->Translate(Command::DOWN);
        showStatus("向下平移5个单位");
    }
        break;
    case Qt::Key_Left:
    {
        this->Translate(Command::LEFT);
        showStatus("向左平移5个单位");
    }
        break;
    case Qt::Key_Right:
    {
        this->Translate(Command::RIGHT);
        showStatus("向右平移5个单位");
    }
        break;
    default:
        event->ignore();
    }
}

//捕捉点,鼠标定位到某个场景点对应的全局坐标
void MyView::catchPt(QPointF pt)
{
    //场景坐标转全局坐标：先转为视图坐标，再转为全局坐标
    QPointF f= mapFromScene(pt);
    f = mapToGlobal(f.toPoint());
    QCursor::setPos(f.toPoint());
}

void MyView::showItemInfo()
{
    Cmd = new Command(this);
    Cmd->ShowItemInfo();
}

void MyView::DrawPt()
{
    drawPt = true;
    drawLine = false;
    drawRect = false;
    drawElli = false;
    drawText = false;
    if(sender()->objectName() == "act1")
    {
        drawCirPt=true;
        drawCross=false;
        drawPtXY=false;
        changeCursor("cross");
        showStatus("当前为编辑模式");
    }
    else if(sender()->objectName() == "act2")
    {
        drawCirPt=false;
        drawCross=true;
        drawPtXY=false;
        changeCursor("cross");
        showStatus("当前为编辑模式");
    }
    else if(sender()->objectName() == "act3")
    {
        changeCursor(Qt::ArrowCursor);
        drawCirPt=false;
        drawCross=false;
        drawPtXY=true;
        dlg = new PosDialog(this);
        dlg->showPt();
        if(dlg->exec() != QDialog::Accepted)    return;
        QPointF pt1 = dlg->getPt();

        CirclePt *pt = new CirclePt();
        pt->setPos(pt1);
        m_scene->addItem(pt);
    }
}

void MyView::DrawLine()
{
    drawPt = false;
    drawLine = true;
    drawRect = false;
    drawElli = false;
    drawText = false;
    if(sender()->objectName() == "actLine_1")
    {
        drawLineXY=false;
        drawLineAH = false;
        changeCursor("cross");
        showStatus("当前为编辑模式");
    }
    else if(sender()->objectName() == "actLine_2")
    {
        changeCursor(Qt::ArrowCursor);
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

        qDebug()<<this->getPenWidth();
        m_scene->addLine(QLineF(list.at(0), list.at(1)),
                       getPen())->
                setFlag(QGraphicsItem::ItemIsSelectable);
    }
    else if(sender()->objectName() == "actLine_3")
    {
        changeCursor(Qt::ArrowCursor);
        drawLineXY = false;
        drawLineAH = true;
        dlg = new PosDialog(this);
        dlg->showLineAH();
        if(dlg->exec() != QDialog::Accepted)    return;

        QPointF pt1 = dlg->getPt();
        float angle = dlg->getAngle();
        float length = dlg->getLength();
        if(pt1==QPointF(0,0) || length==0)
        {
            QMessageBox::warning(0,"出错了","起点的坐标不能为(0,0) !");
            return;
        }
        QLineF line;
        line.setP1(pt1);    //不能为(0,0),否则直线无效，为什么?
        line.setAngle(360-angle);
        line.setLength(length);
        if(line.isNull())
        {
            QMessageBox::warning(0,"出错了","直线无效!");
            return;
        }
        m_scene->addLine(line, getPen())->
                setFlag(QGraphicsItem::ItemIsSelectable);
    }
}

void MyView::DrawRect()
{
    drawPt = false;
    drawLine = false;
    drawRect = true;
    drawElli = false;
    drawText = false;
    if(sender()->objectName()=="actRect_1")
    {
        drawRectXY=false;
        drawRounded=false;
        changeCursor("cross");
        showStatus("当前为编辑模式");
    }
    else if(sender()->objectName()=="actRect_2")
    {
        changeCursor(Qt::ArrowCursor);
        drawRectXY=true;
        drawRounded=false;
        dlg = new PosDialog(this);
        dlg->showRectXY();
        if(dlg->exec() != QDialog::Accepted)    return;

        QPointF pt= dlg->getPt();
        float *value= dlg->getWH();
//        因为视图对y轴镜像，直接绘图pt是矩形的左下顶点，需要变换
        m_scene->addRect(pt.x(),pt.y()-value[1],
                value[0],value[1], getPen(),getBrush())->
                setFlag(QGraphicsItem::ItemIsSelectable);
        delete[] value;
    }
    else if(sender()->objectName()=="actRect_3")
    {
        drawRectXY=false;
        drawRounded=true;

    }
}

void MyView::DrawEllipse()
{
    drawPt = false;
    drawLine = false;
    drawRect = false;
    drawElli = true;
    drawText = false;
    if(sender()->objectName()=="actEllipse_1")
    {
        drawElliXY=false;
        drawRing=false;
        changeCursor("cross");
        showStatus("当前为编辑模式");
    }
    else if(sender()->objectName()=="actEllipse_2")
    {
        changeCursor(Qt::ArrowCursor);
        drawElliXY=true;
        drawRing=false;
        dlg = new PosDialog(this);
        dlg->showEllipse();
        if(dlg->exec() != QDialog::Accepted)    return;
        QPointF pt= dlg->getPt();
        float *value= dlg->getWH();
//        因为视图对y轴镜像，所以pt是矩形的左下顶点,需要变换
        m_scene->addEllipse(pt.x()-value[0]/2, pt.y()-value[1]/2,
                value[0], value[1], getPen(), getBrush())->
                setFlag(QGraphicsItem::ItemIsSelectable);
        delete[] value;
    }
    else if(sender()->objectName()=="ring")
    {
        drawElliXY=false;
        drawRing=true;
        changeCursor("cross");
        showStatus("当前为编辑模式");
    }
}

void MyView::DrawText()
{
    drawPt = false;
    drawLine = false;
    drawRect = false;
    drawElli = false;
    drawText = true;
    changeCursor("cross");
    showStatus("当前为编辑模式");
    if(sender()->objectName()=="textAct")
    {
        drawSingle = true;
        drawMulti = false;
    }
    else if(sender()->objectName()=="multiTextAct")
    {
        drawSingle = false;
        drawMulti = true;
    }
}

void MyView::ShowContextMenu()
{
    QMenu m;
    QAction *Normal = m.addAction("重置为普通模式");
    QAction *Locate = m.addAction("定位到原点");
    QAction *Reset = m.addAction("重置视图");
    QAction *Movable = m.addAction("设置为可动");
    QAction *Delete = m.addAction("删除");

    QAction *Redraw = m.addAction("清空重画");
    QAction *Cut = m.addAction("剪切");
    QAction *Copy = m.addAction("复制");
    QAction *Paste = m.addAction("黏贴");
    QAction* Info = m.addAction("属性");

    Normal->setIcon(QIcon(":/Icon/Icon/normal.png"));
    Locate->setIcon(QIcon(":/Icon/Icon/locate.png"));
    Reset->setIcon(QIcon(":/Icon/Icon/reset.png"));
    Movable->setIcon(QIcon(":/Shape/Shape/movable.png"));
    Delete->setIcon(QIcon(":/Icon/Icon/delete.png"));

    Redraw->setIcon(QIcon(":/Icon/Icon/redraw.png"));
    Cut->setIcon(QIcon(":/Icon/Icon/cut.png"));
    Copy->setIcon(QIcon(":/Icon/Icon/copy.png"));
    Paste->setIcon(QIcon(":/Icon/Icon/paste.png"));
    Info->setIcon(QIcon(":/Icon/Icon/info.png"));
    Cmd = new Command(m_scene);
    connect(Normal,SIGNAL(triggered(bool)), this, SLOT(setNormal()) );
    connect(Locate,SIGNAL(triggered(bool)), this, SLOT(Locate()) );
    connect(Reset,SIGNAL(triggered(bool)), this,  SLOT(Reset()) );
    connect(Movable,SIGNAL(triggered(bool)), this,  SLOT(SetMovable(bool)) );
    connect(Delete,SIGNAL(triggered(bool)), this, SLOT(Delete()) );
//    connect(Delete,SIGNAL(triggered(bool)), Cmd, SLOT(Delete()) );

    connect(Cut,SIGNAL(triggered(bool)), this,  SLOT(Cut()) );
    connect(Copy,SIGNAL(triggered(bool)), this,  SLOT(Copy()) );
    connect(Paste,SIGNAL(triggered(bool)), this,  SLOT(Paste()) );
    connect(Redraw,SIGNAL(triggered(bool)), this, SLOT(Redraw()) );
    connect(Info,SIGNAL(triggered(bool)), this, SLOT(showItemInfo()) );

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
    showStatus("切换为普通模式");
}

void MyView::Locate()
{
    this->centerOn(0,0);
    this->scale(1, 1);
    this->updateCenterRect();
    catchPt(QPointF(0,0));
    showStatus("鼠标切换至原点");
}

void MyView::Reset()
{
    this->resetMatrix();
    this->scale(2,-2);
    this->centerOn(0,0);
    this->updateCenterRect();
}

void MyView::SetMovable(bool state)
{
    Cmd = new Command(this);
    Cmd->SetMovable(state);
}

void MyView::Cut()
{
    Copy();
    Delete();
}

void MyView::Copy()
{
    QByteArray ba;
    QDataStream s(&ba,QIODevice::WriteOnly);
    int count = m_scene->selectedItems().size();
    if(count==0)    return;
    s<< count;
    this->getScene()->Export(s,m_scene->selectedItems());

    QMimeData * md = new QMimeData();
    md->setData("GraphicsItem",ba);
    QClipboard *cb = QApplication::clipboard();
    cb->setMimeData(md);
    m_copied = true;
}

void MyView::Paste()
{
    QClipboard *cb = QApplication::clipboard();
    const QMimeData * md = cb->mimeData();
    QByteArray ba = md->data("GraphicsItem");
    if(ba.isEmpty()){
        qDebug()<<"没有复制图元数据";
        return;
    }
    m_scene->clearSelection();
//    鼠标的当前场景坐标,实际是文字“粘贴”的位置,所以用键盘的效果更好
    QPointF pos = this->getScenePos();

    QDataStream s(ba);
    int count;
    s>>count;
    for(int i=0;i<=count;i++)
    {
        QString className;
        s>>className;   //不能直接用字符串
        if(className=="QGraphicsEllipseItem"||className=="QGraphicsRectItem"
                || className=="CrossPt"|| className=="CirclePt")
        {
            qreal x,y,w,h;
            s >> x;
            s >> y;
            s >> w;
            s >> h;
            if(className=="QGraphicsEllipseItem")
            {
                this->getScene()->addEllipse(pos.x(),pos.y(),w,h,QPen(QColor(Qt::white)) );
            }
            else if(className=="QGraphicsRectItem")
                this->getScene()->addRect(pos.x(),pos.y(),w,h,QPen(QColor(Qt::white)));
            else if(className=="CrossPt")
            {
                CrossPt *pt = new CrossPt();
//                pt->setBoundingRect(QRect(x,y,w,h));
                pt->setPos(pos);
                this->getScene()->addItem(pt);
            }
            else if(className=="CirclePt")
            {
                CirclePt *pt = new CirclePt();
//                pt->setBoundingRect(QRect(x,y,w,h));
                pt->setPos(pos);
                this->getScene()->addItem(pt);
            }
        }
        else if(className=="QGraphicsLineItem")
        {
            qreal x1,y1,x2,y2;
            s>>x1;
            s>>y1;
            s>>x2;
            s>>y2;
            QGraphicsLineItem *Line = this->getScene()->addLine(x1,y1,x2,y2,QPen(QColor(Qt::white)) );
            QTransform t;
//            复制的是直线的两个端点，只能平移
            t.translate(pos.x(),pos.y());
            Line->setTransform(t);
//            Line->setFlag(QGraphicsItem::ItemIsSelectable);
        }
    }
}

void MyView::Delete()
{
    Cmd = new Command(m_scene);
    Cmd->Delete();
}

void MyView::Rotate(QPointF pt, float angle)
{
    Cmd = new Command(m_scene);
    Cmd->Rotate(pt, angle);
}

void MyView::Redraw()
{
    m_scene->clear();
    m_scene->InitScene();
    showStatus("已经清空画面");
}

void MyView::Translate(int direction)
{
    Cmd = new Command(m_scene);
    Cmd->Translate(direction);
}

void MyView::Translate(QPointF pt)
{
    Cmd = new Command(m_scene);
    Cmd->Translate(pt);
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

//将鼠标的全局坐标转为场景坐标
QPointF MyView::getScenePos()
{
    QCursor c;
    QPoint cursorPos = c.pos();
    QPoint viewPos = this->mapFromGlobal(cursorPos);
    QPointF scenePos = this->mapToScene(viewPos);
    return scenePos;
//    qDebug()<<cursorPos<<viewPos<<scenePos;
}

void MyView::selectAll(bool state)
{
    Cmd = new Command(m_scene);
    Cmd->SelectAll(state);
}
// 可以与之前的方式做对比
void MyView::showStatus(QString msg)
{
    m_main->statusBar()->showMessage(msg);
}

QString MyView::inputText(bool multi)
{
    bool ok;
    QString text;
    if(multi)
        text = QInputDialog::getMultiLineText(0, tr("请输入文本"),"",
                                              "第一行\n第二行", &ok);
    else
        text = QInputDialog::getText(0, tr("请输入文本"),"",
                                     QLineEdit::Normal,"text",&ok);
    if (ok && !text.isEmpty())
        ok = true;
        return text;
}

void MyView::test()
{
    qDebug()<<"test:"<<qrand()%80;
}

void MyView::InitView()
{
    mode = NORMAL;
    InitBools();
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    //锚点以鼠标为准,放缩时效果跟网络地图一样
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//    qDebug()<<this->matrix();   //默认是单位矩阵
//    QMatrix m(10,0,0,10,0,0);
    //    this->setMatrix(m);
}

void MyView::InitBools()
{
    drawPt = false;
    drawLine = false;
    drawRect = false;
    drawElli = false;
    drawText = false;
    m_copied = false;
    m_movable = false;
    m_saved = false;
    m_new = false;
}

void MyView::InitViewRect()
{
    //随着鼠标点击，场景总出现几个矩形，暂时去掉
    QRect viewport_rect(0, 0, this->viewport()->width(),
                        this->viewport()->height() );       //98 X 28
    QRectF visible_scene_rect = this->mapToScene(viewport_rect).boundingRect();
    viewCenter = new QGraphicsRectItem(visible_scene_rect);
//    m_scene->addItem(viewCenter);
    this->viewport()->update();
}

int MyView::getPenWidth()
{
    PenWidth = m_main->getPenWidth();
    return PenWidth;
}

Qt::PenStyle MyView::getPenStyle()
{
    PenStyle = m_main->getPenStyle();
    return PenStyle;
}
//没在mainwindow里选,则PenColor是invalid
QColor MyView::getColor()
{
    PenColor = m_main->getPenColor();
    if(!PenColor.isValid())
        PenColor = Qt::white;
    return PenColor;
}
//没在mainwindow里选,则没有brush
QBrush MyView::getBrush()
{
    PenBrush = m_main->getPenBrush();
    return PenBrush;
}

QPen MyView::getPen()
{
    //函数也能找到sender()
//    qDebug()<<"sender name:"<<sender()->objectName();
    pen.setWidth(getPenWidth());
    pen.setStyle(getPenStyle());
    pen.setColor(getColor());
    if(getBrush().style()!=Qt::NoBrush)
        pen.setBrush(getBrush());
    return pen;
}

QFont MyView::getFont()
{
    MainWindow* m = qobject_cast<MainWindow*>(sender());
    TextFont = m->getFont();
    return TextFont;
}

void MyView::setSaved(bool flag)
{
    m_saved = flag;
}

bool MyView::IsSaved()
{
    return m_saved;
}

void MyView::setNew(bool flag)
{
    m_new = flag;
}

bool MyView::IsNew()
{
    return m_new;
}

void MyView::SetMoveFlag(bool flag)
{
    m_movable = flag;
}
