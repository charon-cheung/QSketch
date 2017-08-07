#include "myscene.h"
#include <QDebug>

MyScene::MyScene(QObject *parent):
    QGraphicsScene(parent)
{

}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        qDebug()<<"mouse press scene";
        Line = this->addLine(QLineF(QPointF(0,0), event->pos()) );
    }
}
