#include "posdialog.h"
#include "ui_posdialog.h"
#include <QDebug>
#include <QtMath>

PosDialog::PosDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PosDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("请输入需要的参数");
    this->setWindowIcon(QIcon(":/Icon/Icon/input.png"));
    font.setFamily("Inconsolata");
    font.setPointSize(12);
    this->setStyleSheet("QLabel{ font-family: Inconsolata; font-size: 16pt;}"
                        "QLineEdit{ font-family: Inconsolata; font-size: 14pt;}");
    X = new QDoubleValidator(-MyScene::width/2, MyScene::width/2, 2,this);
    ui->pt1_x->setValidator(X);
    ui->pt2_x->setValidator(X);

    Y = new QDoubleValidator(-MyScene::height/2, MyScene::height/2, 2,this);
    ui->pt1_y->setValidator(Y);
    ui->pt2_y->setValidator(Y);

    double diagonal = qPow(MyScene::width/2,2)+qPow(MyScene::height/2,2);
    diagonal = qSqrt(diagonal);

    Positive = new QDoubleValidator(0,diagonal,2,this);
    ui->length->setValidator(Positive);
    ui->width->setValidator(Positive);
    ui->height->setValidator(Positive);

    Angle = new QDoubleValidator(-360,360,2,this);
    ui->angle->setValidator(Angle);
    ui->pt1_x->setFocus();
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

void PosDialog::showPtAngle()
{
    ui->frame->setVisible(false);
    ui->frame_2->setVisible(false);
    ui->length->setVisible(false);
    ui->label_6->setVisible(false);
    return;
}

void PosDialog::showLineXY()
{
    ui->label->setText("起点的横坐标");
    ui->label_2->setText("起点的纵坐标");
    ui->label_3->setText("终点的横坐标");
    ui->label_4->setText("终点的纵坐标");
    ui->frame->setVisible(false);
    ui->frame_3->setVisible(false);
    return;
}

void PosDialog::showLineAH()
{
    ui->label->setText("起点的横坐标");
    ui->label_2->setText("起点的纵坐标");
    ui->frame->setVisible(false);
    ui->frame_2->setVisible(false);
    return;
}

void PosDialog::showRectXY()
{
    ui->label->setText("左上顶点的横坐标");
    ui->label_2->setText("左上顶点的纵坐标");
    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
}

void PosDialog::showEllipse()
{
    ui->label->setText("圆心横坐标");
    ui->label_2->setText("圆心纵坐标");
    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
}

float PosDialog::getAngle()
{
    return ui->angle->text().toFloat();
}

float PosDialog::getLength()    //很大的非零非整数
{
    QString len = ui->length->text();
    float length;
    if(len.isEmpty() || len=="0")
        length =0;
    else
        length = ui->length->text().toFloat();
    return length;
}

float* PosDialog::getWH()
{
    float *value = new float[2];
    value[0]=ui->width->text().toFloat();
    value[1]=ui->height->text().toFloat();
    if(value[0]!=0 && value[1]!=0)
    {;}
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
