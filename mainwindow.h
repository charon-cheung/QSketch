#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myview.h"
#include "posdialog.h"
#include "QRecentFilesMenu.h"
#include <QMainWindow>

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

    void on_NewView_triggered();

    void on_Print_triggered();

    void on_tabView_tabCloseRequested(int index);

    void on_action_Current_triggered();

    void on_action_All_triggered();

    void on_action_Exit_triggered();

    void on_startBtn_clicked();

    void on_action_Pic_triggered();

    void on_action_Open_triggered();

    void on_action_New_triggered();

    void on_action_Save_triggered();

    void on_pushButton_3_clicked();

    void loadFile(const QString &name);

    void Modified();

signals:
    void getPt(QPointF pt1);
private:
    Ui::MainWindow *ui;
    QString dirPath;

//    MyScene* m_scene;
//    MyView* m_view;
    PosDialog* dlg;
    QRecentFilesMenu *recentFilesMenu;
    QMenu *ptMenu, *lineMenu, *rectMenu, *ellipseMenu;
    QList<QAction*> ptActions;
    QList<QAction*> lineActions;
    QList<QAction*> rectActions;
    QList<QAction*> ellipseActions;
    bool m_modified;
    QString modify_time;
private:
    void InitUi();
    void InitConnect(MyView* view);
    void InitDir();
    void ShowSaveBox();

protected:

};

#endif // MAINWINDOW_H
