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

    Cmd = new Command(m_scene);
    m_main = qobject_cast<MainWindow*>(this->topLevelWidget());
    connect(this,SIGNAL(customContextMenuRequested(const QPoint&)), this,
            SLOT(ShowContextMenu()) );
}

MyView::~MyView()
{
    delete m_scene;
    delete Cmd;
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
        StartPt = this->mapToScene(event->pos());
        if(flag == drawNone)
        {
            mode = NORMAL;
            showStatus("当前为普通模式");
        }
        else
        {
            mode =EDIT;
            showStatus("当前为编辑模式");
            this->setDragMode(QGraphicsView::NoDrag);
            switch (flag)
            {
            case drawCirPt:
            {
                CirclePt *pt1 = new CirclePt();
                pt1->setPos(StartPt);  //这里不是图元坐标，是场景坐标
                m_scene->addItem(pt1);
                break;
            }
            case drawCross:
            {
                CrossPt *pt2 = new CrossPt();
                pt2->setPos(StartPt);
                m_scene->addItem(pt2);
                break;
            }
            case drawLine:
            {
                LineVec.push_back(new QGraphicsLineItem(QLineF(StartPt,StartPt)));
                LineVec.at(LineCount)->setPen(getPen());
                LineVec.at(LineCount)->setFlag(QGraphicsItem::ItemIsSelectable);
                m_scene->addItem(LineVec.at(LineCount));
                break;
            }
            case drawRect:
            {
                RectVec.push_back(new QGraphicsRectItem(QRectF(StartPt,StartPt)));
                RectVec.at(RectCount)->setPen(getPen());
                RectVec.at(RectCount)->setFlag(QGraphicsItem::ItemIsSelectable);
                m_scene->addItem(RectVec.at(RectCount));
                break;
            }
            case drawElli:
            {
                ElliVec.push_back(new QGraphicsEllipseItem(QRectF(StartPt,StartPt)));
                ElliVec.at(ElliCount)->setPen(getPen());
                ElliVec.at(ElliCount)->setFlag(QGraphicsItem::ItemIsSelectable);
                m_scene->addItem(ElliVec.at(ElliCount));
                break;
            }
            case drawRing:
            {
                Ring *r = new Ring();
                r->setPos(StartPt);
                m_scene->addItem(r);
                break;
            }
            case drawText:
            {
                Text = m_scene->addSimpleText(inputMultiText(m_drawMulti), TextFont);
                Text->setFlag(QGraphicsItem::ItemIsSelectable);
                Text->setPos(StartPt);
                Text->setTransform(QTransform::fromScale(1,-1));
//                qDebug()<<Text->transform().m11()<<"  "<<Text->transform().m22();  //对应 1和 -1
                Text->setPen(getPen());
                break;
            }
            default:
                break;
            }
        }
    default:
        event->ignore();
        break;
    }
    QGraphicsView::mousePressEvent(event);
}

