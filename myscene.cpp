#include "myscene.h"
#include <QDebug>
#include <QMessageBox>
const int MAGIC = 0x1234;

MyScene::MyScene(QObject *parent):
    QGraphicsScene(parent)
{
    InitScene();
    space = 50;
    min_space = 10;
    mode = ALL;
}

MyScene::~MyScene()
{

}

void MyScene::InitScene()
{
    this->setSceneRect(-width/2,-height/2,width,height); //场景坐标系,超出view大小加滑条
    this->setBackgroundBrush(QBrush(QColor(0,43,54)));
//    画圆心, QPen 是圆的边缘, QBrush是圆的填充
    Origin = this->addEllipse( -3, -3, 2*3, 2*3, QPen(QColor(122,103,238)),
                               QBrush(QColor(122,103,238), Qt::SolidPattern) );
    Origin->setToolTip("原点");
//    画两个坐标轴
    AxisX = this->addLine(QLineF(QPointF(-width/2+1,0), QPointF(width/2-1,0)), QPen(QColor(139,54,38)));
    AxisY = this->addLine(QLineF(QPointF(0,-height/2+1),QPointF(0,height/2-1)), QPen(QColor(139,54,38)));
//    画两个箭头
    QPolygonF poly_X,poly_Y;
    poly_X<<QPointF(80, 0)<<QPointF(72, 3)<<QPointF(72,-3);
    poly_Y<<QPointF(0, 80)<<QPointF(3, 72)<<QPointF(-3, 72);
    ArrowX = this->addPolygon(poly_X,QPen(QColor(Qt::darkMagenta)),QBrush(QColor(Qt::darkMagenta)));
    ArrowY = this->addPolygon(poly_Y,QPen(QColor(Qt::darkMagenta)),QBrush(QColor(Qt::darkMagenta)));

    // 给数据, 用于跟添加的图元区分
    Origin->setData(0,"origin");
    AxisX->setData(0,"x");
    AxisY->setData(0,"y");
    ArrowX->setData(0,"arrowX");
    ArrowY->setData(0,"arrowY");

    QFont font;
    font.setPointSizeF(10);
    font.setFamily("Inconsolata");
    //两个文字
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

void MyScene::Save(QDataStream &s)
{
    s<< MAGIC;
    QList<QGraphicsItem*> all = items(this->sceneRect(),
                                      Qt::IntersectsItemShape,Qt::AscendingOrder);
    //去掉两个坐标轴和两个箭头,原点,场景矩形. 为什么场景矩形也算进items ?
    s<<all.count()-6;
    Export(s, all);
}

void MyScene::Export(QDataStream& s, QList<QGraphicsItem *> items)
{
    foreach (QGraphicsItem* item, items)
    {
        if(item->data(0).toString()=="origin"||item->data(0).toString()=="x"
                ||item->data(0).toString()=="y"||item->data(0).toString()=="arrowX"
                ||item->data(0).toString()=="arrowY")
        {
            continue;
        }
        QString className;

        if(item->type()==3)
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
        }
        else if(item->type()==4)
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

        else if(item->type()==6)
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

void MyScene::Load(QDataStream &s)
{
    int magic;
    s>>magic;
    if(magic!=MAGIC)
    {
        QMessageBox::warning(0, QStringLiteral("出错了"),
                             QStringLiteral("打开的不是gph文件!"));
        return;
    }

    InitScene();
    int count;
    s>>count;
    Import(s,count);
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
        s>>className;   //不能直接用字符串
        s >> flags;
        if(className=="QGraphicsEllipseItem"||className=="QGraphicsRectItem")
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

            if(className=="QGraphicsEllipseItem")
            {
                QGraphicsEllipseItem* ell = this->addEllipse(x,y,w,h, pen);
                ell->setPos(px,py);
                ell->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
            }
            else if(className=="QGraphicsRectItem")
            {
                QGraphicsRectItem* rect = this->addRect(x,y,w,h, pen);
                rect->setPos(px,py);
                rect->setFlags(QGraphicsItem::GraphicsItemFlags(flags));
            }
        }
        else if(className=="CrossPt")
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
        else if(className=="CirclePt")
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

#if 0
//如果此函数声明，但函数体为空，则场景是白色背景,不会调用 setBackgroundBrush
void MyScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();

    double x = rect.x();
    double y = rect.y();

    QRectF newRect(rect);
    newRect.setTopLeft(QPointF(floor(x/space)*space,
                               floor(y/space)*space) );

    QPolygonF whitePoints, greenPoints;
    painter->fillRect(newRect, backgroundBrush());

    switch (mode)
    {
    case ALL:
        for (qreal i = newRect.left(); i < newRect.right(); i = i + min_space)
        {
            for (qreal j = newRect.top(); j < newRect.bottom(); j = j + min_space)
            {
                greenPoints.append(QPointF(i, j));
            }
        }
    case MAJOR:
        for (qreal i = newRect.left(); i < newRect.right(); i = i + space)
        {
            for (qreal j = newRect.top(); j < newRect.bottom(); j = j + space)
            {
                whitePoints.append(QPointF(i, j));
            }
        }
    case NONE:
    default:
        break;
    }

    if (!greenPoints.empty())
    {
        painter->setPen(Qt::darkGreen);
        painter->drawPoints(greenPoints);
    }

    if (!whitePoints.empty())
    {
        painter->setPen(Qt::white);
        painter->drawPoints(whitePoints);
    }

    // Short origin marker
    //    painter->setPen(Qt::lightGray);
    //    painter->drawLine(-20, 0, 20, 0);
    //    painter->drawLine(0, -20, 0, 20);

    // Long origin marker
    painter->setPen(Qt::lightGray);
    painter->drawLine(0, rect.top(), 0, rect.bottom());
    painter->drawLine(rect.left(), 0, rect.right(), 0);

    painter->restore();
    //    qDebug()<<"draw background";
}
#endif
