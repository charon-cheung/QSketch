#include "ring.h"

Ring::Ring(QGraphicsItem *parent)
    :BaseItem(parent)
{
    setBoundingRect(-m_outer, -m_outer, 2*m_outer,2*m_outer);
}

Ring::~Ring()
{

}

int Ring::type() const
{
    return Type;
}

void Ring::DrawShape(QPainter *painter)
{
//    场景对y轴对称,取矩形的下左点,边界矩形也同样处理
    painter->drawEllipse(pt.x()- m_outer/2, pt.y()-m_outer/2, m_outer,m_outer);
    painter->drawEllipse(pt.x()- m_inner/2, pt.y()-m_inner/2, m_inner,m_inner);
}