void MyView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF dragTrans;
    QPointF EndPt =this->mapToScene(event->pos()); // 不能直接mapToScene(pt).y()
    switch (mode) {
    case NORMAL:
    {
        break;  // 需要处理QGraphicsView::mouseMoveEvent
    }
    case DRAG:
    {
        //Calculate the offset to drag relative to scene coordinates.
        dragTrans = dragBegin - EndPt;
        viewCenter->moveBy(dragTrans.x(), dragTrans.y());
        this->centerOn(viewCenter);
        event->accept();
        break;
    }
    case EDIT:
    {
        if(flag == drawLine)
        {
            QLineF newLine;
            newLine.setP1(StartPt);
            newLine.setP2(EndPt);
            LineVec.at(LineCount)->setLine(newLine);
        }
        else if(flag == drawRect)
        {
            if( (RectVec.at(RectCount)->rect().y() <  EndPt.y() ) &&
                    (RectVec.at(RectCount)->rect().x() < EndPt.x() )  )
            {
                QRectF newRect;
                newRect.setTopLeft(StartPt);
                newRect.setBottomRight(EndPt);
                RectVec.at(RectCount)->setRect(newRect);
            }
            else if((RectVec.at(RectCount)->rect().top() > EndPt.y() )&&
                    (RectVec.at(RectCount)->rect().left() < EndPt.x() ))
            {
                QRectF newRect;
                newRect.setBottomLeft(StartPt);
                newRect.setTopRight(EndPt);
                RectVec.at(RectCount)->setRect(newRect);
            }
            else if((RectVec.at(RectCount)->rect().y() > EndPt.y())&&
                    (RectVec.at(RectCount)->rect().x() > EndPt.x()))
            {
                QRectF newRect;
                newRect.setBottomRight(StartPt);
                newRect.setTopLeft(EndPt);
                RectVec.at(RectCount)->setRect(newRect);
            }
            else if((RectVec.at(RectCount)->rect().top() < EndPt.y())
                    &&(RectVec.at(RectCount)->rect().left() > EndPt.x()))
            {
                QRectF newRect;
                newRect.setTopRight(StartPt);
                newRect.setBottomLeft(EndPt);
                RectVec.at(RectCount)->setRect(newRect);
            }
        }
        else if(flag == drawElli)
        {
            if( (ElliVec.at(ElliCount)->rect().y() <  EndPt.y() ) &&
                    (ElliVec.at(ElliCount)->rect().x() < EndPt.x() )  )
            {
                QRectF newRect;
                newRect.setTopLeft(StartPt);
                newRect.setBottomRight(EndPt);
                ElliVec.at(ElliCount)->setRect(newRect);
            }
            else if((ElliVec.at(ElliCount)->rect().top() > EndPt.y() )&&
                    (ElliVec.at(ElliCount)->rect().left() < EndPt.x() ))
            {
                QRectF newRect;
                newRect.setBottomLeft(StartPt);
                newRect.setTopRight(EndPt);
                ElliVec.at(ElliCount)->setRect(newRect);
            }
            else if((ElliVec.at(ElliCount)->rect().y() > EndPt.y())&&
                    (ElliVec.at(ElliCount)->rect().x() > EndPt.x()))
            {
                QRectF newRect;
                newRect.setBottomRight(StartPt);
                newRect.setTopLeft(EndPt);
                ElliVec.at(ElliCount)->setRect(newRect);
            }
            else if((ElliVec.at(ElliCount)->rect().top() < EndPt.y())
                    &&(ElliVec.at(ElliCount)->rect().left() > EndPt.x()))
            {
                QRectF newRect;
                newRect.setTopRight(StartPt);
                newRect.setBottomLeft(EndPt);
                ElliVec.at(ElliCount)->setRect(newRect);
            }
        }

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
            if(flag == drawLine)
            {
                 LineCount++;
            }
            else if(flag == drawRect)
            {
                RectCount++;
            }
            else if(flag == drawElli)
            {
                ElliCount++;
            }
            mode = NORMAL;     // 不加就报错,会多产生moveEvent,为什么
            this->setDragMode(QGraphicsView::RubberBandDrag);        }
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
        QString t = "当前比例为 "+QString::number(matrix().m11(),'f',2)+":1";
        m_main->showScale(t);   //状态栏显示当前比例
    }
    else
        QGraphicsView::wheelEvent(event);
}

