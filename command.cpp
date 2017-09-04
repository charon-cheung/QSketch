#include "command.h"
#include <QMessageBox>
#include <QStatusBar>
#include <QInputDialog>

Command::Command(MyScene *scene)
{
    m_scene = scene;
    chosenItems = m_scene->selectedItems();
}

Command::Command(MyView *view)
{
    m_view = view;
    m_scene = m_view->getScene();
    chosenItems = m_scene->selectedItems();
}

MyScene *Command::getScene()
{
    MyScene* scene = qobject_cast<MyScene*>(m_view->scene());
    return scene;
}

void Command::Delete()
{
    foreach(QGraphicsItem* item, chosenItems)
    {
        if(!item)
        {
            QMessageBox::warning(0,"删除失败","图元不存在或不完整");
            return;
        }
        m_scene->removeItem(item);  //删除item及其子item
    }
}

void Command::Rotate(QPointF pt, float angle)
{
    foreach(QGraphicsItem* item, chosenItems)
    {
        qDebug()<<"旋转前的坐标:"<<item->scenePos();
//        绕场景坐标的原点旋转, 默认绕图元坐标的原点旋转
//        item->setTransformOriginPoint(item->mapFromScene(0,0));
        item->setTransformOriginPoint(item->mapFromScene(pt));
        item->setRotation(angle);
        qDebug()<<"旋转后的坐标:"<<item->scenePos();
    }
}

void Command::Translate(int direction)
{
    if(direction == UP)
        m_translate.translate(0, PACE);
    else if(direction == DOWN)
        m_translate.translate(0, -PACE);
    else if(direction == LEFT)
        m_translate.translate(-PACE, 0);
    else if(direction == RIGHT)
        m_translate.translate(PACE, 0);
    foreach(QGraphicsItem* item, chosenItems)
    {   // false则不能连续平移,只能平移到固定位置
        item->setTransform(m_translate,true);
    }
}

// 场景坐标是item->scenePos();不是item->mapToScene(item->pos());
//初始为(0,0),每向右平移，坐标就增加(5,0)
void Command::Translate(QPointF pt)
{
    foreach(QGraphicsItem* item, chosenItems)
    {
        QPointF movePt = pt - item->scenePos();
        m_translate.translate(movePt.x(), movePt.y());
        item->setTransform(m_translate);
    }
}

void Command::Zoom(bool in)
{
    qreal factor;
    foreach(QGraphicsItem* item, chosenItems)
    {
        factor = item->scale();
        if(in)
            item->setScale(1.414*factor);
        else
            item->setScale(0.707*factor);
    }
}

void Command::SelectAll(bool state)
{
    QList<QGraphicsItem*> all = m_scene->items(m_scene->sceneRect(),
                       Qt::IntersectsItemShape,Qt::AscendingOrder);
    foreach (QGraphicsItem* item, all)
    {
        //去掉场景初始化的5个图元
        if(item->data(0).isNull())
        {
            item->setSelected(state);
        }
    }
}

void Command::CatchPt()
{
    if(chosenItems.size()!=1)
    {
        QMessageBox::warning(0, "注意!","只能选择一个图元 !");
        return;
    }
    foreach(QGraphicsItem* item, chosenItems)
    {
        switch(item->type())
        {
            case QGraphicsLineItem::Type:
            {
                QGraphicsLineItem* L = qgraphicsitem_cast<QGraphicsLineItem*>(item);
                QPointF p1 = L->line().p1();
                m_view->catchPt(p1);
            }
            case QGraphicsRectItem::Type:
            {
                QGraphicsRectItem* R = qgraphicsitem_cast<QGraphicsRectItem*>(item);
                QPointF p1 = R->rect().center();
                m_view->catchPt(p1);
            }
            case QGraphicsEllipseItem::Type:
            {
                QGraphicsEllipseItem* E = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
                QPointF p1 = E->rect().center();
                m_view->catchPt(p1);
            }
        }
    }
}

void Command::FillBrush()
{
    brush_dlg = new BrushDlg();
    if(brush_dlg->exec() != QDialog::Accepted)    return;

    Qt::BrushStyle style = brush_dlg->getStyle();
    QColor color = brush_dlg->getColor();
    if(!color.isValid())
        color = Qt::white;
    QBrush brush;
    brush.setStyle(style);
    brush.setColor(color);

    foreach(QGraphicsItem* item, chosenItems)
    {
        switch(item->type())
        {
            case QGraphicsRectItem::Type:
            {
                QGraphicsRectItem* R = qgraphicsitem_cast<QGraphicsRectItem*>(item);
                R->setBrush(brush);
                break;      // 低级错误,经常忘了加 break
            }
            case QGraphicsEllipseItem::Type:
            {
                QGraphicsEllipseItem* E = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
                E->setBrush(brush);
                break;
            }
        }
    }
}

