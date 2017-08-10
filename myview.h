#ifndef MYVIEW_H
#define MYVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsItem>

#include "myscene.h"
#include "posdialog.h"

#define PI 3.1415926
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
    bool drawPt, drawCirPt, drawCross, drawPtXY;
    bool drawLine, drawLineXY;
    enum PARAMETER{
        pt_size = 3,
        line_width = 5
    };
    QPointF start;
    QPointF end;
    QMatrix matrix;

    enum ViewMode
    {
        NORMAL,
        DRAG,
        EDIT
    };
    ViewMode mode;
    QPointF dragStart;
    QGraphicsItem *viewCenter;
private:
    void catchPt(QPointF pt);
    void updateCenterRect();

public slots:
    void setLine();
    void setPt();

};

#endif // MYVIEW_H
