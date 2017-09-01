#include "command.h"
#include <QMessageBox>
#include <QStatusBar>

Command::Command(MyScene *scene)
{
    m_scene = scene;
    chosenItems = m_scene->selectedItems();
//    if(chosenItems.size()==0)
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
    foreach (QGraphicsItem* item, all) {
        if(item->data(0).isNull())  //去掉场景初始化的5个图元
        {
            item->setSelected(state);
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
            info = getItemInfo(type, pos, size);
            break;
        }
        case QGraphicsEllipseItem::Type:
        {
            QGraphicsEllipseItem* Ell = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
            type = "椭圆";
            pos = Ell->rect().center();
            qDebug()<<"坐标:"<<Ell->scenePos();
            size = Ell->rect().size();
            info = getItemInfo(type, pos, size);
            break;
        }
        case CrossPt::Type :
        {
            type = "X形点";
            size = QSize(2,2);
            qDebug()<<type;
            info = getItemInfo(type, pos, size);
            break;
        }
        case CirclePt::Type :
        {
            type = "圆形点";
            size = QSize(2,2);
            info = getItemInfo(type, pos, size);
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
            info =  QString("图元类型: %1 \n").arg(type);
            info += QString("两个点的坐标: (%2 . %3) 和 (%4 . %5)      \n").arg(x1).arg(y1).arg(x2).arg(y2);
            info += QString("直线长度: %4   直线角度: %5").arg(length).arg(angle);
            break;
        }
        default:
            break;
        }
    }
    QMessageBox::information(0, "图元信息",info);
}

QString Command::getItemInfo(QString type, QPointF pos, QSizeF size)
{
    QString info;
    info = QString("图元类型: %1 \n").arg(type);
    info += QString("图元坐标: (%2 , %3)     \n").arg(pos.x()).arg(pos.y());
    info += QString("图元大小: %4 X %5").arg(size.width()).arg(size.height());
    return info;
}
