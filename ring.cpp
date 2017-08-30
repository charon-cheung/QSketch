#include "ring.h"
#include <QPen>
#include <QDebug>

Ring::Ring(QGraphicsItem *parent)
    :BaseItem(parent)
{
    setBoundingRect(-outer, -outer, 2*outer,2*outer);
}

Ring::~Ring()
{

}

void Ring::DrawShape(QPainter *painter)
{
//    场景对y轴对称,取矩形的下左点,边界矩形也同样处理
    painter->drawEllipse(pt.x()- outer/2, pt.y()-outer/2, outer,outer);
    painter->drawEllipse(pt.x()- inner/2, pt.y()-inner/2, inner,inner);
}

void Ring::DrawBounding(QPainter *painter)
{
    painter->drawRect(m_rect);
}
