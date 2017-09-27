#include "myline.h"
#include <QMessageBox>
#include <QtMath>

//构造函数后直接调用paint函数,所有必须在构造函数初始化两个端点
MyLine::MyLine(QGraphicsItem *parent, QPointF src1,QPointF src3)
        :QGraphicsItem(parent),
        p1(src1),
        p3(src3)
{
    p2.setX((p1.x() + p3.x())/2);
    p2.setY((p1.y() + p3.y())/2);

    if(getSlope()>0)
    {
        QPointF b1(p1.x()-margin, p1.y()-margin);
        QPointF b2(p3.x()+margin, p3.y()+margin);
        setBoundingRect( QRectF(b1,b2) );
    }
    else
    {
        QPointF b1(p1.x()-margin, p1.y()+margin);
        QPointF b2(p3.x()+margin, p3.y()-margin);
        setBoundingRect( QRectF(b1,b2) );
    }
    this->setAcceptHoverEvents(true);
    this->setFlags(QGraphicsItem::ItemIsSelectable);
}

MyLine::MyLine(QGraphicsItem *parent, QPointF src,qreal length, qreal angle)
        :QGraphicsItem(parent),
        p1(src),
        length(length),
        angle(angle)
{
    this->setAcceptHoverEvents(true);
    this->setFlags(QGraphicsItem::ItemIsSelectable);
}

MyLine::~MyLine()
{

}

int MyLine::type() const
{
    return Type;
}

void MyLine::setView(MyView *v)
{
    m_view = v;
}

void MyLine::setBoundingRect(const QRectF &rect)
{
    if (m_rect == rect)
        return;
    prepareGeometryChange();
    m_rect = rect;
    m_boundingRect = QRectF();
    updateRect();
    update();
}

void MyLine::setBoundingRect(qreal x, qreal y, qreal w, qreal h)
{
    setBoundingRect(QRectF(x, y, w, h));
}

QRectF MyLine::boundingRect() const
{
    if (m_boundingRect.isNull())
        m_boundingRect = m_rect;
    return m_boundingRect;
}

QRectF MyLine::rect() const
{
    return m_rect;
}

QPointF MyLine::getP1() const
{
    return p1;
}

QPointF MyLine::getP2() const
{
    return p2;
}

QPointF MyLine::getP3() const
{
    return p3;
}

QPen MyLine::getSelectedPen()
{
//    选择时的边界矩形样式
    QPen pen;
    pen.setColor(Qt::white);
    pen.setStyle(Qt::DashLine);
    pen.setWidthF(0.2);

    QVector<qreal> dashes;
    dashes <<10<<10<<10<<10;    //个数应为偶数
    pen.setDashPattern(dashes);
    return pen;
}

QPen MyLine::getUnselectedPen()
{
    QColor c = this->scene()->backgroundBrush().color();
//    未选择时的边界矩形样式,颜色同场景的背景颜色
    QPen pen;
    pen.setColor(c);
    return pen;
}

QPen MyLine::getItemPen()
{
    QPen pen;
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(247, 160, 57));
    return pen;
}

void MyLine::setBoudingPainter(QPainter *painter)
{
    if(!this->isSelected())
        painter->setPen(getUnselectedPen());
    else
        painter->setPen(getSelectedPen());
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
}

void MyLine::setItemPainter(QPainter *painter)
{
    painter->setPen(getItemPen());
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
}

void MyLine::DrawBounding(QPainter *painter)
{
    painter->drawRect(m_rect);
    update();
}

void MyLine::DrawElli(QPainter *painter)
{
    painter->setPen(QColor(72, 61, 139 ));
    painter->setBrush(QColor( 72, 61, 139 ));
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    if(equalToOrigin(p3) && equalToOrigin(p2))
        p3 = m_view->getP3();
    p2.setX((p1.x() + p3.x())/2);
    p2.setY((p1.y() + p3.y())/2);

    painter->drawEllipse(p1.x()- size/2, p1.y()-size/2, size,size);
    painter->drawEllipse(p2.x()- size/2, p2.y()-size/2, size,size);
    painter->drawEllipse(p3.x()- size/2, p3.y()-size/2, size,size);
    QPointF b1, b2;
    if(getSlope()>0)
    {
        b1.setX( p1.x()-margin);    b1.setY(p1.y()-margin);
        b2.setX(p3.x()+margin);     b2.setY(p3.y()+margin);
        if( angle>90 && angle<270)
        {   // 225°时有问题
            b1.setX(p1.x()+margin); b1.setY(p1.y()+margin);
            b2.setX(p3.x()-margin); b2.setY(p3.y()-margin);
        }
    }
    else
    {
        b1.setX(p1.x()-margin); b1.setY(p1.y()+margin);
        b2.setX(p3.x()+margin); b2.setY(p3.y()-margin);
        if(angle>90 && angle<270)
        {
            b1.setX(p1.x()+margin); b1.setY(p1.y()-margin);
            b2.setX(p3.x()-margin); b2.setY(p3.y()+margin);
        }
    }
    setBoundingRect( QRectF(b1,b2) );

    update();
}

void MyLine::DrawLines(QPainter *painter)
{
    painter->setPen(QColor(Qt::white));
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    if(equalToOrigin(p3) && equalToOrigin(p2))
    {
        QLineF line;
        line.setP1(p1);
        line.setAngle(360-angle);
        line.setLength(length);
        painter->drawLine(line);
    }
    else
    {
        painter->drawLine(QLineF(p1,p2));
        painter->drawLine(QLineF(p2,p3));
    }
    update();
}

void MyLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    setBoudingPainter(painter);
    DrawBounding(painter);
    DrawElli(painter);
    DrawLines(painter);
}

void MyLine::updateRect()
{

}

bool MyLine::equalToOrigin(QPointF pt)
{
    bool flag;
    if(pt.x()==0 && pt.y()==0)
        flag = true;
    else
        flag = false;
    return flag;
}

qreal MyLine::getSlope()
{
    int slope = (p3.y()-p1.y()) / (p3.x()-p1.x());
    return slope;
}

void MyLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)  return;
    if(isSelected())
        this->setSelected(false);
    else
        this->setSelected(true);
    update();
    QGraphicsItem::mousePressEvent(event);
}

void MyLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void MyLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void MyLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void MyLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

void MyLine::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    qreal offsetX1 = event->pos().x()-p1.x();
    qreal offsetY1 = event->pos().y()-p1.y();
    qreal offsetX2 = event->pos().x()-p2.x();
    qreal offsetY2 = event->pos().y()-p2.y();
    qreal offsetX3 = event->pos().x()-p3.x();
    qreal offsetY3 = event->pos().y()-p3.y();
    //捕捉两个端点和中点,感应范围为点周围边长为2的矩形区域
    if(m_view->goCatch())
    {
        if( qAbs(offsetX1)< 1 || qAbs(offsetY1)<1)
            m_view->catchPt(p1);
        else if( qAbs(offsetX2)<1 || qAbs(offsetY2)<1)
            m_view->catchPt(p2);
        else if( qAbs(offsetX3)<1 || qAbs(offsetY3)<1)
            m_view->catchPt(p3);
    }
    QGraphicsItem::hoverMoveEvent(event);
}