void Command::SetMovable(bool state)
{
    foreach(QGraphicsItem* item, chosenItems)
    {
        item->setFlag(QGraphicsItem::ItemIsMovable, state);
        if(item->flags()==( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable ))
        {
            m_view->SetMoveFlag(true);
            m_view->showStatus("当前所选图元已经可拖动");
        }
        else
        {
            m_view->SetMoveFlag(false);
            m_view->showStatus("当前所选图元无法再拖动");
        }
    }
}

void Command::Stretch()
{
    foreach(QGraphicsItem* item, chosenItems)
    {
        switch(item->type())
        {
        case QGraphicsLineItem::Type:
        {
            select_dlg = new SelectDlg();
            if(select_dlg->exec()!=QDialog::Accepted)   return;
            qreal len = select_dlg->getLength();
            int direction = select_dlg->getDirection();

            QGraphicsLineItem* L = qgraphicsitem_cast<QGraphicsLineItem*>(item);
            qreal angle = L->line().angle();
            QPointF p1 = L->line().p1();
            QPointF p2 = L->line().p2();

            QLineF line;
            line.setLength(len);

            if(direction== SelectDlg::Direction::start)
            {
                line.setP1(p1);
                line.setAngle(180+angle);
            }
            else if(direction== SelectDlg::Direction::end)
            {
                line.setP1(p2);
                line.setAngle(360+angle);
            }
            m_scene->addLine(line,QPen(Qt::white))->setFlag(QGraphicsItem::ItemIsSelectable);
            break;
        }
        }
    }
}

void Command::changeStyle()
{
    foreach(QGraphicsItem* item, chosenItems)
    {
        QAbstractGraphicsShapeItem* shape = qgraphicsitem_cast<QGraphicsRectItem*>(item);
        shape->setPen(m_view->getPen());
        shape->setBrush(m_view->getBrush());
    }
}

void Command::SetSymmetry(Qt::Axis axis)
{
    QTransform t;
    t.rotate(180, axis);
    foreach(QGraphicsItem* item, chosenItems)
        // combine为true,否则连续对称操作会出错,仍是以原始位置为准
        item->setTransform(t,true);
}

void Command::ShowItemInfo()
{
    if(chosenItems.size()!=1)
    {
        QMessageBox::warning(0, "注意!","只能选择一个图元 !");
        return;
    }
    QString type;
    QPointF pos;
    QSizeF size;
    QColor color;
    QString info;
    foreach (QGraphicsItem* item, chosenItems)
    {
        pos = item->scenePos();  //场景坐标,图元坐标是item->pos()
        switch(item->type())
        {
        case QGraphicsRectItem::Type:
        {
            QGraphicsRectItem* R = qgraphicsitem_cast<QGraphicsRectItem*>(item);
            type = "矩形";
            pos = R->rect().center();
            size = R->rect().size();
            color = R->pen().color();
            info = getItemInfo(type, pos, size, color);
            break;
        }
        case QGraphicsEllipseItem::Type:
        {
            QGraphicsEllipseItem* Ell = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
            type = "椭圆";
            pos = Ell->rect().center();
            qDebug()<<"坐标:"<<Ell->scenePos();
            size = Ell->rect().size();
            color = Ell->pen().color();
            info = getItemInfo(type, pos, size, color);
            break;
        }
        case CrossPt::Type :
        {
            type = "X形点";
            size = QSize(2,2);
            color = QColor(247, 160, 57);
            info = getItemInfo(type, pos, size, color);
            break;
        }
        case CirclePt::Type :
        {
            type = "圆形点";
            size = QSize(2,2);
            color = QColor(247, 160, 57);
            info = getItemInfo(type, pos, size, color);
            break;
        }
        case QGraphicsLineItem::Type :
        {
            QGraphicsLineItem* L = qgraphicsitem_cast<QGraphicsLineItem*>(item);
            qreal x1=L->line().p1().x();
            qreal y1=L->line().p1().y();
            qreal x2=L->line().p2().x();
            qreal y2=L->line().p2().y();
            qreal length = L->line().length();
            qreal angle = 360 - L->line().angle();
            if(angle > 360)   angle -= 360;

            type = "直线";
            color = L->pen().color();
            info =  QString("图元类型: %1 \n").arg(type);
            info += QString("两个点的坐标: (%2 . %3) 和 (%4 . %5)      \n").arg(x1).arg(y1).arg(x2).arg(y2);
            info += QString("直线长度: %4   直线角度: %5               \n").arg(length).arg(angle);
            info += QString("直线颜色: %6").arg(QVariant(color.toRgb()).toString());
            break;
        }
        default:
            break;
        }
    }
    QMessageBox::information(0, "图元信息",info);
}

QString Command::getItemInfo(QString type, QPointF pos, QSizeF size, QColor c)
{
    QString info;
    info = QString("图元类型: %1 \n").arg(type);
    info += QString("图元坐标: (%2 , %3)     \n").arg(pos.x()).arg(pos.y());
    info += QString("图元大小: %4 X %5       \n").arg(size.width()).arg(size.height());
    info += QString("图元颜色: %6").arg(QVariant(c.toRgb()).toString());
    return info;
}
