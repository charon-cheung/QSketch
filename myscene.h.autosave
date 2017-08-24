#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <crosspt.h>
#include <QPainter>
#include <QDataStream>

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyScene(QObject *parent = Q_NULLPTR);
    virtual ~MyScene();

    void InitScene();
    QPen getPen();

    QSet<QList<QGraphicsItem*> > set;   //不要写成>>，否则报错
    void Save(QDataStream& s);
    void Export(QDataStream& s,QList<QGraphicsItem*> items);

    void Load(QDataStream& s);
    void Import(QDataStream& s,int count);

private:
    QGraphicsLineItem* Line;
    QPen p;
    enum SIZE{
        width = 1200,
        height = 800
    };
    enum GridMode
    {
        ALL,
        MAJOR,
        NONE
    };
    GridMode mode;
    int space;
    int min_space;

    QGraphicsLineItem  *X, *Y;
    QGraphicsPolygonItem *ArrowX, *ArrowY;
    QGraphicsEllipseItem *Origin;
private:
    void setPen();

protected:
//    virtual void drawBackground(QPainter * painter, const QRectF & rect);

};

#endif // MYSCENE_H
