#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "command.h"
#include "Dialogs/posdialog.h"
#include "QRecentFilesMenu.h"
#include <QMainWindow>
#include <QDockWidget>
#include <QLabel>
#include <QCheckBox>

namespace Ui {
class MainWindow;
}

class Command;
class PosDialog;
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
    QFont btnFont(int size);
    void showScale(QString s);
    void DraftStatusBar(bool on);

public slots:

    void on_Open_triggered();

    void on_Save_triggered();

    void on_NewView_triggered();

    void on_Print_triggered();

    void on_tabView_tabCloseRequested(int index);

    void on_action_Current_triggered();

    void on_action_All_triggered();

    void on_action_Exit_triggered();

    void on_startBtn_clicked();

    void on_action_Open_triggered();

    void on_action_New_triggered();

    void on_action_Save_triggered();

    void on_openBtn_clicked();

    void addRecentFile(const QString &name);

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

    void on_StretchAct_triggered();

    void on_action_PDF_triggered();

    void on_actionX_triggered();

    void on_actionY_triggered();

    void SwitchSceneMode();

    void showFullView(bool full);
    void InitWorkWidgets(bool on);

signals:
    void toFont(QFont f);
private slots:
    void on_lineAngle_triggered();

    void on_fullViewAct_triggered();

    void on_zoomInView_triggered();

    void on_zoomOutView_triggered();

    void on_smartZoomAct_triggered();

    void on_areaZoom_triggered();

    void on_cutAct_triggered();

    void on_copyAct_triggered();

    void on_pasteAct_triggered();

    void on_insertPix_triggered();

    void on_insertWidget_triggered();

    void on_SVG_triggered();

    void on_Image_triggered();

    void on_Instruction_triggered();

    void on_About_triggered();

    void on_action_Publish_triggered();

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
    QList<QAction*> SceneModes;
    QDockWidget* dock;

    bool m_modified;
    MyView *newView;

    int PenWidth;
    Qt::PenStyle PenStyle;
    QColor PenColor;
    QBrush PenBrush;
    QFont TextFont;

    QLabel* scale;
    QPushButton *SceneMode, *Normal, *AreaZoom, *SmartZoom, *Reset, *Empty ;
    QCheckBox *CatchMode,*DraftMode;
    Command* Cmd;
    QToolBar *floatToolBar;

private:
    void InitUi();
    void CreateActions();
    void CreateStatusBar();
    void CreateToolWidget(QPushButton* btn);
    void CreateToolBar();
    void CreateMenus();
    void CreateConnects(MyView* view);
    void CreateCorner();
    void CreateDir();
    void ShowSaveBox();
    MyView* getCurrentView();
    QString getCurrentTabName();

};

#endif // MAINWINDOW_H
