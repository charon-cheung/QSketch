#ifndef COMMAND_H
#define COMMAND_H

#include "myview.h"
#include "Dialogs/brushdlg.h"
#include "Dialogs/selectdlg.h"
#include <QTimeLine>

class MyView;
class MyScene;
class MainWindow;
class Command
{

public:
    Command(MyScene* scene);
    Command(MyView* view);
    MyScene* getScene();

    enum TransDirection
    {
        UP,     //不能再有同名的枚举变量
        DOWN,
        LEFT,
        RIGHT,
        PACE = 8
    };
    enum{
        range = 2
    };

public slots:
    void Delete();
    void Rotate(float angle);
    void Translate(int direction);
    void Translate(QPointF pt);
    void PaceUp();
    void PaceDown();
    void Zoom(bool in);
    void SelectAll(bool state);
    void FillBrush();
    void SetMovable(bool state);
    void Stretch();
    void changeStyle();
    void SetSymmetry(Qt::Axis axis);
    void ShowItemInfo();
    void SmartZoom();

    QList<QPointF> getDividePts();
    void CatchPt();
    qreal getLinesAngle();

    void InsertPix();
    void InsertWidget(QWidget* w);
    void Animation();
    void test(qreal value);
private:
    MyView* m_view;
    MyScene* m_scene;
    QList<QGraphicsItem*> chosenItems;
    QTransform m_translate;
    int pace;
    bool m_up, m_down;
    BrushDlg* brush_dlg;
    SelectDlg* select_dlg;
    QTimeLine* timeLine;
    QString getItemInfo(QString type, QPointF pos, QSizeF size, QColor c);
    qreal getSlope(QGraphicsLineItem* line);
    bool inCatchRange(QPointF src, QPointF des);
};

#endif // COMMAND_H
