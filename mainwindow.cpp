#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QFileDialog>
#include <QFile>
#include <QtPrintSupport/QPrinter>
#include <QPagedPaintDevice>
#include <QMessageBox>
#include <QSettings>
#include <QFileSystemWatcher>
#include <QDateTime>
#include <QDate>
#include <QColorDialog>
#include <QFontDialog>
#include <QSvgGenerator>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitUi();
    CreateActions();
    CreateMenus();
    CreateDir();
//    ui->centralWidget->setMouseTracking(true);
//    this->setMouseTracking(true);   //鼠标不按下的移动也能捕捉到MouseMoveEvent
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue("recentFiles", recentFilesMenu->saveState());
    delete ui;
}

int MainWindow::getPenWidth()
{
    switch(ui->PenWidth->currentIndex())
    {
    case 0:
        PenWidth = 1;
        break;
    case 1:
        PenWidth = 2;
        break;
    case 2:
        PenWidth = 3;
        break;
    case 3:
        PenWidth = 4;
        break;
    case 4:
        PenWidth = 5;
        break;
    default:
        PenWidth = 1;
        break;
    }
    return PenWidth;
}

Qt::PenStyle MainWindow::getPenStyle()
{
    switch(ui->PenStyle->currentIndex())
    {
    case 0:
        PenStyle = Qt::SolidLine;
        break;
    case 1:
        PenStyle = Qt::DashLine;
        break;
    case 2:
        PenStyle = Qt::DotLine;
        break;
    case 3:
        PenStyle = Qt::DashDotLine;
        break;
    case 4:
        PenStyle = Qt::DashDotDotLine;
        break;
    default:
        PenStyle = Qt::SolidLine;
        break;
    }
    return PenStyle;
}

QColor MainWindow::getPenColor()
{
    return PenColor;
}

QBrush MainWindow::getPenBrush()
{
    return PenBrush;
}

QFont MainWindow::getFont()
{
    return TextFont;
}

QFont MainWindow::btnFont(int size)
{
    QFont f;
    f.setFamily("Inconsolata");
    f.setPointSize(size);
    return f;
}

void MainWindow::showScale(QString s)
{
    scale->setText(s);
}
//显示光标所在的场景坐标
void MainWindow::showCoordinate(QString s)
{
    MousePos->setText(s);
}

void MainWindow::DraftStatusBar(bool on)
{
    CatchMode->setDisabled(on);
    SceneMode->setDisabled(on);
    scale->setDisabled(on);
    if(on)
        ui->statusBar->showMessage("进入草图模式");
    else
        ui->statusBar->showMessage("退出草图模式");
}

void MainWindow::InitWorkWidgets(bool on)
{
    CatchMode->setVisible(on);
    SceneMode->setVisible(on);
    DraftMode->setVisible(on);
    scale->setVisible(on);
    ui->tabWidget->setVisible(on);
    ui->mainToolBar->setVisible(on);
    floatToolBar->setVisible(on);

    ui->action_Save->setEnabled(on);
    ui->menu_SaveAs->setEnabled(on);
    ui->action_Publish->setEnabled(on);
    ui->action_Print->setEnabled(on);

    ui->cmdMenu->setEnabled(on);
    ui->editMenu->setEnabled(on);
    ui->viewMenu->setEnabled(on);
    ui->insertMenu->setEnabled(on);
}

void MainWindow::InitUi()
{
    this->setWindowTitle("QSketch");
    this->setWindowIcon(QIcon(":/Icon/Icon/Draw.png"));

    ui->tabView->setTabsClosable(true);
    ui->tabView->setCurrentIndex(0);
    ui->tabWidget->setCurrentIndex(0);

    QStringList PenStyles;
    PenStyles<< "实线" <<"虚线"<< "点线" << "点划线" << "点点划线";
    ui->PenStyle->insertItems(0,PenStyles);

    QStringList PenWidths;
    PenWidths<< QString::number(1) << QString::number(2) << QString::number(3) << QString::number(4) << QString::number(5);
    ui->PenWidth->insertItems(0,PenWidths);

    CreateStatusBar();
    CreateToolBar();
    InitWorkWidgets(false);
    //    CreateCorner();
}

