#ifndef RING_H
#define RING_H

#include "baseitem.h"

class Ring : public BaseItem
{
public:
    explicit Ring(QGraphicsItem *parent = Q_NULLPTR);
    ~Ring();

    enum {Type = UserType + 4};

    void DrawShape(QPainter* painter);

private:
    enum{
        m_outer = 20,
        m_inner =10
    };
};

#endif // RING_H
