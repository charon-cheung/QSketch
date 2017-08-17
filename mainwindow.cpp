#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCursor>
#include <QDebug>
#include <QGraphicsItem>
#include <QTransform>
#include <QDateTime>
#include <QApplication>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->InitUi();
    this->InitDir();

    const int tabH = ui->tabView->tabBar()->height();
    m_view = new MyView(ui->tab);
    m_view->setGeometry(0,tabH,600,400);
    // 坐标放大倍数,倍数为1时,1个单位坐标就是1个像素
    m_view->scale(6, -6);
    m_view->show();

//    ui->centralWidget->setMouseTracking(true);
//    this->setMouseTracking(true);   //鼠标不按下的移动也能捕捉到MouseMoveEvent
    foreach(QAction* act, ptActions)
        connect(act, &QAction::triggered, m_view, &MyView::setPt);
    foreach(QAction* act, lineActions)
        connect(act, &QAction::triggered, m_view, &MyView::setLine );
    foreach(QAction* act, rectActions)
        connect(act, &QAction::triggered, m_view, &MyView::setRect );
    foreach(QAction* act, ellipseActions)
        connect(act, &QAction::triggered, m_view, &MyView::setEllipse );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitUi()
{
    ui->mainToolBar->addAction(ui->NewView);
    ui->mainToolBar->addAction(ui->Open);
    ui->mainToolBar->addAction(ui->Save);
    ui->mainToolBar->addAction(ui->SaveAs);
    ui->mainToolBar->addAction(ui->Print);
    ui->mainToolBar->addAction(ui->Previous);
    ui->mainToolBar->addAction(ui->Next);

    this->setWindowTitle("QSketch");
    this->setWindowIcon(QIcon(":/Icon/Icon/ruler.png"));
    ui->DrawPt->setFocusPolicy(Qt::NoFocus);
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
    MyView *newView = new MyView(this);


    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               dirPath+"/files",
                               tr("画面文件(*.gph)"));
    int index=ui->tabView->addTab(newView,"新标签");
    ui->tabView->setCurrentWidget(newView);

}

void MainWindow::on_Open_triggered()
{

}

void MainWindow::on_Save_triggered()
{

}

void MainWindow::on_SaveAs_triggered()
{
    QImage image(this->size(),QImage::Format_RGB32);
    QPainter painter(&image);
    m_view->getScene()->render(&painter);   //关键函数
//    因为m_view->scale(6, -6);对纵坐标做了镜像处理，所以再倒过来
    QImage mirroredImage = image.mirrored(false, true);
    QString path = QApplication::applicationDirPath()+"/Images";
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("MM-dd--hh-mm-ss"); //设置显示格式
    QString file = path+ "/" +str+ ".png";
    mirroredImage.save(file);
}
