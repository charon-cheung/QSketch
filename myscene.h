#ifndef MYSCENE_H
#define MYSCENE_H

#include "Items/baseitem.h"
#include "Items/circlept.h"
#include "Items/crosspt.h"
#include "Items/ring.h"
#include "Items/myline.h"
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>
#include <myview.h>

class MyView;     //需要前置声明
class MyScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum SIZE{
        width = 1200,
        height = 800
    };
    enum GridMode
    {
        POINT,
        GRID,
        NONE
    };
    GridMode mode;

public:
    explicit MyScene(QObject *parent = Q_NULLPTR);
    virtual ~MyScene();

    void InitText();
    void InitData();
    void InitShape();
    void InitScene();
    //    QSet<QList<QGraphicsItem*> > set;   //不要写成>>，否则报错
    void Save(QDataStream& s);
    void Export(QDataStream& s,QList<QGraphicsItem*> items);
    bool Load(QDataStream& s);
    void Import(QDataStream& s,int count);
    void setMode(GridMode m);
    void setDraftMode(bool on);
    void selectPen(QPen p);
    QRectF itemsBoundingRect() const;
    MyView* getCurrentView();
private:
    enum{
        min_space = 20,
        space = min_space * 5
    };
    QPen pen;
    QGraphicsLineItem  *AxisX, *AxisY;
    QGraphicsPolygonItem *ArrowX, *ArrowY;
    QGraphicsEllipseItem *Origin;
    QGraphicsSimpleTextItem* X, *Y;
    bool m_draft,m_pressed;
    qreal x,y;
    QGraphicsItem* tempItem;
    MyView* view;

protected:
    void drawBackground(QPainter * painter, const QRectF & rect);
    void mousePressEvent(QGraphicsSceneMouseEvent  *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYSCENE_H
