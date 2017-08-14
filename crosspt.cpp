#include "crosspt.h"
#include <QPen>
#include <QBrush>

CrossPt::CrossPt(QGraphicsItem *parent)
        :QGraphicsItem(parent)
{
    // 可选择、可移动
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

CrossPt::CrossPt(const QRectF &rect, QGraphicsItem *parent)
{
    setRect(rect);
}

CrossPt::CrossPt(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
{
    setRect(x,y,w,h);
}

CrossPt::~CrossPt()
{

}

QRectF CrossPt::rect() const
{
    return m_rect;
}

void CrossPt::setRect(qreal x, qreal y, qreal w, qreal h)
{
    setRect(QRectF(x, y, w, h));
}

void CrossPt::updateRect()
{

}


void CrossPt::setRect(const QRectF &rect)
{
    if (m_rect == rect)
        return;

    prepareGeometryChange();
    m_rect = rect;
    m_boundingRect = QRectF();
    updateRect();
    update();
}

QRectF CrossPt::boundingRect() const
{
    if (m_boundingRect.isNull())
        m_boundingRect = m_rect;

    return m_boundingRect;
}

void CrossPt::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // 反走样
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawEllipse(m_rect);

    QPen p;
    p.setWidth(1);
    p.setColor(QColor(0, 160, 230));
    p.setBrush(QColor(247, 160, 57));

    QPointF p1 = pt+QPoint(pt_size,pt_size);
    QPointF p2 = pt+QPoint(-pt_size,-pt_size);
    QPointF p3 = pt+QPoint(-pt_size,pt_size);
    QPointF p4 = pt+QPoint(pt_size,-pt_size);

    painter->setPen(p);
    painter->drawLine(QLineF(p1,p2));
    painter->drawLine(QLineF(p3,p4));
}
