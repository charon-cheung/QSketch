#ifndef CIRCLEPT_H
#define CIRCLEPT_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

class CirclePt : public QGraphicsItem
{
public:
    explicit CirclePt(QGraphicsItem *parent = Q_NULLPTR);
    explicit CirclePt(const QRectF &rect, QGraphicsItem *parent = Q_NULLPTR);
    explicit CirclePt(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = Q_NULLPTR);
    ~CirclePt();

    enum {Type = UserType + 2};
    int type() const;

    QRectF rect() const;
    void setBoundingRect(const QRectF &rect);
    inline void setBoundingRect(qreal x, qreal y, qreal w, qreal h);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

private:
    enum {
        pt_size = 2
    };
    QPointF pt;
    QRectF m_rect;
    mutable QRectF m_boundingRect;
    double m_dScale;  // 缩放比例

private:
    void updateRect();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CIRCLEPT_H
