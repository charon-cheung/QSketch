#ifndef BASEITEM_H
#define BASEITEM_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>

class BaseItem : public QGraphicsItem
{
public:
    explicit BaseItem(QGraphicsItem *parent = Q_NULLPTR);
    virtual ~BaseItem();

    enum {Type = UserType + 1};
    int type() const;
    void setBoundingRect(const QRectF &rect);
    void setBoundingRect(qreal x, qreal y, qreal w, qreal h);
    QRectF boundingRect() const;
    QRectF rect() const;

    QPen getSelectedPen();
    QPen getUnselectedPen();
    QPen getItemPen();
    void setBoudingPainter(QPainter *painter);
    void setItemPainter(QPainter *painter);

    void DrawBounding(QPainter* painter);
    virtual void DrawShape(QPainter* painter);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    void updateRect();

protected:
    QPointF pt;
    QRectF m_rect;
    mutable QRectF m_boundingRect;
    double m_dScale;  // 缩放比例
    bool m_hovered;
    QPen m_pen;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) ;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) ;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) ;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif // BASEITEM_H
