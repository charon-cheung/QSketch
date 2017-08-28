#include "circlept.h"
#include <QPen>
#include <QBrush>
#include <QDebug>

CirclePt::CirclePt(QGraphicsItem *parent)
    :QGraphicsItem(parent)
{
//    可选择
    setFlags(QGraphicsItem::ItemIsSelectable );
}

CirclePt::CirclePt(const QRectF &rect, QGraphicsItem *parent)
{
    setBoundingRect(rect);
}

CirclePt::CirclePt(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
{
    setBoundingRect(x,y,w,h);
}

CirclePt::~CirclePt()
{

}

QRectF CirclePt::rect() const
{
    return m_rect;
}

void CirclePt::setBoundingRect(const QRectF &rect)
{
    if (m_rect == rect)
        return;

    prepareGeometryChange();
    m_rect = rect;
    m_boundingRect = QRectF();
    updateRect();
    update();
}

void CirclePt::setBoundingRect(qreal x, qreal y, qreal w, qreal h)
{
    setBoundingRect(QRectF(x, y, w, h));
}

void CirclePt::updateRect()
{

}

//三个函数如果不加父类的事件,无法实现拖动、Ctrl多选
void CirclePt::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)  return;
    if(isSelected())
        this->setSelected(false);
    else
        this->setSelected(true);
    update();
    QGraphicsItem::mousePressEvent(event);
}

void CirclePt::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void CirclePt::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    this->setSelected(false);
    QGraphicsItem::mouseReleaseEvent(event);
}

int CirclePt::type() const
{
    return Type;
}

QRectF CirclePt::boundingRect() const
{
    if (m_boundingRect.isNull())
        m_boundingRect = m_rect;
    return m_boundingRect;
}

void CirclePt::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QColor c = this->scene()->backgroundBrush().color();
    //    未选择时的边界矩形样式,颜色同场景的背景颜色
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

    QPen p;
    p.setWidth(2);
    p.setColor(QColor(247, 160, 57));
    p.setBrush(QColor(247, 160, 57));

    painter->setPen(p);
//    场景对y轴对称,取矩形的下左点,边界矩形也同样处理
    painter->drawEllipse(pt.x()-pt_size/2,pt.y()-pt_size/2,pt_size,pt_size);
}