void MainWindow::CreateActions()
{
    ui->mainToolBar->addAction(ui->NewView);
    ui->mainToolBar->addAction(ui->Open);
    ui->mainToolBar->addAction(ui->Save);
    ui->mainToolBar->addAction(ui->Print);
    ui->mainToolBar->addSeparator();
}

void MainWindow::CreateStatusBar()
{
    //无法设置QLabel QPushButton的大小,暂时通过设置字体来控制
    scale = new QLabel(this);
    scale->setFont(btnFont(14));
    scale->show();
    ui->statusBar->addPermanentWidget(scale);

    MousePos = new QLabel(this);
    MousePos->setFont(btnFont(14));
    MousePos->show();
    ui->statusBar->addPermanentWidget(MousePos);

    SceneModes<<ui->showGridAct <<ui->showPtAct <<ui->resetSceneAct;
    QMenu* modes = new QMenu(this);
    modes->addActions(SceneModes);

    SceneMode = new QPushButton(this);
    SceneMode->setFont(btnFont(14));
    SceneMode->setText(" 场景模式");
    SceneMode->setMenu(modes);
    ui->statusBar->addPermanentWidget(SceneMode);

    CatchMode = new QCheckBox(this);
    CatchMode->setFont(btnFont(14));
    CatchMode->setText("捕捉模式");
    ui->statusBar->addPermanentWidget(CatchMode);

    DraftMode = new QCheckBox(this);
    DraftMode->setFont(btnFont(14));
    DraftMode->setText("草图模式");
    ui->statusBar->addPermanentWidget(DraftMode);

    ui->statusBar->setFont(btnFont(14));
    ui->statusBar->showMessage("初始化完成");
}

void MainWindow::CreateToolWidget(QPushButton* btn)
{
    btn->setFlat(true);
    btn->setIconSize(QSize(32,32));
    QString iconPath = tr(":/Icon/Icon/%1.png").arg(btn->objectName());
    btn->setIcon(QIcon(iconPath));
    btn->resize(32,32);
}

void MainWindow::CreateToolBar()
{
    Normal = new QPushButton(this);
    Normal->setObjectName("normal");
    Normal->setToolTip("恢复为普通模式");
    CreateToolWidget(Normal);

    AreaZoom = new QPushButton(this);
    AreaZoom->setObjectName("areazoom");
    AreaZoom->setToolTip("局部放大");
    CreateToolWidget(AreaZoom);

    SmartZoom = new QPushButton(this);
    SmartZoom->setObjectName("smartzoom");
    SmartZoom->setToolTip("智能放缩");
    CreateToolWidget(SmartZoom);

    Reset = new QPushButton(this);
    Reset->setObjectName("reset");
    Reset->setToolTip("重置视图");
    CreateToolWidget(Reset);

    Empty = new QPushButton(this);
    Empty->setObjectName("empty");
    Empty->setToolTip("清空");
    CreateToolWidget(Empty);

    floatToolBar = new QToolBar();
    //唯一直接改变widget间距的方法,也可以用添加布局的方式
    floatToolBar->setStyleSheet("QToolBar{spacing: 10px;} ");
    floatToolBar->addWidget(Normal);
    floatToolBar->addWidget(AreaZoom);
    floatToolBar->addWidget(SmartZoom);
    floatToolBar->addWidget(Reset);
    floatToolBar->addWidget(Empty);
    floatToolBar->setOrientation(Qt::Vertical);
    floatToolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea | Qt::TopToolBarArea);

    addToolBar(floatToolBar);
}

