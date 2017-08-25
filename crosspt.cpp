#include "crosspt.h"
#include <QPen>
#include <QBrush>
#include <QDebug>

CrossPt::CrossPt(QGraphicsItem *parent)
    :QGraphicsItem(parent)
{
//    可选择
    setFlags(QGraphicsItem::ItemIsSelectable );
}

CrossPt::CrossPt(const QRectF &rect, QGraphicsItem *parent)
{
    setBoundingRect(rect);
}

CrossPt::CrossPt(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
{
    setBoundingRect(x,y,w,h);
}

CrossPt::~CrossPt()
{

}

QRectF CrossPt::rect() const
{
    return m_rect;
}

void CrossPt::setBoundingRect(qreal x, qreal y, qreal w, qreal h)
{
    setBoundingRect(QRectF(x, y, w, h));
}

void CrossPt::updateRect()
{

}

void CrossPt::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)  return;
    if(isSelected())
        this->setSelected(false);
    else
        this->setSelected(true);
    update();
    QGraphicsItem::mousePressEvent(event);
}

void CrossPt::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void CrossPt::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    this->setSelected(false);
    QGraphicsItem::mouseReleaseEvent(event);
}

int CrossPt::type() const
{
    return Type;
}

void CrossPt::setBoundingRect(const QRectF &rect)
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

    QColor c = this->scene()->backgroundBrush().color();
    //    未选择时的边界矩形样式
    QPen pen_1;
    pen_1.setColor(c);
    //    选择时的边界矩形样式
    QPen pen_2;
    pen_2.setColor(Qt::white);
    pen_2.setStyle(Qt::DashLine);
    pen_2.setWidthF(0.1);

    QVector<qreal> dashes;
    dashes <<15<<15<<15<<15;    //个数应为偶数
    pen_2.setDashPattern(dashes);

    // 反走样,画边界矩形
    if(!this->isSelected())
        painter->setPen(pen_1);
    else
        painter->setPen(pen_2);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawRect(m_rect);
    //画两条线
    QPointF p1 = pt+QPoint(pt_size,pt_size);
    QPointF p2 = pt+QPoint(-pt_size,-pt_size);
    QPointF p3 = pt+QPoint(-pt_size,pt_size);
    QPointF p4 = pt+QPoint(pt_size,-pt_size);

    QPen p;
    p.setWidth(2);
    p.setColor(QColor(0, 160, 230));
    p.setBrush(QColor(247, 160, 57));

    painter->setPen(p);
    painter->drawLine(QLineF(p1,p2));
    painter->drawLine(QLineF(p3,p4));
}
