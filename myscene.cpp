#include "myscene.h"
#include <QMessageBox>
const int MAGIC = 0x1234;

MyScene::MyScene(QObject *parent):
    QGraphicsScene(parent)
{
    setSceneRect(-width/2,-height/2,width,height); //场景坐标系,超出view大小加滑条
    setBackgroundBrush(QBrush(QColor(33,40,48)));
    InitScene();

    mode = NONE;
    m_draft = false;
    m_pressed = false;
}

MyScene::~MyScene()
{

}

void MyScene::InitText()
{
    QFont font;
    font.setPointSizeF(10);
    font.setFamily("Inconsolata");

    X = this->addSimpleText("X轴",font);
    X->setPos(120,-5);
    //    qDebug()<<"X的坐标:"<<X->mapToScene(X->pos());   //总是Pos的2倍
    X->setTransform(QTransform::fromScale(1,-1));
    X->setBrush(QBrush(Qt::darkCyan,Qt::SolidPattern));

    Y = this->addSimpleText("Y轴",font);
    Y->setPos(-30,120);
    Y->setTransform(QTransform::fromScale(1,-1));
    Y->setBrush(QBrush(Qt::darkCyan,Qt::SolidPattern));
}

void MyScene::InitData()
{
    // 给数据,用于跟添加的图形区分
    Origin->setData(0,"Origin");
    AxisX->setData(0,"AxisX");
    AxisY->setData(0,"AxisY");
    ArrowX->setData(0,"ArrowX");
    ArrowY->setData(0,"ArrowY");
    X->setData(0,"X");
    Y->setData(0,"Y");
}

void MyScene::InitShape()
{
    Origin = this->addEllipse( -3, -3, 2*3, 2*3, QPen(QColor(122,103,238)),
                               QBrush(QColor(122,103,238), Qt::SolidPattern) );
    Origin->setToolTip("原点");
    AxisX = this->addLine(QLineF(QPointF(-width/2+1,0), QPointF(width/2-1,0)), QPen(QColor(139,54,38)));
    AxisY = this->addLine(QLineF(QPointF(0,-height/2+1),QPointF(0,height/2-1)), QPen(QColor(139,54,38)));

    QPolygonF poly_X,poly_Y;
    poly_X<<QPointF(80, 0)<<QPointF(72, 3)<<QPointF(72,-3);
    poly_Y<<QPointF(0, 80)<<QPointF(3, 72)<<QPointF(-3, 72);
    ArrowX = this->addPolygon(poly_X,QPen(QColor(Qt::darkMagenta)),QBrush(QColor(Qt::darkMagenta)));
    ArrowY = this->addPolygon(poly_Y,QPen(QColor(Qt::darkMagenta)),QBrush(QColor(Qt::darkMagenta)));
}

void MyScene::InitScene()
{
    InitShape();
    InitText();
    InitData();
}

void MyScene::UnloadScene()
{
    removeItem(AxisX);
    removeItem(AxisY);
    removeItem(ArrowX);
    removeItem(ArrowY);
    removeItem(Origin);
    removeItem(X);
    removeItem(Y);
}

void MyScene::Save(QDataStream &s)
{
    s<< MAGIC;
    QList<QGraphicsItem*> all = items(this->sceneRect(),Qt::IntersectsItemShape,Qt::AscendingOrder);
    //去掉两个坐标轴和两个箭头,原点,场景矩形. 为什么场景矩形也算进items ?
    s<<all.count()-8;
    Export(s, all);
}

