#include "circlept.h"

#include <QDebug>

CirclePt::CirclePt(QGraphicsItem *parent)
    :BaseItem(parent)
{
//    场景对y轴对称,所以不是(-5,5,10,10),取下左点
    setBoundingRect(-2*size, -2*size, 4*size,4*size);
}

CirclePt::~CirclePt()
{

}

int CirclePt::type() const
{
    return Type;
}


void CirclePt::DrawShape(QPainter *painter)
{
//    场景对y轴对称,取矩形的下左点,边界矩形也同样处理
    painter->drawEllipse(pt.x()- size/2, pt.y()-size/2, size,size);
}
