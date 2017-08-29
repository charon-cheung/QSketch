#ifndef MYVIEW_H
#define MYVIEW_H
#include <QGraphicsView>
#include <QMimeData>
#include <QMouseEvent>
#include <QGraphicsSimpleTextItem>
#include <QComboBox>
#include "myscene.h"
#include "posdialog.h"
#include "mainwindow.h"

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
        PACE =5
    };

    AppMode mode;
    QPointF dragBegin;
    QGraphicsItem *viewCenter;
    QPointF pastePos;
    bool m_copied;
    bool m_movable;
    bool m_saved, m_new;
    QTransform m_translate;
    QList<QGraphicsItem*> chosenItems;

    Qt::PenStyle PenStyle;
    int PenWidth;
    QColor PenColor;
    QBrush PenBrush;
    QPen pen;
private:
    inline void test();
    void Init();

    inline void changeCursor(const QString& shape);
    inline void changeCursor(Qt::CursorShape shape);
    Qt::PenStyle getPenStyle(QComboBox* Stylebox);
    int getPenWidth(QComboBox* Stylebox);

    QPointF getScenePos();
    void selectAll(bool state);
    inline void showStatus(QString msg);
    QString getItemInfo(QString type, QPointF pos, QSizeF size);

public slots:
    void setLine();
    void setPt();
    void setRect();
    void setEllipse();
    QPen getPen();
    void ShowContextMenu();  //加右键菜单
    void setNormal();
    void Locate();      //重置原点
    void Reset();       //重置放缩倍数
    void SetMovable(bool state);

    void Cut();
    void Copy();
    void Paste();
    void Delete();
    void Rotate();
    void Redraw();
    void Translate(int direction);
    void catchPt(QPointF pt);
    void showItemInfo();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

};

#endif // MYVIEW_H
