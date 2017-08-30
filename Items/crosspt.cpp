#include "crosspt.h"

CrossPt::CrossPt(QGraphicsItem *parent)
    :BaseItem(parent)
{
    setBoundingRect(-2*size, -2*size, 4*size,4*size);
}

CrossPt::~CrossPt()
{

}

int CrossPt::type() const
{
    return Type;
}

void CrossPt::DrawShape(QPainter *painter)
{
    QPointF p1 = pt+QPoint(size,size);
    QPointF p2 = pt+QPoint(-size,-size);
    QPointF p3 = pt+QPoint(-size,size);
    QPointF p4 = pt+QPoint(size,-size);

    painter->drawLine(QLineF(p1,p2));
    painter->drawLine(QLineF(p3,p4));
}