void MainWindow::CreateMenus()
{
    // 最近打开的文件记录
    QSettings settings;
    recentFilesMenu = new QRecentFilesMenu(tr("Recent Files"), ui->openMenu);
    recentFilesMenu->restoreState(settings.value("recentFiles").toByteArray());
    ui->openMenu->insertMenu(ui->action_Save, recentFilesMenu);
    connect(recentFilesMenu, SIGNAL(recentFileTriggered(const QString &)), this, SLOT(addRecentFile(QString)) );
    //    几种绘图
    QMenu* ptMenu = new QMenu(this);
    ptActions<< ui->act1 << ui->act2 << ui->act3 <<ui->dividePt;
    ptMenu->addActions(ptActions);
    ui->DrawPt->setMenu(ptMenu);

    QMenu* lineMenu = new QMenu(this);
    lineActions<< ui->actLine_1 << ui->actLine_2 << ui->actLine_3;
    lineMenu->addActions(lineActions);
    ui->DrawLine->setMenu(lineMenu);

    QMenu* rectMenu = new QMenu(this);
    rectActions<< ui->actRect_1<< ui->actRect_2;
    rectMenu->addActions(rectActions);
    ui->DrawRect->setMenu(rectMenu);

    QMenu* ellipseMenu = new QMenu(this);
    ellipseActions<< ui->actEllipse_1<< ui->actEllipse_2<<ui->ring;
    ellipseMenu->addActions(ellipseActions);
    ui->DrawEllipse->setMenu(ellipseMenu);

    QMenu* textMenu = new QMenu(this);
    textActions << ui->textAct << ui->multiTextAct;
    textMenu->addActions(textActions);
    ui->DrawText->setMenu(textMenu);
}

void MainWindow::CreateConnects(MyView* view)
{
    foreach(QAction* act, ptActions)
        connect(act, &QAction::triggered, view, &MyView::DrawPt);
    foreach(QAction* act, lineActions)
        connect(act, &QAction::triggered, view, &MyView::DrawLine );
    foreach(QAction* act, rectActions)
        connect(act, &QAction::triggered, view, &MyView::DrawRect );
    foreach(QAction* act, ellipseActions)
        connect(act, &QAction::triggered, view, &MyView::DrawEllipse );
    foreach(QAction* act, textActions)
        connect(act, &QAction::triggered, view, &MyView::DrawTexts );
    foreach (QAction* mode, SceneModes)
        connect(mode, &QAction::triggered, this, &MainWindow::SwitchSceneMode );

    connect(this, SIGNAL(toFont(QFont)), view, SLOT(getFont()) );
    connect(CatchMode, SIGNAL(toggled(bool)), view, SLOT(setCatch(bool)) );
    connect(DraftMode, SIGNAL(toggled(bool)), view, SLOT(setDraftMode(bool)) );

    connect(Normal, &QPushButton::clicked, view, &MyView::setNormalMode);
    connect(AreaZoom, SIGNAL(clicked(bool)), this, SLOT(on_areaZoom_triggered()) );
    connect(SmartZoom, SIGNAL(clicked(bool)), this, SLOT(on_smartZoomAct_triggered()) );
    connect(Reset, &QPushButton::clicked, view, &MyView::Reset);
    connect(Empty, &QPushButton::clicked, view, &MyView::Empty);
    connect(ui->insertPix_2, SIGNAL(clicked(bool)), this, SLOT(on_insertPix_triggered()) );
    connect(ui->insertWidget_2, SIGNAL(clicked(bool)), this, SLOT(on_insertWidget_triggered()) );
}

void MainWindow::CreateCorner()
{
    QMenu* m = new QMenu(this);
    m->addAction(ui->action_Open);
    m->addAction(ui->action_Exit);
    QPushButton* btn = new QPushButton(this);
    btn->resize(60,30);
    btn->setMenu(m);
    ui->menuBar->setCornerWidget(btn,Qt::TopLeftCorner);
}

void MainWindow::CreateDir()
{
    dirPath = QApplication::applicationDirPath();
    filePath = dirPath + "/Files";
    QDir dir(dirPath);
    if(!dir.entryList().contains("Files"))
        dir.mkdir("Files");
    if(!dir.entryList().contains("Images"))
        dir.mkdir("Images");
    return;
}

