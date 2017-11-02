#ifndef MYLINE_H
#define MYLINE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <myview.h>

class MyView;
//之前继承QGraphicsLineItem,导致无法接受鼠标事件,也无法选择和移动,为什么???
class MyLine : public QGraphicsItem
{
public:
    explicit MyLine(QGraphicsItem* parent,QPointF src1,QPointF src3);
    explicit MyLine(QGraphicsItem* parent,QPointF p1,qreal length, qreal angle);
    ~MyLine();

    enum { Type = UserType +4 };
    int type() const;
    void setView(MyView* v);
    void setBoundingRect(const QRectF &rect);
    void setBoundingRect(qreal x, qreal y, qreal w, qreal h);
    QRectF boundingRect() const;
    QRectF rect() const;
    QPointF getP1() const;
    QPointF getP2() const;
    QPointF getP3() const;
    qreal getLength();
    qreal getAngle();

    QPen getSelectedPen();
    QPen getUnselectedPen();
    QPen getItemPen();
    void setBoudingPainter(QPainter *painter);
    void setItemPainter(QPainter *painter);

    void DrawBounding(QPainter* painter);
    void DrawElli(QPainter* painter);
    void DrawLines(QPainter* painter);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    void updateRect();
    bool equalToOrigin(QPointF pt);
    qreal getSlope();
private:
    enum{
        size = 6,
        margin = 10
        };
    MyView* m_view;
    QPointF p1, p2, p3;
    qreal length,angle;
    QRectF m_rect;
    mutable QRectF m_boundingRect;
    bool m_hovered;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    // hover动作以boundingRect为准,没有boundingRect则无法hover
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // MYLINE_H
