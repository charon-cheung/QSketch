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
#include <QCursor>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->InitUi();
    this->InitDir();
    // 坐标放大倍数,倍数为1时,1个单位坐标就是1个像素
    ui->m_view->scale(2,-2);
    ui->m_view->updateCenterRect();     //改善坐标轴不清晰的问题,但补得不全
//    ui->centralWidget->setMouseTracking(true);
//    this->setMouseTracking(true);   //鼠标不按下的移动也能捕捉到MouseMoveEvent
    foreach(QAction* act, ptActions)
        connect(act, &QAction::triggered, ui->m_view, &MyView::setPt);
    foreach(QAction* act, lineActions)
        connect(act, &QAction::triggered, ui->m_view, &MyView::setLine );
    foreach(QAction* act, rectActions)
        connect(act, &QAction::triggered, ui->m_view, &MyView::setRect );
    foreach(QAction* act, ellipseActions)
        connect(act, &QAction::triggered, ui->m_view, &MyView::setEllipse );
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
    ui->mainToolBar->addAction(ui->SaveAs);
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
}

void MainWindow::InitView()
{

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
    QString name=fullName.remove(dirPath+"/Files/");
//    name.chop(4);   //去掉扩展名
//    QFile f(fullName);
    MyView *newView = new MyView(this);
    newView->setObjectName(name);
    ui->tabView->addTab(newView,name);
    ui->tabView->setCurrentWidget(newView);
    int index = ui->tabView->currentIndex();
    ui->tabView->setTabIcon(index, QIcon(":/Icon/Icon/gph.png"));
}

void MainWindow::on_Open_triggered()
{

}
//参考 GraphForm::Save()   GraphIO::ExportGraph
void MainWindow::on_Save_triggered()
{
//    QString filename=QFileDialog::getSaveFileName(this,tr("save license"),tr("abc.txt"));
//    QByteArray ba = filename.toLatin1();

//    char* Filename = ba.data(); //将QString装换为char*

//    FILE* pFile = fopen(Filename,"wb");

//    fwrite(---,---,---,pFile);

//    fclose(pFile);
    int index = ui->tabView->currentIndex();
    QString tabText = ui->tabView->tabText(index);
    QDir d(dirPath+"/Files");
    if(d.entryList().contains(tabText.append(".gph")))
    {
        //判断是否修改
    }
    else
    {
        QString fullName=dirPath+"/Files/"+tabText+".gph";
        QFile *f;
        f->setFileName(fullName);
        QDataStream ds(f);
        if(!f->open(QIODevice::WriteOnly) )
        {
            qDebug()<<"保存时出错";
            return;
        }

    }
}

void MainWindow::on_SaveAs_triggered()
{
    QImage image(this->size(),QImage::Format_RGB32);
    QPainter painter(&image);
    ui->m_view->getScene()->render(&painter);   //关键函数
//    因为ui->m_view->scale(6, -6);对纵坐标做了镜像处理，所以再倒过来
    QImage mirroredImage = image.mirrored(false, true);
    QString path = QApplication::applicationDirPath()+"/Images";
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("MM-dd--hh-mm-ss"); //设置显示格式
    QString file = path+ "/" +str+ ".png";
    mirroredImage.save(file);
}

void MainWindow::on_Print_triggered()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);

    QPainter painter(&printer);
    ui->m_view->getScene()->render(&painter);
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
    on_tabView_tabCloseRequested(index);
}

void MainWindow::on_action_All_triggered()
{
    int count=ui->tabView->count();
    for(int i=count-1;i>=0;i--)
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
