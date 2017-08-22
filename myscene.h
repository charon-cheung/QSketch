#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
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
    QList<QGraphicsItem*> getChosenItems();
    QSet<QList<QGraphicsItem*> > set;   //不要写成>>，否则报错
    void Save(QDataStream& s);
    void Load(QDataStream& s);
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

};

#endif // MYSCENE_H
