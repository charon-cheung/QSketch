#include "posdialog.h"
#include "ui_posdialog.h"
#include <QDebug>
#include <QtMath>

PosDialog::PosDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PosDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("请输入点的坐标");
    this->setWindowIcon(QIcon(":/Icon/Icon/input.png"));
    X = new QDoubleValidator(-MyScene::width/2, MyScene::width/2, 2,this);
    ui->pt1_x->setValidator(X);
    ui->pt2_x->setValidator(X);

    Y = new QDoubleValidator(-MyScene::height/2, MyScene::height/2, 2,this);
    ui->pt1_y->setValidator(Y);
    ui->pt2_y->setValidator(Y);

    Positive = new QDoubleValidator(0,800,2,this);
    ui->length->setValidator(Positive);
    ui->width->setValidator(Positive);
    ui->height->setValidator(Positive);

    Angle = new QDoubleValidator(-360,360,2,this);
    ui->angle->setValidator(Angle);
}

PosDialog::~PosDialog()
{
    delete ui;
}

QPointF PosDialog::getPt()
{
    return pt1;
}

QList<QPointF> PosDialog::getLine()
{
    QList<QPointF> list;
    list << pt1 << pt2;
    return list;
}

void PosDialog::showPt()
{
    ui->frame->setVisible(false);
    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
    return;
}

void PosDialog::showLineXY()
{
    ui->frame->setVisible(false);
    ui->frame_3->setVisible(false);
    return;
}

void PosDialog::showLineAH()
{
    ui->frame->setVisible(false);
    ui->frame_2->setVisible(false);
    return;
}

void PosDialog::showRectXY()
{
    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
}

void PosDialog::showEllipse()
{
    showRectXY();
}

float PosDialog::getAngle()
{
    return ui->angle->text().toFloat();
}

float PosDialog::getLength()    //很大的非零非整数
{
    QString length = ui->length->text();
    if(length.isEmpty() || length=="0")
        return 0;
    else return ui->length->text().toFloat();
}

float* PosDialog::getWH()
{
    float *value = new float[2];
    value[0]=ui->width->text().toFloat();
    value[1]=ui->height->text().toFloat();
    if(value[0]!=0 && value!=0)
        return value;
}

void PosDialog::on_accept_clicked()
{
    pt1.setX(ui->pt1_x->text().toFloat());
    pt1.setY(ui->pt1_y->text().toFloat());
    pt2.setX(ui->pt2_x->text().toFloat());
    pt2.setY(ui->pt2_y->text().toFloat());
    accept();
}

void PosDialog::on_cancel_clicked()
{
    this->close();
}
