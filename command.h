#ifndef COMMAND_H
#define COMMAND_H

#include "myview.h"
#include "mainwindow.h"
#include "brushdlg.h"
#include "selectdlg.h"

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
        PACE =5
    };

public slots:
    void Delete();
    void Rotate(QPointF pt, float angle);
    void Translate(int direction);
    void Translate(QPointF pt);
    void Zoom(bool in);
    void SelectAll(bool state);
    void CatchPt();
    void FillBrush();
    void SetMovable(bool state);
    void Stretch();
    void changeStyle();
    void SetSymmetry(Qt::Axis axis);
    void ShowItemInfo();

    QString getItemInfo(QString type, QPointF pos, QSizeF size, QColor c);
private:

    MyView* m_view;
    MyScene* m_scene;
    QList<QGraphicsItem*> chosenItems;
    QTransform m_translate;
    BrushDlg* brush_dlg;
    SelectDlg* select_dlg;
};

#endif // COMMAND_H
