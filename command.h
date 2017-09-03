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

    enum Direction
    {
        UP,
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
    void ShowItemInfo();
    QString getItemInfo(QString type, QPointF pos, QSizeF size);
private:

    MyView* m_view;
    MyScene* m_scene;
    QList<QGraphicsItem*> chosenItems;
    QTransform m_translate;
    BrushDlg* brush_dlg;
    SelectDlg* select_dlg;
};

#endif // COMMAND_H
