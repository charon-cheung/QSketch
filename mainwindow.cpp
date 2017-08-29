#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QCursor>
//#include <QTransform>
#include <QDateTime>
#include <QDebug>
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitUi();
    InitActions();
    InitMenus();
    InitDir();
//    TabNameList.clear();
    m_modified = false;
//    ui->centralWidget->setMouseTracking(true);
//    this->setMouseTracking(true);   //鼠标不按下的移动也能捕捉到MouseMoveEvent
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue("recentFiles", recentFilesMenu->saveState());
    delete ui;
}

QColor MainWindow::getPenColor()
{
    return PenColor;
}

QBrush MainWindow::getPenBrush()
{
    return PenBrush;
}

void MainWindow::InitUi()
{
    this->setWindowTitle("QSketch");
    this->setWindowIcon(QIcon(":/Icon/Icon/cube.png"));

    ui->tabView->setTabsClosable(true);
    ui->tabView->setCurrentIndex(0);
    ui->tabWidget->setCurrentIndex(0);

    QStringList PenStyles;
    PenStyles<< "实线" <<"虚线"<< "点线" << "点划线" << "点点划线";
    ui->PenStyle->insertItems(0,PenStyles);

    QStringList PenWidths;
    PenWidths<< QString::number(1) << QString::number(2) << QString::number(3) << QString::number(4) << QString::number(5);
    ui->PenWidth->insertItems(0,PenWidths);
    ui->statusBar->showMessage("初始化完成");
}

void MainWindow::InitActions()
{
    ui->mainToolBar->addAction(ui->NewView);
    ui->mainToolBar->addAction(ui->Open);
    ui->mainToolBar->addAction(ui->Save);
    ui->mainToolBar->addAction(ui->Print);
    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addAction(ui->action_Normal);
    ui->mainToolBar->addAction(ui->action_Reset);
    ui->mainToolBar->addAction(ui->action_Redraw);
    ui->mainToolBar->addSeparator();
//    ui->mainToolBar->addWidget(ui->PenStyle);
}

void MainWindow::InitMenus()
{
    // 最近打开的文件记录
    QSettings settings;
    recentFilesMenu = new QRecentFilesMenu(tr("Recent Files"), ui->openMenu);
    recentFilesMenu->restoreState(settings.value("recentFiles").toByteArray());
    ui->openMenu->insertMenu(ui->action_Save, recentFilesMenu);
    connect(recentFilesMenu, SIGNAL(recentFileTriggered(const QString &)), this, SLOT(LoadFile(const QString &)));
//    几种绘图
    QMenu* ptMenu = new QMenu(this);
    ptActions<< ui->act1 << ui->act2 << ui->act3;
    ptMenu->addActions(ptActions);
    ui->DrawPt->setMenu(ptMenu);

    QMenu* lineMenu = new QMenu(this);
    lineActions<< ui->actLine_1 << ui->actLine_2 << ui->actLine_3;
    lineMenu->addActions(lineActions);
    ui->DrawLine->setMenu(lineMenu);

    QMenu* rectMenu = new QMenu(this);
    rectActions<< ui->actRect_1<<ui->actRect_2<<ui->actRect_3;
    rectMenu->addActions(rectActions);
    ui->DrawRect->setMenu(rectMenu);

    QMenu* ellipseMenu = new QMenu(this);
    ellipseActions<< ui->actEllipse_1<< ui->actEllipse_2;
    ellipseMenu->addActions(ellipseActions);
    ui->DrawEllipse->setMenu(ellipseMenu);
}

void MainWindow::InitConnect(MyView* view)
{
    foreach(QAction* act, ptActions)
        connect(act, &QAction::triggered, view, &MyView::setPt);
    foreach(QAction* act, lineActions)
        connect(act, &QAction::triggered, view, &MyView::setLine );
    foreach(QAction* act, rectActions)
        connect(act, &QAction::triggered, view, &MyView::setRect );
    foreach(QAction* act, ellipseActions)
        connect(act, &QAction::triggered, view, &MyView::setEllipse );

    connect(ui->PenStyle, SIGNAL(activated(QString)), view, SLOT(getPen()) );
    connect(ui->PenWidth, SIGNAL(activated(QString)), view, SLOT(getPen()) );
    connect(this, SIGNAL(toColor(QColor)), view, SLOT(getPen()) );
    connect(this, SIGNAL(toBrush(QBrush)), view, SLOT(getPen()) );

}

void MainWindow::InitDir()
{
    dirPath = QApplication::applicationDirPath();
    QDir dir(dirPath);
    if(!dir.entryList().contains("Files"))
    {
        dir.mkdir("Files");
    }
    if(!dir.entryList().contains("Images"))
    {
        dir.mkdir("Images");
    }
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
    // 坐标放大倍数,倍数为1时,1个单位坐标就是1个像素
    newView->scale(1,-1);
    newView->updateCenterRect();     //改善坐标轴不清晰的问题,但补得不全
    int count = ui->tabView->count();
    for(int i =0;i<count;i++)
    {
        if(ui->tabView->tabText(i)==name)
            on_tabView_tabCloseRequested(i);
    }
    ui->tabView->addTab(newView,QIcon(":/Icon/Icon/gph.png"),name);
    ui->tabView->setCurrentWidget(newView);

    InitConnect(newView);
}

