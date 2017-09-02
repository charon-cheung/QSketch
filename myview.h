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
    void catchPt(QPointF pt);

private:
    MyScene* m_scene;
    PosDialog* dlg;
    Command* Cmd;
    MainWindow* m_main;

    enum DrawFlag{
        drawNone =  0x0,
        drawCirPt = 0x1,
        drawCross = 0x2,
        drawPtXY = 0x4,
        drawLine = 0x8,
        drawLineXY = 0x10,
        drawLineAH = 0x20,
        drawRect = 0x40,
        drawRectXY = 0x80,
        drawElli = 0x100,
        drawElliXY = 0x200,
        drawRing = 0x400,
        drawText = 0x800
    };
    DrawFlag flag;
    bool m_drawMulti;

    QVector<QGraphicsLineItem*> LineVec;
    QVector<QGraphicsRectItem*> RectVec;
    QVector<QGraphicsEllipseItem*> ElliVec;
    int LineCount,RectCount,ElliCount;
//    QGraphicsLineItem* Line;

    QGraphicsSimpleTextItem* Text;
    QPointF StartPt;
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
    void InitParameters();
    void InitViewRect();

    inline void changeCursor(const QString& shape);
    inline void changeCursor(Qt::CursorShape shape);

    int getPenWidth();
    Qt::PenStyle getPenStyle();
    QColor getColor();
    QBrush getBrush();
    QPen getPen();

    QString inputMultiText(bool multi);
    QPointF getScenePos();
    void selectAll(bool state);

public slots:
    void DrawPt();
    void DrawLine();
    void DrawRect();
    void DrawEllipse();
    void DrawTexts();

    QFont getFont();
    void ShowContextMenu();  //加右键菜单

    void setNormal();
    void Locate();      //重置原点
    void Reset();       //重置放缩倍数
    void Zoom(bool in);
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
