#ifndef BASEITEM_H
#define BASEITEM_H

#include <QGraphicsItem>

class BaseItem : public QGraphicsItem
{
public:
    explicit BaseItem(QGraphicsItem *parent = Q_NULLPTR);
//    explicit BaseItem(const QRectF &rect, QGraphicsItem *parent = Q_NULLPTR);
//    explicit BaseItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = Q_NULLPTR);
    virtual ~BaseItem();

    enum {Type = UserType + 1};
    int type() const;

//    QRectF rect() const;
    void setBoundingRect(const QRectF &rect);
//    inline void setBoundingRect(qreal x, qreal y, qreal w, qreal h);
//    QRectF boundingRect() const;
private:


private:
    void updateRect();

protected:
    QPointF pt;
    QRectF m_rect;
    mutable QRectF m_boundingRect;
    double m_dScale;  // 缩放比例
    bool m_hovered;

    virtual void DrawShape(QPainter* painter);
    virtual void DrawBounding(QPainter* painter);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif // BASEITEM_H