void MyScene::Export(QDataStream& s, QList<QGraphicsItem *> items)
{
    foreach (QGraphicsItem* item, items)
    {
        if(!item->data(0).toString().isEmpty())
            continue;

        QString className;

        if(item->type()==QGraphicsRectItem::Type)
        {
            className = "QGraphicsRectItem";
            QGraphicsRectItem* rectangle = qgraphicsitem_cast<QGraphicsRectItem*>(item);
            s<< className;
            s<< int(rectangle->flags());
            s<< rectangle->rect().x();
            s<< rectangle->rect().y();
            s<< rectangle->rect().width();
            s<< rectangle->rect().height();
            s<< rectangle->pos().x();
            s<< rectangle->pos().y();
            s<< rectangle->pen().color();
            s<< int(rectangle->pen().style());  //强制转换Qt::PenStyle
            s<< rectangle->pen().width();
            s<< rectangle->brush().color();
            s<< int(rectangle->brush().style());
        }
        else if(item->type()==QGraphicsEllipseItem::Type)
        {
            className = "QGraphicsEllipseItem";
            QGraphicsEllipseItem* rectangle = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
            s<< className;
            s<< int(rectangle->flags());
            s<< rectangle->rect().x();
            s<< rectangle->rect().y();
            s<< rectangle->rect().width();
            s<< rectangle->rect().height();
            s<< rectangle->pos().x();
            s<< rectangle->pos().y();
            s<< rectangle->pen().color();
            s<< int(rectangle->pen().style());  //强制转换Qt::PenStyle
            s<< rectangle->pen().width();
            s<< rectangle->brush().color();
            s<< int(rectangle->brush().style());
        }
        else if(item->type()== CrossPt::Type)
        {
            className = "CrossPt";
            CrossPt* rectangle = qgraphicsitem_cast<CrossPt*>(item);
            s<< className;
            s<< int(rectangle->flags());
            s<< rectangle->rect().x();
            s<< rectangle->rect().y();
            s<< rectangle->rect().width();
            s<< rectangle->rect().height();
            s<< rectangle->pos().x();
            s<< rectangle->pos().y();
        }
        else if(item->type()== CirclePt::Type)
        {
            className = "CirclePt";
            CirclePt* rectangle = qgraphicsitem_cast<CirclePt*>(item);
            s<< className;
            s<< int(rectangle->flags());
            s<< rectangle->rect().x();
            s<< rectangle->rect().y();
            s<< rectangle->rect().width();
            s<< rectangle->rect().height();
            s<< rectangle->pos().x();
            s<< rectangle->pos().y();
        }
        else if(item->type()== MyLine::Type)
        {
            className = "MyLine";
            MyLine* myline = qgraphicsitem_cast<MyLine*>(item);
            s<< className;
            s<< int(myline->flags());
            s<<myline->getP1();
            s<<myline->getP2();
            s<<myline->getP3();
        }
        else if(item->type()== QGraphicsSimpleTextItem::Type)
        {
            className = "QGraphicsSimpleTextItem";
            QGraphicsSimpleTextItem* text = qgraphicsitem_cast<QGraphicsSimpleTextItem*>(item);
            s<< className;
            s<< int(text->flags());
            s<< text->text();
            s<< text->font().family();
            s<< text->font().pointSizeF();
            s<< text->font().bold();
            s<< text->pos().x();
            s<< text->pos().y();
            s<< text->pen().color();
            s<< int(text->pen().style());  //强制转换Qt::PenStyle
            s<< text->pen().width();
        }
        else if(item->type()==QGraphicsLineItem::Type)
        {
            className = "QGraphicsLineItem";
            QGraphicsLineItem* Line = qgraphicsitem_cast<QGraphicsLineItem*>(item);
            s<< className;
            s<< Line->flags();
            s<< Line->line().x1();
            s<< Line->line().y1();
            s<< Line->line().x2();
            s<< Line->line().y2();
            s<< Line->pen().color();
            s<< int(Line->pen().style());  //强制转换Qt::PenStyle
            s<< Line->pen().width();
        }
    }
}

bool MyScene::Load(QDataStream &s)
{
    int magic;
    s>>magic;
    if(magic!=MAGIC){
        QMessageBox::warning(0, QStringLiteral("出错了"),
                             QStringLiteral("打开的不是gph文件!"));
        return false;
    }
    InitScene();
    int count;
    s>>count;
    Import(s,count);
    return true;
}

void MyScene::Import(QDataStream &s, int count)
{
    for(int i=0;i<=count;i++)
    {
        QString className;
        int flags;
        QColor c;
        int style;
        int width;
        QPen pen;

        QColor b;
        int brushStyle;
        QBrush brush;

        s>>className;   //不能直接用字符串
        s >> flags;
        if(className =="QGraphicsEllipseItem"||className=="QGraphicsRectItem")
        {
            qreal x,y,w,h;
            qreal px,py;
            s >> x;     s >> y;
            s >> w;     s >> h;
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
                QGraphicsEllipseItem* ell = this->addEllipse(x,y,w,h, pen);
                ell->setBrush(brush);
                ell->setPos(px,py);
                ell->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
            }
            else if(className=="QGraphicsRectItem")
            {
                QGraphicsRectItem* rect = this->addRect(x,y,w,h, pen);
                rect->setBrush(brush);
                rect->setPos(px,py);
                rect->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
            }
        }
        else if(className =="CrossPt")
        {
            qreal x,y,w,h;
            qreal px,py;
            s >> x;     s >> y;
            s >> w;     s >> h;
            s >> px;    s >> py;
            CrossPt *pt = new CrossPt();
            pt->setBoundingRect(QRect(x,y,w,h));
            pt->setPos(px,py);
            pt->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
            this->addItem(pt);
        }
        else if(className =="CirclePt")
        {
            qreal x,y,w,h;
            qreal px,py;
            s >> x;     s >> y;
            s >> w;     s >> h;
            s >> px;    s >> py;
            CirclePt *pt = new CirclePt();
            pt->setBoundingRect(QRect(x,y,w,h));
            pt->setPos(px,py);
            pt->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
            this->addItem(pt);
        }
        else if(className == "MyLine")
        {
            QPointF p1,p2,p3;
            s>>p1;
            s>>p2;
            s>>p3;

            MyLine* line = new MyLine(0,p1,p3);
            line->setView(getCurrentView());
            addItem(line);
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

            QGraphicsSimpleTextItem* Text = this->addSimpleText(text,font);
            Text->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
            Text->setPos(px,py);
            Text->setTransform(QTransform::fromScale(1,-1));
            Text->setPen(pen);
        }
        else if(className=="QGraphicsLineItem")
        {
            qreal x1,y1,x2,y2;

            s>>x1; s>>y1; s>>x2; s>>y2;
            s >> c; s >> style; s >> width;

            pen.setColor(c);
            pen.setStyle(Qt::PenStyle(style) );
            pen.setWidth(width);
            this->addLine(x1,y1,x2,y2, pen)->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
        }
    }
}

