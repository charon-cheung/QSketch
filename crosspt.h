#ifndef CROSSPT_H
#define CROSSPT_H
#include <QGraphicsItem>
#include <QPainter>
class CrossPt : public QGraphicsItem
{
public:
    explicit CrossPt(QGraphicsItem *parent = Q_NULLPTR);
    explicit CrossPt(const QRectF &rect, QGraphicsItem *parent = Q_NULLPTR);
    explicit CrossPt(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = Q_NULLPTR);
    ~CrossPt();

    QRectF rect() const;
    void setRect(const QRectF &rect);
    inline void setRect(qreal x, qreal y, qreal w, qreal h);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

private:
    enum {
        pt_size = 4
    };
    QPointF pt;
    QRectF m_rect;
    mutable QRectF m_boundingRect;
    double m_dScale;  // 缩放比例

private:
    void updateRect();
};

#endif // CROSSPT_H
