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
    this->addLine(QLineF(QPointF(-width/2,0), QPointF(width/2,0)),p);
    this->addLine(QLineF(QPointF(0,-height/2),QPointF(0,height/2)),p);
//    画坐标轴刻度值
    QFont font;
    font.setPixelSize(1);
    QTransform tran;
    QGraphicsSimpleTextItem* coord[100];
    for(int i=0;i<100;i++)
    {
//        if(i%4!=0)  continue;
        coord[i] = this->addSimpleText(QString::number(i),font);
        coord[i]->setTransform(tran.scale(1,-1));//m_view->scale(1, -1);造成文本位置不正常
        coord[i]->setPos(i,-4);
        coord[i]->setBrush(QBrush(Qt::white,Qt::SolidPattern));
    }
}

MyScene::~MyScene()
{

}
