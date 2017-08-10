#include "myscene.h"
#include <QDebug>

MyScene::MyScene(QObject *parent):
    QGraphicsScene(parent)
{
    qDebug()<<"constructor";
    this->setSceneRect(-width/2,-height/2,width,height); //场景坐标系,超出view大小加滑条
    this->setBackgroundBrush(QBrush(Qt::black));
//    画圆心, QPen 是圆的边缘, QBrush是圆的填充
    this->addEllipse( -3, -3, 2*3, 2*3, QPen(QColor(Qt::white)),
                     QBrush(Qt::white, Qt::SolidPattern) );
    QPen p;
    p.setColor(QColor(Qt::white));
    p.setStyle(Qt::DashDotLine);
//    画两个坐标轴
    this->addLine(QLineF(QPointF(-width/2,0), QPointF(width/2,0)),p);
    this->addLine(QLineF(QPointF(0,-height/2),QPointF(0,height/2)),p);
//    画坐标轴刻度值
    QFont font;
    font.setPixelSize(1);
    QTransform tran;
    QGraphicsSimpleTextItem* coord[100];
    for(int i=0;i<100;i++)
    {
        if(i%4!=0)  continue;
        coord[i] = this->addSimpleText(QString::number(i),font);
        coord[i]->setTransform(tran.scale(1,-1));//m_view->scale(1, -1);造成文本位置不正常
        coord[i]->setPos(i,-4);
        coord[i]->setBrush(QBrush(Qt::white,Qt::SolidPattern));
    }

    space = 50;
    min_space = 10;
    mode = ALL;
}

MyScene::~MyScene()
{

}

//如果此函数声明，但函数体为空，则场景是白色背景,不会调用 setBackgroundBrush
void MyScene::drawBackground(QPainter *painter, const QRectF &rect)
{
#if 1
    painter->save();

    double x = rect.x();
    double y = rect.y();

    QRectF newRect(rect);
    newRect.setTopLeft(QPointF(floor(x/space)*space,
                               floor(y/space)*space));

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
    qDebug()<<"draw background";
#endif
}
