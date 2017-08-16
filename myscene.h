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
    QPen getPen();
    QList<QGraphicsItem*> getChosenItems();

public slots:
    void Delete();
    void SaveImage();

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
    QList<QGraphicsItem*> chosenItems;
    QGraphicsLineItem  *X,*Y;
    QGraphicsEllipseItem *Origin;
private:
    void setPen();

protected:
//    virtual void drawBackground(QPainter * painter, const QRectF & rect);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYSCENE_H