void MainWindow::ShowSaveBox()
{
    int index = ui->tabView->currentIndex();
    QWidget* w = ui->tabView->widget(index);

    QMessageBox msg;
    msg.setText(tr("      是否需要保存 ?"));
    QPushButton* Save = msg.addButton(tr("保存"), QMessageBox::ActionRole);
    QPushButton* NotSave = msg.addButton(tr("不保存"), QMessageBox::ActionRole);
    QPushButton* Cancel = msg.addButton(tr("取消"), QMessageBox::ActionRole);
    //    不显示最大化和最小化按钮,必须用两行完成
    msg.setWindowFlags(msg.windowFlags() & ~Qt::WindowMinMaxButtonsHint);
    msg.setWindowFlags(msg.windowFlags() | Qt::WindowStaysOnTopHint);
    msg.exec();

    if(msg.clickedButton() == Save)
    {
        this->on_Save_triggered();
        ui->tabView->removeTab(index);
    }
    else if (msg.clickedButton() == NotSave)
    {
        foreach(QObject *obj, w->children() )
            if(obj->inherits("QGraphicsView") )
                delete obj;
        ui->tabView->removeTab(index);
    }
    else if (msg.clickedButton() == Cancel)
        msg.close();
}

MyView *MainWindow::getCurrentView()
{
    MyView *view = qobject_cast<MyView*>(ui->tabView->currentWidget());
    if(!view)
        return NULL;
    else
        return view;
}

QString MainWindow::getCurrentTabName()
{
    if(!getCurrentView())
    {
        QMessageBox::warning(0,"出错了!","当前不存在画面");
        return "";
    }
    QString name = ui->tabView->tabText(ui->tabView->currentIndex());
    name.remove(".gph");
    return name;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    if(!getCurrentView())   return;

    QMenu m(this);
    m.addAction(ui->ToolBarCmd);
    m.addAction(ui->FloatBarCmd);

    if(ui->mainToolBar->isVisible())
        ui->ToolBarCmd->setIcon(QIcon(":/Icon/Icon/tick.png"));
    else
        ui->ToolBarCmd->setIcon(QIcon());

    if(floatToolBar->isVisible())
        ui->FloatBarCmd->setIcon(QIcon(":/Icon/Icon/tick.png"));
    else
        ui->FloatBarCmd->setIcon(QIcon());
    // 仅限小范围内出现
    if( event->pos().x()<this->width() && event->pos().y()< 2*ui->mainToolBar->height())
    m.exec(QCursor::pos());
}

void MainWindow::on_NewView_triggered()
{
    QString fullName = QFileDialog::getSaveFileName(this, tr("新建画面"),
                                                    dirPath+"/Files", tr("画面文件(*.gph)"));
    if(fullName.isEmpty())      return;
    QString name=fullName.remove(dirPath+"/Files/");

    MyView *newView = new MyView(this);
    newView->setObjectName(name);
    newView->setNew(true);
    newView->setFocus();    //获得焦点
    newView->updateCenterRect();     //改善坐标轴不清晰的问题,但补得不全
    int count = ui->tabView->count();
    for(int i =0;i<count;i++)
    {
        if(ui->tabView->tabText(i)==name)
            on_tabView_tabCloseRequested(i);
    }
    ui->tabView->addTab(newView,QIcon(":/Icon/Icon/gph.png"),name);
    ui->tabView->setCurrentWidget(newView);

    CreateConnects(newView);
    InitWorkWidgets(true);
}

