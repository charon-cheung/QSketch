#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsItem>
class MyScene : public QGraphicsScene
{
public:
    MyScene(QObject *parent = Q_NULLPTR);

private:
    QGraphicsLineItem* Line;
    QPointF start;
    QPointF end;
protected:


};

#endif // MYSCENE_H
