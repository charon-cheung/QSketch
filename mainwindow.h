#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "command.h"
#include "myview.h"
#include "posdialog.h"
#include "QRecentFilesMenu.h"
#include <QMainWindow>
#include <QDockWidget>
#include <QQuickView>

namespace Ui {
class MainWindow;
}

class Command;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int getPenWidth();
    Qt::PenStyle getPenStyle();
    QColor getPenColor();
    QBrush getPenBrush();
    QFont  getFont();
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

    void LoadFile(const QString &name);

    void on_action_Reset_triggered();

    void on_action_Normal_triggered();

    void on_action_Redraw_triggered();

    void on_ColorPicker_clicked();

    void on_BrushPicker_clicked();

    void on_FontPicker_clicked();

    void on_translateAct_triggered();

    void on_rotateAct_triggered();

    void on_changeStyleAct_triggered();

    void on_ResetStyle_clicked();

    void on_movableAct_triggered();

    void on_brushAct_triggered();

    void on_zoomIn_triggered();

    void on_zoomOut_triggered();

    void on_deleteAct_triggered();

    void on_infoAct_triggered();

signals:
    void toFont(QFont f);
private:
    Ui::MainWindow *ui;
    QString dirPath,filePath;
    QString fullName;
    QString ModifiedTime;
    QStringList TabNameList;

    PosDialog* dlg;
    QRecentFilesMenu *recentFilesMenu;
    QMenu *ptMenu, *lineMenu, *rectMenu, *ellipseMenu;
    QList<QAction*> ptActions;
    QList<QAction*> lineActions;
    QList<QAction*> rectActions;
    QList<QAction*> ellipseActions;
    QList<QAction*> textActions;
    QDockWidget* dock;

    bool m_modified;
    MyView *newView;

    int PenWidth;
    Qt::PenStyle PenStyle;
    QColor PenColor;
    QBrush PenBrush;
    QFont TextFont;

    Command* Cmd;
private:
    void InitUi();
    void InitActions();
    void InitMenus();
    void InitConnects(MyView* view);
    void InitDir();
    void ShowSaveBox();
    MyView* getCurrentView();

protected:

};

#endif // MAINWINDOW_H
