#ifndef MYSCENE_H
#define MYSCENE_H

#include <baseitem.h>
#include <circlept.h>
#include <crosspt.h>
#include <ring.h>
#include <QDataStream>
//#include <myview.h>

class MyView;     //需要前置声明
class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum SIZE{
        width = 1200,
        height = 800
    };
public:
    explicit MyScene(QObject *parent = Q_NULLPTR);
    virtual ~MyScene();

    void InitScene();
//    QSet<QList<QGraphicsItem*> > set;   //不要写成>>，否则报错
    void Save(QDataStream& s);
    void Export(QDataStream& s,QList<QGraphicsItem*> items);
//    void ExportPen(QDataStream& s, QGraphicsItem* item);
    void Load(QDataStream& s);
    void Import(QDataStream& s,int count);

private:
//    QGraphicsLineItem* Line;

    enum GridMode
    {
        ALL,
        MAJOR,
        NONE
    };
    GridMode mode;
    int space;
    int min_space;

    QGraphicsLineItem  *AxisX, *AxisY;
    QGraphicsPolygonItem *ArrowX, *ArrowY;
    QGraphicsEllipseItem *Origin;
    QGraphicsSimpleTextItem* X, *Y;

private:

protected:
//    virtual void drawBackground(QPainter * painter, const QRectF & rect);

};

#endif // MYSCENE_H
