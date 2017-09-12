#include "baseitem.h"

BaseItem::BaseItem(QGraphicsItem *parent)
    :QGraphicsItem(parent)
{
    m_hovered = false;
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsSelectable );
}

BaseItem::~BaseItem()
{

}

void BaseItem::setBoundingRect(const QRectF &rect)
{
    if (m_rect == rect)
        return;
    prepareGeometryChange();
    m_rect = rect;
    m_boundingRect = QRectF();
    updateRect();
    update();
}

void BaseItem::setBoundingRect(qreal x, qreal y, qreal w, qreal h)
{
    setBoundingRect(QRectF(x, y, w, h));
}

QRectF BaseItem::boundingRect() const
{
    if (m_boundingRect.isNull())
        m_boundingRect = m_rect;
    return m_boundingRect;
}

QRectF BaseItem::rect() const
{
    return m_rect;
}

QPen BaseItem::getSelectedPen()
{
//    选择时的边界矩形样式
    QPen pen;
    pen.setColor(QColor(255,185,15));
    pen.setStyle(Qt::DashLine);
    pen.setWidthF(0.2);

    QVector<qreal> dashes;
    dashes <<10<<10<<10<<10;    //个数应为偶数
    pen.setDashPattern(dashes);
    return pen;
}

QPen BaseItem::getUnselectedPen()
{
    QColor c = this->scene()->backgroundBrush().color();
//    未选择时的边界矩形样式,颜色同场景的背景颜色
    QPen pen;
    pen.setColor(c);
    return pen;
}

QPen BaseItem::getItemPen()
{
    QPen pen;
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(247, 160, 57));
    return pen;
}

void BaseItem::setBoudingPainter(QPainter *painter)
{
    if(!this->isSelected())
        painter->setPen(getUnselectedPen());
    else
        painter->setPen(getSelectedPen());
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
}

void BaseItem::setItemPainter(QPainter *painter)
{
    painter->setPen(getItemPen());
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
}

void BaseItem::updateRect()
{

}

void BaseItem::DrawShape(QPainter *painter)
{
    Q_UNUSED(painter);
}

void BaseItem::DrawBounding(QPainter *painter)
{
    Q_UNUSED(painter);
    painter->drawRect(m_rect);
}

void BaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);

    setBoudingPainter(painter);
    DrawBounding(painter);
    setItemPainter(painter);
    DrawShape(painter);
}

void BaseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)  return;
    if(isSelected())
        this->setSelected(false);
    else
        this->setSelected(true);
    update();
    QGraphicsItem::mousePressEvent(event);
}

void BaseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void BaseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void BaseItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void BaseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

void BaseItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsItem::hoverMoveEvent(event);
}