void MainWindow::on_Open_triggered()
{
    fullName=QFileDialog::getOpenFileName(this,"打开画面文件",dirPath+"/Files",tr("画面文件(*.gph)") );
    if(fullName.isEmpty())      return;
    QFile f(fullName);
    if(!f.open(QIODevice::ReadWrite)){
        QMessageBox::warning(0,"出错了!","文件读取失败:"+f.errorString());
        return ;
    }

    QString tabName = fullName.remove(dirPath+"/Files/");
    // 在关闭时应remove name,否则关闭后无法再打开
    if(TabNameList.contains(tabName))
    {
        QMessageBox::information(0,"注意!","此文件已经打开或者不是gph文件");
        return;
    }
    else
        TabNameList<<tabName;

    fullName = dirPath+"/Files/"+tabName;
    addRecentFile(tabName);

    QDataStream ds(&f);
    MyView *openView = new MyView(this);
    //MyView已经包含了一个MyScene对象,不能再定义一个对象,否则打开的是另一个场景,无法编辑
    if(!openView->getScene()->Load(ds))
        return;

    ui->tabView->addTab(openView,QIcon(":/Icon/Icon/gph.png"),tabName);
    ui->tabView->setCurrentWidget(openView);
    ui->tabView->currentWidget()->setObjectName(tabName);
    ui->tabView->setTabToolTip(ui->tabView->currentIndex(),ModifiedTime);

    CreateConnects(openView);

    QFileSystemWatcher *watcher = new QFileSystemWatcher(this);
    watcher->addPath(dirPath+"/Files/"+fullName);
    //    先修改再保存，才能知道有没有修改
    //    connect(watcher, SIGNAL(fileChanged(QString)), this,SLOT(Modified()) );
    InitWorkWidgets(true);
    ui->statusBar->showMessage("加载文件完成");
}

void MainWindow::on_Save_triggered()
{
    int index = ui->tabView->currentIndex();
    QString tabName = ui->tabView->tabText(index);
    if(!getCurrentView())   return;

    QDir d(dirPath+"/Files");

    QString fullName=dirPath+"/Files/"+tabName;
    QFile f(fullName);
    if(!f.open(QIODevice::WriteOnly)){
        QMessageBox::warning(0,"出错了!","文件保存失败:"+f.errorString());
        return;
    }

    QDataStream ds(&f);
    MyView* view = getCurrentView();
    view->getScene()->Save(ds);
    f.close();
    view->setSaved(true);
    return;
}

void MainWindow::on_Print_triggered()
{
    if(!getCurrentView())   return;
    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);
    printer.setPageOrientation(QPageLayout::Landscape);     //横向
//    printer.setColorMode(QPrinter::Color);    //不设置彩色打印时,加这两句会无法打印
//    printer.setOutputFormat(QPrinter::PdfFormat);
    QPagedPaintDevice::Margins m;
    m.bottom=10;
    m.top=10;
    m.left=10;
    m.right=10;
    printer.setMargins(m);
    //不管是否提前对y轴对称,得到的结果总是y轴向下
    QPainter painter(&printer);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    getCurrentView()->getScene()->render(&painter);
}

void MainWindow::on_tabView_tabCloseRequested(int index)
{
    QString name = ui->tabView->currentWidget()->objectName();
    QWidget* w = ui->tabView->widget(index);
    if(!getCurrentView())   return;
    TabNameList.removeOne(name);

    if(getCurrentView()->IsSaved())
    {
        foreach(QObject *obj, w->children() )
            if(obj->inherits("QGraphicsView") )
                delete obj;
        ui->tabView->removeTab(index);
    }
    else
        ShowSaveBox();
    if(ui->tabView->currentIndex()==0)
        InitWorkWidgets(false);
}

void MainWindow::on_action_Current_triggered()
{
    int index = ui->tabView->currentIndex();
    if(index==0)    return;
    on_tabView_tabCloseRequested(index);
}

void MainWindow::on_action_All_triggered()
{
    int count=ui->tabView->count();
    for(int i=count-1;i>0;i--)
        on_tabView_tabCloseRequested(i);
    InitWorkWidgets(false);
}

void MainWindow::on_action_Exit_triggered()
{
    qApp->quit();
}

