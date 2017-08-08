#ifndef MYVIEW_H
#define MYVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsItem>

#include "myscene.h"
class MyView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyView(QWidget *parent = 0);
    ~MyView();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    MyScene* m_scene;
    QGraphicsLineItem* Line;
    bool drawPt, drawCirPt, drawCross, drawPtPos;
    bool drawLine, drawLinePos;
    enum{
        pt_size = 5,
        line_width = 5
    };
    QPointF start;
    QPointF end;
public slots:
    void setLine();
    void setPt();
};

#endif // MYVIEW_H
