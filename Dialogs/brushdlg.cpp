#include "brushdlg.h"
#include "ui_brushdlg.h"
#include <QColorDialog>
#include <QDebug>

BrushDlg::BrushDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrushDlg)
{
    ui->setupUi(this);
}

BrushDlg::~BrushDlg()
{
    delete ui;
}

Qt::BrushStyle BrushDlg::getStyle()
{
    if(ui->radioButton->isChecked())
        style = Qt::SolidPattern;
    else if(ui->radioButton_2->isChecked())
        style = Qt::HorPattern;
    else if(ui->radioButton_3->isChecked())
        style = Qt::VerPattern;
    else if(ui->radioButton_4->isChecked())
        style = Qt::CrossPattern;
    else if(ui->radioButton_5->isChecked())
        style = Qt::BDiagPattern;
    else if(ui->radioButton_6->isChecked())
        style = Qt::FDiagPattern;
    else if(ui->radioButton_7->isChecked())
        style = Qt::DiagCrossPattern;
    else if(ui->radioButton_8->isChecked())
        style = Qt::Dense7Pattern;
    else if(ui->radioButton_9->isChecked())
        style = Qt::NoBrush;

    return style;
}

QColor BrushDlg::getColor()
{
    return color;
}

void BrushDlg::on_pushButton_clicked()
{
    accept();
}

void BrushDlg::on_pushButton_2_clicked()
{
    close();
}

void BrushDlg::on_ColorPick_clicked()
{
    color = QColorDialog::getColor(Qt::white);
}