void MainWindow::on_startBtn_clicked()
{
    QDir dir(filePath);
    if(dir.entryList().contains("画面1.gph"))
    {
        QMessageBox::warning(0,"文件已经存在","请直接打开 画面1.gph");
        return;
    }
    else
    {
        newView = new MyView(this);
        newView->setNew(true);
        newView->setFocus();    //获得焦点
//        newView->setMatrix(QMatrix(1,0,0,-1,0,0));
//        newView->scale(1,-1);   // 翻转y轴,默认y轴正方向指向下方
        newView->updateCenterRect();
        ui->tabView->addTab(newView,QIcon(":/Icon/Icon/gph.png"),"画面1.gph");
        ui->tabView->setCurrentWidget(newView);

        CreateConnects(newView);
    }
    InitWorkWidgets(true);
}

void MainWindow::on_action_Open_triggered()
{
    on_Open_triggered();
}

void MainWindow::on_action_New_triggered()
{
    on_NewView_triggered();
}

void MainWindow::on_action_Save_triggered()
{
    on_Save_triggered();
}

void MainWindow::on_openBtn_clicked()
{
    on_action_Open_triggered();
}

void MainWindow::addRecentFile(const QString &fileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::restoreOverrideCursor();
    recentFilesMenu->addRecentFile(fileName);

    QFileInfo info;
    info.setFile(fullName);
    ModifiedTime = info.lastModified().toString("上次修改时间: MM-dd hh:mm:ss");
}

void MainWindow::on_ColorPicker_clicked()
{
    PenColor = QColorDialog::getColor(Qt::white);
}

void MainWindow::on_BrushPicker_clicked()
{
    PenBrush.setColor(QColorDialog::getColor(Qt::white));
    PenBrush.setStyle(Qt::SolidPattern);
}

void MainWindow::on_FontPicker_clicked()
{
    bool ok;
    TextFont = QFontDialog::getFont(&ok, QFont("Inconsolata", 12), 0);
//    a comma-separated list of the attributes,suited for use in QSettings.
    emit toFont(TextFont);
}

void MainWindow::on_translateAct_triggered()
{
    if(!getCurrentView())   return;
    dlg = new PosDialog(this);
    dlg->showPt();
    if(dlg->exec() != QDialog::Accepted)    return;
    QPointF pt = dlg->getPt();

    Cmd = new Command(getCurrentView());
    Cmd->Translate(pt);
}

void MainWindow::on_rotateAct_triggered()
{
    if(!getCurrentView())   return;
    double angle = QInputDialog::getDouble(0, tr("请输入旋转角度"),"",90,-360,360);
    Cmd = new Command(getCurrentView());
    Cmd->Rotate(angle);
}

void MainWindow::on_changeStyleAct_triggered()
{
    if(!getCurrentView())   return;
    Cmd = new Command(getCurrentView());
    Cmd->changeStyle();
}

void MainWindow::on_ResetStyle_clicked()
{
    PenWidth = 1;
    PenStyle = Qt::SolidLine;
    PenColor = Qt::white;
    PenBrush.setColor(Qt::white);
    PenBrush.setStyle(Qt::NoBrush);
    ui->PenWidth->setCurrentText("1");
    ui->PenStyle->setCurrentText("实线");
}

void MainWindow::on_brushAct_triggered()
{
    if(!getCurrentView())   return;
    Cmd = new Command(getCurrentView());
    Cmd->FillBrush();
}

void MainWindow::on_zoomIn_triggered()
{
    if(!getCurrentView())   return;
    Cmd = new Command(getCurrentView());
    Cmd->Zoom(true);
}

void MainWindow::on_zoomOut_triggered()
{
    if(!getCurrentView())   return;
    Cmd = new Command(getCurrentView());
    Cmd->Zoom(false);
}

void MainWindow::on_infoAct_triggered()
{
    if(!getCurrentView())   return;
    getCurrentView()->showItemInfo();
}

void MainWindow::on_StretchAct_triggered()
{
    if(!getCurrentView())   return;
    Cmd = new Command(getCurrentView());
    Cmd->Stretch();
}

