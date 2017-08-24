#ifndef MYVIEW_H
#define MYVIEW_H
#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsItem>
#include <QDebug>
#include <QDataStream>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>

#include "myscene.h"
#include "posdialog.h"

#define PI 3.1415926
class MyView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyView(QWidget *parent = 0);
    ~MyView();
    MyScene* getScene() const;
    void updateCenterRect();

private:
    MyScene* m_scene;
    QGraphicsLineItem* Line;
    PosDialog* dlg;
    bool drawPt, drawCirPt, drawCross, drawPtXY;
    bool drawLine, drawLineXY,drawLineAH;
    bool drawRect, drawRectXY,drawRounded;
    bool drawElli, drawElliXY;

    enum Parameter{
        pt_size = 3,
        line_width = 5
    };
    QPointF start;
    QPointF end;
    QPointF mouseMovePos;
    enum AppMode
    {
        NORMAL,
        DRAG,
        EDIT
    };
    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        PACE
    };

    AppMode mode;
    QPointF dragBegin;
    QGraphicsItem *viewCenter;
    QPointF pastePos;
    bool copied;
    QTransform m_translate;
    QList<QGraphicsItem*> chosenItems;
private:
    void test();
    void changeCursor(const QString& shape);
    void changeCursor(Qt::CursorShape shape);
    QPointF getScenePos();
    void selectAll();
public slots:
    void setLine();
    void setPt();
    void setRect();
    void setEllipse();

    void ShowContextMenu();  //加右键菜单
    void setNormal();
    void Locate();   //重置原点
    void Reset();   //重置放缩倍数
    void Cut();
    void Copy();
    void Paste();
    void Delete();
    void Redraw();
    void Translate(int direction);
    void catchPt(QPointF pt);
    void showInfo();
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // MYVIEW_H
