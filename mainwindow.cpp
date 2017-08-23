#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCursor>
#include <QGraphicsItem>
#include <QTransform>
#include <QDateTime>
#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QtPrintSupport/QPrinter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitUi();
    InitDir();

//    ui->centralWidget->setMouseTracking(true);
//    this->setMouseTracking(true);   //鼠标不按下的移动也能捕捉到MouseMoveEvent
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitUi()
{
    this->setWindowTitle("QSketch");
    this->setWindowIcon(QIcon(":/Icon/Icon/cad.png"));

    ui->mainToolBar->addAction(ui->NewView);
    ui->mainToolBar->addAction(ui->Open);
    ui->mainToolBar->addAction(ui->Save);
    ui->mainToolBar->addAction(ui->Print);

    ui->tabView->setTabsClosable(true);
    ui->tabView->setCurrentIndex(0);
    ui->tabWidget->setCurrentIndex(0);
//    ui->DrawPt->setFocusPolicy(Qt::NoFocus);

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

    ui->statusBar->showMessage("初始化完成");
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

void MainWindow::on_NewView_triggered()
{
    QString fullName = QFileDialog::getSaveFileName(this, tr("新建画面"),
                       dirPath+"/Files", tr("画面文件(*.gph)"));
    if(fullName.isEmpty())      return;
    QString name=fullName.remove(dirPath+"/Files/");

    MyView *newView = new MyView(this);
    newView->setObjectName(name);
    // 坐标放大倍数,倍数为1时,1个单位坐标就是1个像素
    newView->scale(2,-2);
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
    QString fileName=QFileDialog::getOpenFileName(this,"打开画面文件",dirPath+"/Files",tr("画面文件(*.gph)") );
    if(fileName.isEmpty())      return;
    QFile f(fileName);
    if(!f.open(QIODevice::ReadOnly)){
        qDebug()<<"画面文件读取失败："<<fileName;
        return ;
    }
    QDataStream ds(&f);
    MyScene *doc = new MyScene(0);
    doc->Load(ds);

    MyView *openView = new MyView(this);
    openView->setScene(doc);
    fileName.remove(dirPath+"/Files/");
    openView->setObjectName(fileName);
    openView->scale(2,-2);
    ui->tabView->addTab(openView,QIcon(":/Icon/Icon/gph.png"),fileName);
    ui->tabView->setCurrentWidget(openView);

    InitConnect(openView);
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
}

void MainWindow::on_Print_triggered()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);

    QPainter painter(&printer);
    MyView* view = qobject_cast<MyView*>(ui->tabView->currentWidget());
    view->getScene()->render(&painter);
}

void MainWindow::on_tabView_tabCloseRequested(int index)
{
    QWidget* w = ui->tabView->widget(index);
    foreach(QObject *obj, w->children() )
        if(obj->inherits("QGraphicsView") )
            delete obj;
    ui->tabView->removeTab(index);
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
    ui->tabView->setCurrentIndex(1);
}

void MainWindow::on_action_Pic_triggered()
{
    if(ui->tabView->currentIndex()==0)
        return;
    MyView* view = qobject_cast<MyView*>(ui->tabView->currentWidget());
    QImage image(view->size(),QImage::Format_RGB32);
    QPainter painter(&image);
    view->getScene()->render(&painter);     //关键函数

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