void MainWindow::on_action_PDF_triggered()
{
    if(!getCurrentView())   return;
    QPrinter printer( QPrinter::HighResolution );
    printer.setPageSize( QPrinter::A4 );
    printer.setOrientation( QPrinter::Landscape );
    printer.setOutputFormat( QPrinter::PdfFormat );
    //    file will be created in build directory
    printer.setOutputFileName( getCurrentTabName()+".pdf" );

    QPainter p;
    if( !p.begin( &printer ) )
    {
        QMessageBox::warning(0,"出错了!","另存PDF时出错");
        return;
    }
    //不管是否提前对y轴对称,得到的结果总是y轴向下 ???
    //    getCurrentView()->scale(1,-1);
    getCurrentView()->getScene()->render(&p);
    p.end();
    ui->statusBar->showMessage("画面保存为PDF文件");
}

void MainWindow::on_actionX_triggered()
{
    if(!getCurrentView())   return;
    Cmd = new Command(getCurrentView());
    Cmd->SetSymmetry(Qt::XAxis);
}

void MainWindow::on_actionY_triggered()
{
    if(!getCurrentView())   return;
    Cmd = new Command(getCurrentView());
    Cmd->SetSymmetry(Qt::YAxis);
}

void MainWindow::SwitchSceneMode()
{
    if(!getCurrentView())   return;
    if(sender()->objectName()=="showGridAct")
        //不要写成MyScene::GridMode::GRID,有的编译器报错
        getCurrentView()->getScene()->setMode(MyScene::GRID);
    else if(sender()->objectName()=="showPtAct")
        getCurrentView()->getScene()->setMode(MyScene::POINT);
    else if(sender()->objectName()=="resetSceneAct")
        getCurrentView()->getScene()->setMode(MyScene::NONE);
}

void MainWindow::showFullView(bool full)
{
    if(!getCurrentView())   return;
    if(full)
    {
        ui->tabWidget->setVisible(false);
        ui->menuBar->setVisible(false);
        ui->mainToolBar->setVisible(false);
        floatToolBar->setVisible(false);
        ui->statusBar->setVisible(false);
    }
    else
    {
        ui->tabWidget->setVisible(true);
        ui->menuBar->setVisible(true);
        ui->mainToolBar->setVisible(true);
        floatToolBar->setVisible(true);
        ui->statusBar->setVisible(true);
        ui->statusBar->showMessage("退出全屏模式");
    }
}

void MainWindow::on_lineAngle_triggered()
{
    if(!getCurrentView())   return;
    Cmd = new Command(getCurrentView());
    qreal angle = Cmd->getLinesAngle();
    QString a = QString::number(angle,'f',2);

    if(angle==360)  return;
    if(angle==0)
        QMessageBox::information(0,"直线夹角",QString("两直线平行"));
    else if(angle==90)
        QMessageBox::information(0,"直线夹角",QString("两直线垂直"));
    else
        QMessageBox::information(0,"直线夹角",QString("两直线所成角度为%1°").arg(a));
}

void MainWindow::on_fullViewAct_triggered()
{
    if(!getCurrentView())   return;
    showFullView(true);
}

void MainWindow::on_smartZoomAct_triggered()
{
    if(!getCurrentView())   return;
    Cmd = new Command(getCurrentView());
    Cmd->SmartZoom();
}

void MainWindow::on_areaZoom_triggered()
{
    if(!getCurrentView())   return;
    getCurrentView()->setZoomMode(true);
}

void MainWindow::on_cutAct_triggered()
{
    if(!getCurrentView())   return;
    getCurrentView()->Copy();
    getCurrentView()->Delete();
}

void MainWindow::on_copyAct_triggered()
{
    if(!getCurrentView())   return;
    getCurrentView()->Copy();
}

void MainWindow::on_pasteAct_triggered()
{
    if(!getCurrentView())   return;
    getCurrentView()->Paste();
}

void MainWindow::on_deleteAct_triggered()
{
    if(!getCurrentView())   return;
    getCurrentView()->Delete();
}

void MainWindow::on_insertPix_triggered()
{
    if(!getCurrentView())   return;
    Cmd = new Command(getCurrentView());
    Cmd->InsertPix();
}

