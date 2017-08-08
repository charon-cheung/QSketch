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


private:
    Ui::MainWindow *ui;
    MyScene* m_scene;
    MyView* m_view;
    QMenu *ptMenu, *LineMenu;
    QList<QAction*> ptActions;
    QList<QAction*> lineActions;
    QGraphicsLineItem* Line;
private:
    void InitUi();
protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent* e);
//    void mouseMoveEvent(QGraphicsSceneMoveEvent* e);
//    void mouseReleaseEvent(QMouseEvent* e);
};

#endif // MAINWINDOW_H
