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
    ~MyScene();
private:
    QGraphicsLineItem* Line;
    QPointF start;
    QPointF end;
    enum{
        width = 600,
        height = 400
    };
protected:


};

#endif // MYSCENE_H
