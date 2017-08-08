#include "myscene.h"
#include <QDebug>

MyScene::MyScene(QObject *parent):
    QGraphicsScene(parent)
{
    this->setSceneRect(-width/2,-height/2,width,height); //场景坐标系,超出view大小加滑条
    this->setBackgroundBrush(QBrush(Qt::black));
//    画圆心，QPen是圆的边缘，QBrush是圆的填充
    this->addEllipse(-5, -5, 2*5, 2*5, QPen(QColor(Qt::white)),
                     QBrush(Qt::white,Qt::SolidPattern));
    QPen p;
    p.setColor(QColor(Qt::white));
    p.setStyle(Qt::DashDotLine);
//    画两个坐标轴
    this->addLine(QLineF(QPointF(-300,0),QPointF(300,0)),p);
    this->addLine(QLineF(QPointF(0,-200),QPointF(0,200)),p);
//    画坐标轴刻度值
    QFont f;
    f.setPixelSize(24);
    QGraphicsSimpleTextItem* pos = this->addSimpleText("abcde",f);
    pos->setPos(100,100);   //m_view->scale(1, -1);

    pos->setBrush(QBrush(Qt::white,Qt::SolidPattern));
}
