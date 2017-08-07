#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "myview.h"
#include "myscene.h"

#include <QMainWindow>
#include <QGraphicsSceneMoveEvent>
#include <QGraphicsLineItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_DrawLine_clicked();


    void on_DrawPtPos_clicked();

private:
    Ui::MainWindow *ui;
    MyScene* m_scene;
    MyView* m_view;
    bool drawPt,drawLine;
    QPointF mapPressPt;

    QGraphicsLineItem* Line;
    enum{
        pt_size = 5,
        line_width = 5
    };
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseMoveEvent(QGraphicsSceneMoveEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

//    void paintEvent(QPaintEvent* e);

};

#endif // MAINWINDOW_H
