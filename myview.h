#ifndef MYVIEW_H
#define MYVIEW_H
#include <QGraphicsView>
#include <QMimeData>
#include <QMouseEvent>
#include <QGraphicsSimpleTextItem>
#include "posdialog.h"
#include "mainwindow.h"
#include "command.h"

class Command;
class MainWindow;
#define PI 3.1415926
class MyView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyView(QWidget *parent = 0);
    ~MyView();

    MyScene* getScene();
    void updateCenterRect();
    void setSaved(bool flag);
    bool IsSaved();
    void setNew(bool flag);
    bool IsNew();
    void SetMoveFlag(bool flag);
    void showStatus(QString msg);

private:
    MyScene* m_scene;
    PosDialog* dlg;
    Command* Cmd;
    MainWindow* m_main;
    QList<bool> drawFlags;
    bool drawPt, drawCirPt, drawCross, drawPtXY;
    bool drawLine, drawLineXY, drawLineAH;
    bool drawRect, drawRectXY, drawRounded;
    bool drawElli, drawElliXY, drawRing;
    bool drawText, drawSingle, drawMulti;
    QGraphicsLineItem* Line;
    QGraphicsSimpleTextItem* Text;
    QPointF start;
    QPointF end;
    QPointF mouseMovePos;
    enum AppMode
    {
        NORMAL,
        DRAG,
        EDIT
    };

    AppMode mode;
    QPointF dragBegin;
    QGraphicsItem *viewCenter;
    QPointF pastePos;
    bool m_copied, m_movable, m_saved, m_new;
    QTransform m_translate;
    QList<QGraphicsItem*> chosenItems;
    // 画笔样式
    Qt::PenStyle PenStyle;
    int PenWidth;
    QColor PenColor;
    QBrush PenBrush;
    QPen pen;
    QFont TextFont;
private:
    inline void test();
    void InitView();
    void InitBools();
    void InitViewRect();

    inline void changeCursor(const QString& shape);
    inline void changeCursor(Qt::CursorShape shape);

    int getPenWidth();
    Qt::PenStyle getPenStyle();
    QColor getColor();
    QBrush getBrush();
    QPen getPen();

    QString inputText(bool multi);
    QPointF getScenePos();
    void selectAll(bool state);
    void catchPt(QPointF pt);

public slots:
    void DrawPt();
    void DrawLine();
    void DrawRect();
    void DrawEllipse();
    void DrawText();

    QFont getFont();
    void ShowContextMenu();  //加右键菜单

    void setNormal();
    void Locate();      //重置原点
    void Reset();       //重置放缩倍数
    void SetMovable(bool state);
    void Copy();
    void Paste();
    void Delete();
    void Rotate(QPointF pt, float angle);
    void Redraw();
    void Translate(int direction);
    void Translate(QPointF pt);
    void showItemInfo();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // MYVIEW_H
