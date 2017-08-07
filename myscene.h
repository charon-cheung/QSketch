#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
class MyScene : public QGraphicsScene
{
public:
    MyScene(QObject *parent = Q_NULLPTR);

private:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

};

#endif // MYSCENE_H