void MyView::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier && event->key()==Qt::Key_C)
        this->Copy();
    if(event->modifiers() == Qt::ControlModifier && event->key()==Qt::Key_X)
    {
        Copy();
        Delete();
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key()==Qt::Key_V)
        this->Paste();
    else if(event->modifiers() == Qt::ControlModifier && event->key()==Qt::Key_A)
    {
        this->selectAll(true);
        showStatus("已经选择所有的图元");
    }

    switch(event->key())
    {
    case Qt::Key_Space:
        this->setNormal();
        this->selectAll(false);
        break;
    case Qt::Key_Home:
        this->Locate();
        this->Reset();
        break;
    case Qt::Key_Plus:
        this->scale(1.41421, 1.41421);
        this->updateCenterRect();
        break;
    case Qt::Key_Minus:
        this->scale(0.70711, 0.70711);
        this->updateCenterRect();
        break;
    case Qt::Key_PageUp:
        this->Zoom(true);
        break;
    case Qt::Key_PageDown:
        this->Zoom(false);
        break;
    case Qt::Key_M :
        Cmd = new Command(this);
        Cmd->SetMovable(!m_movable);
        break;
    case Qt::Key_C :
        Cmd = new Command(this);
        Cmd->CatchPt();
        break;
    case Qt::Key_V :
        Cmd = new Command(m_scene);
        Cmd->Stretch();
        break;
    case Qt::Key_D :
        Cmd = new Command(this);
        Cmd->changeStyle();
        break;
    case Qt::Key_F :
        Cmd = new Command(this);
        Cmd->FillBrush();
        break;
    case Qt::Key_R :
        this->showItemInfo();
        break;
    case Qt::Key_Delete:
        this->Delete();
        break;
    case Qt::Key_Up:
        this->Translate(Command::UP);
        showStatus("向上平移5个单位");
        break;
    case Qt::Key_Down:
        this->Translate(Command::DOWN);
        showStatus("向下平移5个单位");
        break;
    case Qt::Key_Left:
        this->Translate(Command::LEFT);
        showStatus("向左平移5个单位");
        break;
    case Qt::Key_Right:
        this->Translate(Command::RIGHT);
        showStatus("向右平移5个单位");
        break;
    case Qt::Key_T:
        Cmd = new Command(this);
        Cmd->test();
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
    if(sender()->objectName() == "act1")
    {
        flag = drawCirPt;
        changeCursor("cross");
        showStatus("当前为编辑模式");
    }
    else if(sender()->objectName() == "act2")
    {
        flag = drawCross;
        changeCursor("cross");
        showStatus("当前为编辑模式");
    }
    else if(sender()->objectName() == "act3")
    {
        flag = drawPtXY;
        changeCursor(Qt::ArrowCursor);
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
    if(sender()->objectName() == "actLine_1")
    {
        flag = drawLine;
        changeCursor("cross");
        showStatus("当前为编辑模式");
    }
    else if(sender()->objectName() == "actLine_2")
    {
        flag = drawLineXY;
        changeCursor(Qt::ArrowCursor);
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
        m_scene->addLine(QLineF(list.at(0), list.at(1)),getPen())->
                setFlag(QGraphicsItem::ItemIsSelectable);
        setNormal();
    }
    else if(sender()->objectName() == "actLine_3")
    {
        flag = drawLineAH;
        changeCursor(Qt::ArrowCursor);
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
        setNormal();
    }
}

void MyView::DrawRect()
{
    if(sender()->objectName()=="actRect_1")
    {
        flag = drawRect;
        changeCursor("cross");
        showStatus("当前为编辑模式");
    }
    else if(sender()->objectName()=="actRect_2")
    {
        flag = drawRectXY;
        changeCursor(Qt::ArrowCursor);
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
}

void MyView::DrawEllipse()
{
    if(sender()->objectName()=="actEllipse_1")
    {
        flag = drawElli;
        changeCursor("cross");
        showStatus("当前为编辑模式");
    }
    else if(sender()->objectName()=="actEllipse_2")
    {
        flag =drawElliXY;
        changeCursor(Qt::ArrowCursor);
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
        flag = drawRing;
        changeCursor("cross");
        showStatus("当前为编辑模式");
    }
}

void MyView::DrawTexts()
{
    flag = drawText;
    changeCursor("cross");
    showStatus("当前为编辑模式");
    if(sender()->objectName()=="textAct")
        m_drawMulti = false;
    else if(sender()->objectName()=="multiTextAct")
        m_drawMulti = true;
}

void MyView::ShowContextMenu()
{
    QMenu m;
    QAction *Normal = m.addAction("重置为普通模式");
    QAction *Locate = m.addAction("定位到原点");
    QAction *Reset = m.addAction("重置视图");
    QAction *Delete = m.addAction("删除");
    QAction *Redraw = m.addAction("清空重画");
    QAction *Cut = m.addAction("剪切");
    QAction *Copy = m.addAction("复制");
    QAction *Paste = m.addAction("黏贴");
    QAction* Info = m.addAction("属性");

    Normal->setIcon(QIcon(":/Icon/Icon/normal.png"));
    Locate->setIcon(QIcon(":/Icon/Icon/locate.png"));
    Reset->setIcon(QIcon(":/Icon/Icon/reset.png"));
    Delete->setIcon(QIcon(":/Icon/Icon/delete.png"));
    Redraw->setIcon(QIcon(":/Icon/Icon/redraw.png"));
    Cut->setIcon(QIcon(":/Icon/Icon/cut.png"));
    Copy->setIcon(QIcon(":/Icon/Icon/copy.png"));
    Paste->setIcon(QIcon(":/Icon/Icon/paste.png"));
    Info->setIcon(QIcon(":/Icon/Icon/info.png"));

    connect(Normal,SIGNAL(triggered(bool)), this, SLOT(setNormal()) );
    connect(Locate,SIGNAL(triggered(bool)), this, SLOT(Locate()) );
    connect(Reset,SIGNAL(triggered(bool)), this,  SLOT(Reset()) );
    connect(Delete, SIGNAL(triggered(bool)), this, SLOT(Delete()) );
    connect(Cut,&QAction::triggered, [this]{this->Copy(); this->Delete();} );
    connect(Copy,SIGNAL(triggered(bool)), this,  SLOT(Copy()) );
    connect(Paste,SIGNAL(triggered(bool)), this,  SLOT(Paste()) );
    connect(Redraw,SIGNAL(triggered(bool)), this, SLOT(Redraw()) );
    connect(Info, SIGNAL(triggered(bool)), this, SLOT(showItemInfo()) );

    m.exec(QCursor::pos());
}

void MyView::setNormal()
{
    mode = NORMAL;
    flag = drawNone;
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
    showStatus("重置");
}

void MyView::Zoom(bool in)
{
    Cmd = new Command(m_scene);
    Cmd->Zoom(in);
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
        QMessageBox::warning(0,"出错了!","没有复制图元数据");
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
    m_main->statusBar()->setFont(QFont("Inconsolata",14));
    m_main->statusBar()->showMessage(msg);
}

QString MyView::inputMultiText(bool multi)
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
    InitParameters();
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    //锚点以鼠标为准,放缩时效果跟网络地图一样
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setMatrix(QMatrix(1,0,0,-1,0,0));     // x,y轴比例为1:1和1:-1
//    qDebug()<<this->transform();
//    qDebug()<<matrix().m11() << matrix().m22();
}

void MyView::InitParameters()
{
    mode = NORMAL;
    flag = drawNone;
    m_drawMulti = false;
    m_copied = false;
    m_movable = false;
    m_saved = false;
    m_new = false;
    LineCount = 0;
    RectCount = 0;
    ElliCount = 0;
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
    //sender()->objectName();  函数也能找到 sender()
    pen.setWidth(getPenWidth());
    pen.setStyle(getPenStyle());
    pen.setColor(getColor());
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