void MainWindow::on_Open_triggered()
{
    QString fullName=QFileDialog::getOpenFileName(this,"打开画面文件",dirPath+"/Files",tr("画面文件(*.gph)") );
    if(fullName.isEmpty())      return;
    QFile f(fullName);
    QString name = fullName.remove(dirPath+"/Files/");
    if(TabNameList.contains(name))
        return;
    else
        TabNameList<<name;

    if(!f.open(QIODevice::ReadWrite)){
        qDebug()<<"画面文件读取失败："<<fullName;
        return ;
    }
    LoadFile(fullName);

    QDataStream ds(&f);
    MyView *openView = new MyView(this);
    //MyView已经包含了一个MyScene对象,不能再定义一个对象,否则打开的是另一个场景,无法编辑
    openView->getScene()->Load(ds);

    openView->scale(1,-1);
    ui->tabView->addTab(openView,QIcon(":/Icon/Icon/gph.png"),name);
    ui->tabView->setCurrentWidget(openView);
    ui->tabView->setTabToolTip(ui->tabView->currentIndex(),modify_time);

    InitConnect(openView);

    QFileSystemWatcher *watcher = new QFileSystemWatcher(this);
    watcher->addPath(dirPath+"/Files/"+fullName);
//    先修改再保存，才能知道有没有修改
//    connect(watcher, SIGNAL(fileChanged(QString)), this,SLOT(Modified()) );
    ui->statusBar->showMessage("加载文件完成");
}

void MainWindow::on_Save_triggered()
{
    int index = ui->tabView->currentIndex();
    QString tabName = ui->tabView->tabText(index);
    if(tabName=="开始")   return;

    QDir d(dirPath+"/Files");
    //是否需要判断是新建的文件还是打开已有的文件?
    QString fullName=dirPath+"/Files/"+tabName;
    QFile f(fullName);
    if(!f.open(QIODevice::WriteOnly)){
        qDebug()<<"画面文件写入失败:"<<f.fileName();
        return;
    }

    QDataStream ds(&f);
    MyView* view = qobject_cast<MyView*>(ui->tabView->currentWidget());
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
//    printer.setColorMode(QPrinter::Color);    //不设置打印机时，加这两句会无法打印
//    printer.setOutputFormat(QPrinter::PdfFormat);
    QPagedPaintDevice::Margins m;
    m.bottom=10;
    m.top=10;
    m.left=10;
    m.right=10;
    printer.setMargins(m);

    QPainter painter(&printer);
    getCurrentView()->getScene()->render(&painter);
}

void MainWindow::on_tabView_tabCloseRequested(int index)
{
    QWidget* w = ui->tabView->widget(index);
    if(!getCurrentView())   return;
    if(getCurrentView()->IsSaved())
    {
        foreach(QObject *obj, w->children() )
            if(obj->inherits("QGraphicsView") )
                delete obj;
        ui->tabView->removeTab(index);
    }
    else
        ShowSaveBox();
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
}

void MainWindow::on_action_Exit_triggered()
{
    qApp->quit();
}

void MainWindow::on_startBtn_clicked()
{
    newView = new MyView(this);
    newView->setNew(true);
    newView->setFocus();    //获得焦点
    newView->scale(1,-1);   // 翻转y轴,默认y轴正方向指向下方
    newView->updateCenterRect();
    ui->tabView->addTab(newView,QIcon(":/Icon/Icon/gph.png"),"画面1.gph");
    ui->tabView->setCurrentWidget(newView);

    InitConnect(newView);
}

void MainWindow::on_action_Pic_triggered()
{
    if(!getCurrentView())
        return;
    QImage image(getCurrentView()->size(),QImage::Format_RGB32);
    QPainter painter(&image);
    getCurrentView()->getScene()->render(&painter);     //关键函数

//    因为m_view->scale(6, -6);对纵坐标做了镜像处理，所以再倒过来
    QImage mirroredImage = image.mirrored(false, true);
    QString path = QApplication::applicationDirPath()+"/Images";
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("MM-dd--hh-mm-ss"); //设置显示格式
    QString file = path+ "/" +str+ ".png";
    mirroredImage.save(file);
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

void MainWindow::on_pushButton_3_clicked()
{
    on_Open_triggered();
}

void MainWindow::LoadFile(const QString &fileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::restoreOverrideCursor();
    recentFilesMenu->addRecentFile(fileName);
}

QString MainWindow::getModifyTime()
{
    m_modified = true;
    QDateTime time = QDateTime::currentDateTime();
    modify_time = time.toString("上次修改时间: MM-dd hh:mm:ss"); //设置显示格式
    return modify_time;
}

void MainWindow::on_action_Reset_triggered()
{
    if(!getCurrentView())   return;
    getCurrentView()->Reset();
}

void MainWindow::on_action_Normal_triggered()
{
    if(!getCurrentView())   return;
    getCurrentView()->setNormal();
}

void MainWindow::on_action_Redraw_triggered()
{
    if(!getCurrentView())   return;
    getCurrentView()->Redraw();
}

void MainWindow::on_ColorPicker_clicked()
{
    PenColor = QColorDialog::getColor(Qt::white);
    emit toColor(PenColor);
}

void MainWindow::on_BrushPicker_clicked()
{
    PenBrush.setColor(QColorDialog::getColor(Qt::white));
    emit toBrush(PenBrush);
}

