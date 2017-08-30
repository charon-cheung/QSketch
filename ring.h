#ifndef RING_H
#define RING_H

#include "baseitem.h"

class Ring : public BaseItem
{
public:
    explicit Ring(QGraphicsItem *parent = Q_NULLPTR);
    ~Ring();

    enum {Type = UserType + 4};
    enum {
        outer = 20,
        inner = 5
    };
//    QPointF pt;
//    QRectF m_rect;
//    mutable QRectF m_boundingRect;
//    double m_dScale;  // 缩放比例
//    bool m_hovered;
protected:
    void DrawShape(QPainter* painter);
    void DrawBounding(QPainter* painter);
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;
};

#endif // RING_H
