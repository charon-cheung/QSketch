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
    // 获得主窗口指针的方法,很重要！
    m_main = qobject_cast<MainWindow*>(this->topLevelWidget());
    m_main->showScale("当前比例为 1:1 ");
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

MainWindow *MyView::getMainWindow()
{
    return m_main;
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
        if(flag == dragZoom)
        {
            mode = ZOOM;
        }
        else if(flag == drawNone)
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
                pt1->setPos(StartPt);  //这里不是图形坐标，是场景坐标
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
                Text->setPen(getPen());
                mode = NORMAL;     //画完文本后没有releaseEvent的恢复普通模式,只能在这里添加
                this->setDragMode(QGraphicsView::RubberBandDrag);
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
    QPointF EndPt = this->mapToScene(event->pos()); // 不能直接mapToScene(pt).y()
    QString MousePos = "("+ QString::number(EndPt.x()) +","+ QString::number(EndPt.y()) +")";
    switch (mode) {
    case NORMAL:
    {
        m_main->showCoordinate("当前坐标为:"+MousePos);
        break;  // 需要处理QGraphicsView::mouseMoveEvent
    }
    case ZOOM:
        break;
    case DRAG:
    {
        //Calculate the offset to drag relative to scene coordinates.
        dragTrans = dragBegin - EndPt;
        viewCenter->moveBy(dragTrans.x(), dragTrans.y());
        this->centerOn(viewCenter);
        event->accept();
        m_main->showCoordinate("当前坐标为:"+MousePos);
        break;
    }
    case EDIT:
    {
        m_main->showCoordinate("当前坐标为:"+MousePos);
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
    QPointF ReleasPt = this->mapToScene(event->pos());
    switch(event->button())
    {
    case Qt::MidButton:
        mode = NORMAL;
        changeCursor(Qt::ArrowCursor);
        event->accept();
        showStatus("当前为普通模式");
        break;
    case Qt::LeftButton:
        if(mode==ZOOM)
        {   //窗口缩放函数,最后的比例参数选择很重要
            this->fitInView( QRectF(StartPt,ReleasPt), Qt::KeepAspectRatio);
        }
        else if(mode==EDIT)
        {
            if(flag == drawLine)
                LineCount++;
            else if(flag == drawRect)
                RectCount++;
            else if(flag == drawElli)
                ElliCount++;

            mode = NORMAL;     // 不加就报错,会多产生moveEvent,为什么
            this->setDragMode(QGraphicsView::RubberBandDrag);
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
        QString t = "当前比例为 "+QString::number(matrix().m11(),'f',2)+":1 ";
        m_main->showScale(t);   //状态栏显示当前比例
    }
    else
        QGraphicsView::wheelEvent(event);
}

void MyView::keyPressEvent(QKeyEvent *event)
{
    if(pressCtrlKey(event, Qt::Key_A))
    {
        Cmd = new Command(this);
        Cmd->SelectAll(true);
    }
    else if(pressCtrlKey(event, Qt::Key_Return))
        setFullView(!m_full);

    switch(event->key())
    {
    case Qt::Key_Space:
        this->setNormalMode();
        break;
    case Qt::Key_Home:
        this->Locate();
        this->Reset();
        break;
    case Qt::Key_M :
        Cmd = new Command(this);
        Cmd->SetMovable(!m_movable);
        break;
    case Qt::Key_C :
        setCatch(!m_catch);     //捕捉模式
        break;
    case Qt::Key_D:
        Cmd = new Command(this);
        Cmd->CatchPt();
        break;
    case Qt::Key_Q:
        Cmd = new Command(this);
        Cmd->Rotate(45);
        showStatus("逆时针转45°");
        break;
    case Qt::Key_E:
        Cmd = new Command(this);
        Cmd->Rotate(-45);
        showStatus("顺时针转45°");
        break;
    case Qt::Key_Up:
        this->Translate(Command::UP);
        showStatus("向上平移"+QString::number(Command::PACE)+"个单位");
        break;
    case Qt::Key_Down:
        this->Translate(Command::DOWN);
        showStatus("向下平移"+QString::number(Command::PACE)+"个单位");
        break;
    case Qt::Key_Left:
        this->Translate(Command::LEFT);
        showStatus("向左平移"+QString::number(Command::PACE)+"个单位");
        break;
    case Qt::Key_Right:
        this->Translate(Command::RIGHT);
        showStatus("向右平移"+QString::number(Command::PACE)+"个单位");
        break;
    case Qt::Key_PageUp:
        this->PaceUp();
        break;
    case Qt::Key_PageDown:
        this->PaceDown();
        break;
    default:
        event->ignore();
        break;
    }
}

//捕捉点,鼠标定位到某个场景点对应的全局坐标
void MyView::catchPt(QPointF pt)
{
    //场景坐标转全局坐标：先转为视图坐标，再转为全局坐标
    QPointF f= mapFromScene(pt);
    f = mapToGlobal(f.toPoint());
    QCursor::setPos(f.toPoint());
    changeCursor("crosshairs");
}

void MyView::setCatch(bool on)
{
    m_catch = on;
    if(on)
        showStatus("开启捕捉模式");
    else
        showStatus("关闭捕捉模式");
}

void MyView::setFullView(bool full)
{
    m_full = full;
    if(full)
        m_main->showFullView(true);
    else
        m_main->showFullView(false);
}

bool MyView::goCatch()
{
    return m_catch;
}

// 获得直线的终点
QPointF MyView::getP3()
{
    return p2;
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
    else if(sender()->objectName() == "dividePt")
    {
        flag = drawDividePt;
        changeCursor(Qt::ArrowCursor);

        Cmd = new Command(this);
        QList<QPointF> Positions = Cmd->getDividePts();
        if(Positions.isEmpty())    return;

        foreach (QPointF pos, Positions)
        {
            CirclePt *pt = new CirclePt();
            pt->setPos(pos);
            m_scene->addItem(pt);
        }
        setNormalMode();
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

        MyLine* line = new MyLine(0,list.at(0),list.at(1));
        line->setView(this);
        m_scene->addItem(line);
//        m_scene->addLine(QLineF(list.at(0), list.at(1)),getPen())->
//                setFlag(QGraphicsItem::ItemIsSelectable);
        setNormalMode();
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
        p2 = line.p2();

        MyLine* L = new MyLine(0, pt1, length, angle);
        L->setView(this);
        m_scene->addItem(L);
//        m_scene->addLine(line, getPen())->setFlag(QGraphicsItem::ItemIsSelectable);
        setNormalMode();
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
        setNormalMode();
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
        setNormalMode();
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
    QAction *Normal = m.addAction("重置为普通模式  Space");
    QAction *Locate = m.addAction("定位到原点");
    QAction *Reset = m.addAction("重置视图  Home");
    QAction *Delete = m.addAction("删除");
    QAction *Empty = m.addAction("清空重画");
    QAction* Info = m.addAction("属性");

    Normal->setIcon(QIcon(":/Icon/Icon/normal.png"));
    Locate->setIcon(QIcon(":/Icon/Icon/locate.png"));
    Reset->setIcon(QIcon(":/Icon/Icon/reset.png"));
    Delete->setIcon(QIcon(":/Icon/Icon/delete.png"));
    Empty->setIcon(QIcon(":/Icon/Icon/empty.png"));
    Info->setIcon(QIcon(":/Icon/Icon/info.png"));

    connect(Normal,SIGNAL(triggered(bool)), this, SLOT(setNormalMode()) );
    connect(Locate,SIGNAL(triggered(bool)), this, SLOT(Locate()) );
    connect(Reset,SIGNAL(triggered(bool)), this,  SLOT(Reset()) );
    connect(Delete, SIGNAL(triggered(bool)), this, SLOT(Delete()) );
    connect(Empty,SIGNAL(triggered(bool)), this, SLOT(Empty()) );
    connect(Info, SIGNAL(triggered(bool)), this, SLOT(showItemInfo()) );

    m.exec(QCursor::pos());
}

void MyView::setZoomMode(bool on)
{
    if(on)
        flag = dragZoom;
}

void MyView::setDraftMode(bool on)
{
    getScene()->setDraftMode(on);
    if(on)
    {
        this->setDragMode(QGraphicsView::NoDrag);
        m_main->DraftStatusBar(true);
        getScene()->UnloadScene();
        getScene()->selectPen(getPen());
    }
    else
    {
        // 提示是否保存
        m_main->DraftStatusBar(false);
        Empty();
        Reset();
        this->setDragMode(QGraphicsView::RubberBandDrag);
    }
}

void MyView::setNormalMode()
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
    changeCursor(Qt::ArrowCursor);
    showStatus("鼠标切换至原点");
}

void MyView::Reset()
{
    this->resetMatrix();
    this->scale(1,-1);
    this->centerOn(0,0);
    this->updateCenterRect();
    showStatus("重置画面");
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
        QMessageBox::warning(0,"出错了!","没有复制图形数据");
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
        int flags;
        QColor c;
        int style;
        int width;
        QPen pen;

        QTransform t;
        QColor b;
        int brushStyle;
        QBrush brush;

        s>>className;   //不能直接用字符串
        s >> flags;
        // 四种图形的复制基点都是中心点
        if(className=="QGraphicsEllipseItem"||className=="QGraphicsRectItem"
                || className=="CrossPt"|| className=="CirclePt")
        {
            qreal x,y,w,h;
            qreal px,py;
            s >> x; s >> y;
            s >> w; s >> h;
            s >> px;    s >> py;
            s >> c; s >> style; s >> width;

            pen.setColor(c);
            pen.setStyle(Qt::PenStyle(style) );
            pen.setWidth(width);

            s >> b; s>>brushStyle;
            brush.setColor(b);
            brush.setStyle(Qt::BrushStyle(brushStyle) );
            if(className=="QGraphicsEllipseItem")
            {
                QGraphicsEllipseItem* Elli = getScene()->addEllipse(pos.x(),pos.y(),w,h,pen );
                Elli->setBrush(brush);
                Elli->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
//                QPointF movePt = pos - Elli->rect().center();
//                t.translate(movePt.x(), movePt.y());
//                Elli->setTransform(t,true);
            }
            else if(className=="QGraphicsRectItem")
            {
                QGraphicsRectItem* Rect = getScene()->addRect(pos.x(),pos.y(),w,h,pen );
                Rect->setBrush(brush);
                Rect->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
//                QPointF movePt = pos - Rect->rect().center();
//                t.translate(movePt.x(), movePt.y());
//                Rect->setTransform(t,true);
            }
            else if(className=="CrossPt")
            {
                CrossPt *pt = new CrossPt();
                pt->setPos(pos);
                pt->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
                this->getScene()->addItem(pt);
            }
            else if(className=="CirclePt")
            {
                CirclePt *pt = new CirclePt();
                pt->setPos(pos);
                pt->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
                this->getScene()->addItem(pt);
            }
        }
        else if(className == "MyLine")
        {
            QPointF p1,p2,p3;
            s>>p1;
            s>>p2;
            s>>p3;

            MyLine* line = new MyLine(0,p1,p3);
            line->setView(this);
//            QPointF movePt = pos - p2;
//            QTransform t;
//            t.translate(movePt.x(), movePt.y());
//            line->setTransform(t,true);
            m_scene->addItem(line);
        }
        else if(className=="QGraphicsLineItem")
        {
            qreal x1,y1,x2,y2;

            s>>x1; s>>y1; s>>x2; s>>y2;
            s >> c; s >> style; s >> width;

            pen.setColor(c);
            pen.setStyle(Qt::PenStyle(style) );
            pen.setWidth(width);
            QGraphicsLineItem *Line = getScene()->addLine(x1,y1,x2,y2, pen );
//            复制的是直线的两个端点，只能以中点为基准而平移
            QPointF mid = QPointF( (x1+x2)/2, (y1+y2)/2 );
            QPointF movePt = pos - mid;
            QTransform t;
            t.translate(movePt.x(), movePt.y());
            Line->setTransform(t,true);
            Line->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
        }
        else if(className == "QGraphicsSimpleTextItem")
        {
            QString text,family;
            qreal fontSizeF;
            bool bold;
            qreal px,py;

            s >> text;  s >> family;
            s >> fontSizeF; s >> bold;
            QFont font;
            font.setFamily(family);
            font.setPointSize(fontSizeF);
            font.setBold(bold);

            s >> px;    s >> py;
            s >> c; s >> style; s >> width;
            pen.setColor(c);
            pen.setStyle(Qt::PenStyle(style) );
            pen.setWidth(width);

            QGraphicsSimpleTextItem* Text = m_scene->addSimpleText(text,font);
            Text->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
            Text->setPos(pos);
            Text->setTransform(QTransform::fromScale(1,-1));
            Text->setPen(pen);
        }
    }
}

void MyView::Delete()
{
    Cmd = new Command(this);
    Cmd->Delete();
}

void MyView::Empty()
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

void MyView::PaceUp()
{
    Cmd = new Command(m_scene);
    Cmd->PaceUp();
}

void MyView::PaceDown()
{
    Cmd = new Command(m_scene);
    Cmd->PaceDown();
}

void MyView::updateCenterRect()
{
    QRect viewRect = this->viewport()->rect();
    // This setPos is necessary because we translate the rect on the mouseMove event handler
    // If we update the rect, the shape will still be translated, so we have to return
    // it to the centre of the scene.
    this->viewCenter->setPos(0, 0);
    qgraphicsitem_cast<QGraphicsRectItem*>(this->viewCenter)->setRect(mapToScene(viewRect).boundingRect());
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
        text = QInputDialog::getMultiLineText(0, tr("请输入多行文本"),"",
                                              "第一行\n第二行", &ok);
    else
        text = QInputDialog::getText(0, tr("请输入单行文本"),"",
                                     QLineEdit::Normal,"text",&ok);
    if (ok && !text.isEmpty())
        ok = true;
    return text;
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
    m_catch = false;
    m_full = false;
    LineCount = 0;
    RectCount = 0;
    ElliCount = 0;
}

void MyView::InitViewRect()
{
    //viewCenter用于在moveEvent里拖动画面
    QRect viewport_rect(0, 0, viewport()->width(), viewport()->height() );       //98 X 28
    QRectF visible_scene_rect = this->mapToScene(viewport_rect).boundingRect();
    viewCenter = new QGraphicsRectItem(visible_scene_rect);
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

bool MyView::pressCtrlKey(QKeyEvent *event, int key)
{
    bool modifer = (event->modifiers() == Qt::ControlModifier);
    return modifer && (event->key() == key);
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

void MyView::setDraftFile(bool flag)
{
    m_draft = flag;
}

bool MyView::isDraftFile()
{
    return m_draft;
}

void MyView::SetMoveFlag(bool flag)
{
    m_movable = flag;
}
