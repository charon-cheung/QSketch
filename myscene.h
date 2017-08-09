#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QPainter>

class MyScene : public QGraphicsScene
{
public:
    MyScene(QObject *parent = Q_NULLPTR);
    ~MyScene();
private:
    QGraphicsLineItem* Line;
    QPointF start;
    QPointF end;
    QPointF dragStart,dragEnd,dragTrans;
    enum{
        width = 600,
        height = 400
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
protected:
    virtual void drawBackground(QPainter * painter, const QRectF & rect);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

};

#endif // MYSCENE_H