void MainWindow::on_insertWidget_triggered()
{
    if(!getCurrentView())   return;
    //要插入的widget不能有parent,否则不成功
    QWidget* w = new QWidget();
    w->resize(200,140);
    Cmd = new Command(getCurrentView());
    Cmd->InsertWidget(w);
}

void MainWindow::on_SVG_triggered()
{
    if(!getCurrentView())   return;
    QString path = QApplication::applicationDirPath();
    QString file = path+ "/"+ getCurrentTabName()+".svg";

    QSvgGenerator svgGen;
    svgGen.setFileName( file );
    svgGen.setSize(QSize(MyScene::width, MyScene::height));
    svgGen.setViewBox(QRect(-MyScene::width/2,MyScene::height/2, MyScene::width, MyScene::height));
    svgGen.setTitle(tr("SVG Drawing"));
    svgGen.setDescription(tr("An SVG drawing generated by QSvgGenerator"));

    QPainter painter( &svgGen );
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    getCurrentView()->getScene()->render( &painter );
}

void MainWindow::on_Image_triggered()
{
    if(!getCurrentView())   return;
    QImage image(getCurrentView()->size(),QImage::Format_RGB32);
    QPainter painter(&image);
    getCurrentView()->getScene()->render(&painter);     //关键函数

    //    因为m_view->scale(6, -6);对纵坐标做了镜像处理，所以再倒过来
    QImage mirroredImage = image.mirrored(false, true);
    QString path = QApplication::applicationDirPath();
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("MM-dd--hh-mm-ss"); //设置显示格式
    QString file = path+ "/" +str+ ".png";
    mirroredImage.save(file);
}

void MainWindow::on_zoomInView_triggered()
{
    if(!getCurrentView())   return;
    getCurrentView()->scale(1.41421, 1.41421);
    getCurrentView()->updateCenterRect();
    QString t = "当前比例为 "+QString::number(getCurrentView()->matrix().m11(),'f',2)+":1 ";
    showScale(t);   //状态栏显示当前比例
}

void MainWindow::on_zoomOutView_triggered()
{
    if(!getCurrentView())   return;
    getCurrentView()->scale(0.70711, 0.70711);
    getCurrentView()->updateCenterRect();
    QString t = "当前比例为 "+QString::number(getCurrentView()->matrix().m11(),'f',2)+":1 ";
    showScale(t);   //状态栏显示当前比例
}

void MainWindow::on_Instruction_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/rjosodtssp/QSketch/blob/master/README.md", QUrl::TolerantMode));
}

void MainWindow::on_About_triggered()
{
    QMessageBox::about(0,"QSketch 1.0","QSketch是一个简易的2D绘图工具      \n\n"
                                       "开发环境:Qt 5.9.1 Graphics View框架\n\n"
                                       "编译器:MinGW 5.3 32bit for C++     \n\n");
}

void MainWindow::on_action_Publish_triggered()
{
    if(!getCurrentView())   return;
    QInputDialog dlg;
    QString address = dlg.getText(0, "请输入邮箱地址","",QLineEdit::Normal,"");

    if(address.isEmpty())   return;

}

void MainWindow::on_animation_triggered()
{
    if(!getCurrentView())   return;
    Cmd = new Command(getCurrentView());
    Cmd->Animation();
}

void MainWindow::on_ToolBarCmd_triggered()
{
    if(ui->mainToolBar->isVisible())
    {
        ui->mainToolBar->setVisible(false);
        ui->ToolBarCmd->setIcon(QIcon());
    }
    else
    {
        ui->mainToolBar->setVisible(true);
        ui->ToolBarCmd->setIcon(QIcon(":/Icon/Icon/tick.png"));
    }
}

void MainWindow::on_FloatBarCmd_triggered()
{
    if(floatToolBar->isVisible())
    {
        floatToolBar->setVisible(false);
        ui->FloatBarCmd->setIcon(QIcon());
    }
    else
    {
        floatToolBar->setVisible(true);
        ui->FloatBarCmd->setIcon(QIcon(":/Icon/Icon/tick.png"));
    }
}
