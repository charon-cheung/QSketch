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

int BaseItem::type() const
{

}

void BaseItem::setBoundingRect(const QRectF &rect)
{

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
}

void BaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
    DrawBounding(painter);
    DrawShape(painter);
}

void BaseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void BaseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void BaseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

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

void BaseItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

}