void MyScene::setMode(MyScene::GridMode m)
{
    mode = m;
    this->update();
}

void MyScene::setDraftMode(bool on)
{
    m_draft = on;
    view = getCurrentView();
}

void MyScene::selectPen(QPen p)
{
    pen = p;
}

//如果此函数声明，但函数体为空，则场景是白色背景,不会调用 setBackgroundBrush
void MyScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();

    double x = rect.x();
    double y = rect.y();
    QRectF newRect(rect);
    newRect.setTopLeft(QPointF(floor(x/space)*space,
                               floor(y/space)*space) );

    QPolygonF greenPoints;
    //调用构造函数里的 setBackgroundBrush
    painter->fillRect(newRect, backgroundBrush());

    switch (mode)
    {
    case POINT:
        for (qreal i = newRect.left(); i < newRect.right(); i = i + min_space)
        {
            for (qreal j = newRect.top(); j < newRect.bottom(); j = j + min_space)
                greenPoints.append(QPointF(i, j));
        }
        break;
    case GRID:
        //画粗线,先竖线后横线
        for (qreal i = newRect.left(); i < newRect.right(); i = i + space)
        {
            painter->setPen(QColor(48,54,100));
            painter->drawLine(QLineF( QPointF(i,newRect.top()), QPointF(i,newRect.bottom()) ) );
        }
        for (qreal j = newRect.top(); j < newRect.bottom(); j = j + space)
        {
            painter->setPen(QColor(48,54,100));
            painter->drawLine(QLineF( QPointF(newRect.left(), j), QPointF(newRect.right(), j) ) );
        }
        //画细线
        for (qreal i = newRect.left(); i < newRect.right(); i = i + min_space)
        {
            painter->setPen(QColor(37,50,70));
            if(qRound(i) %space !=0)
                painter->drawLine(QLineF( QPointF(i,newRect.top()), QPointF(i,newRect.bottom()) ) );
        }
        for (qreal j = newRect.top(); j < newRect.bottom(); j = j + min_space)
        {
            painter->setPen(QColor(37,50,70));
            if(qRound(j) %space !=0)
                painter->drawLine(QLineF( QPointF(newRect.left(), j), QPointF(newRect.right(), j) ) );
        }
        break;
    case NONE:
        break;
    default:
        break;
    }

    if (!greenPoints.empty())
    {
        painter->setPen(Qt::darkGreen);
        painter->drawPoints(greenPoints);
    }
    // Long origin marker
    painter->setPen(Qt::lightGray);
    painter->drawLine(0, rect.top(), 0, rect.bottom());
    painter->drawLine(rect.left(), 0, rect.right(), 0);

    painter->restore();
}
// 鼠标事件实现草图模式
void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_draft)    return;
    if(event->button()!=Qt::LeftButton)     return;

    m_pressed = true;
    x = event->scenePos().x();
    y = event->scenePos().y();
    this->update();
    QGraphicsScene::mousePressEvent(event);
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_draft || !m_pressed)    return;
    tempItem = (QGraphicsItem*)addLine(x,y,event->scenePos().x(),
                                       event->scenePos().y(),
                                       view->getPen());
    x = event->scenePos().x();
    y = event->scenePos().y();

    this->update();
    QGraphicsScene::mouseMoveEvent(event);
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_draft)    return;
    this->update();
    m_pressed = false;
    QGraphicsScene::mouseReleaseEvent(event);
}

// 覆盖了QGraphicsScene::itemsBoundingRect()的源码
QRectF MyScene::itemsBoundingRect() const
{
    QRectF boundingRect;
    for (QGraphicsItem *item : this->items())
    {
        if(item->data(0).toString().isEmpty())
            boundingRect |= item->sceneBoundingRect();
    }
    return boundingRect;
}

MyView* MyScene::getCurrentView()
{
    view = qobject_cast<MyView*>(this->views().at(0));
    return view;
}
