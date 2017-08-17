#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "myview.h"

#include "posdialog.h"
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

    void on_Open_triggered();

    void on_Save_triggered();

    void on_SaveAs_triggered();

    void on_NewView_triggered();

    void on_Print_triggered();

    void on_tabView_tabCloseRequested(int index);

signals:
    void getPt(QPointF pt1);
private:
    Ui::MainWindow *ui;
    QString dirPath;

    MyScene* m_scene;
    MyView* m_view;
    PosDialog* dlg;

    QMenu *ptMenu, *lineMenu, *rectMenu, *ellipseMenu;
    QList<QAction*> ptActions;
    QList<QAction*> lineActions;
    QList<QAction*> rectActions;
    QList<QAction*> ellipseActions;
    QGraphicsLineItem* Line;
private:
    void InitUi();
    void InitView();
    void InitDir();
protected:

};

#endif // MAINWINDOW_H
