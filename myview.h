#ifndef MYVIEW_H
#define MYVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsItem>

#include "myscene.h"
#include "posdialog.h"
class MyView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyView(QWidget *parent = 0);
    ~MyView();
signals:
    void changeScale(qreal t);
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    MyScene* m_scene;
    QGraphicsLineItem* Line;
    PosDialog* dlg;
    bool drawPt, drawCirPt, drawCross, drawPtPos;
    bool drawLine, drawLinePos;
    enum{
        pt_size = 3,
        line_width = 5
    };
    QPointF start;
    QPointF end;
    QMatrix matrix;


    QGraphicsRectItem *viewCenter;
private:
    void catchPt(QPointF pt);

public slots:
    void setLine();
    void setPt();

};

#endif // MYVIEW_H
