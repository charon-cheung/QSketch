#include "myscene.h"
#include<QDebug>
MyScene::MyScene(QObject *parent):
    QGraphicsScene(parent)
{

}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        qDebug()<<"mouse press scene";
//        this->addLine(QLineF(QPoint(0,0), QPoint(80,60)) );
    }
}
