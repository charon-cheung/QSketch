#include "selectdlg.h"
#include "ui_selectdlg.h"
#include <QMessageBox>

SelectDlg::SelectDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDlg)
{
    ui->setupUi(this);
    ui->start->setChecked(true);
}

SelectDlg::~SelectDlg()
{
    delete ui;
}

qreal SelectDlg::getLength()
{
    QString length = ui->lineEdit->text();
    return length.toDouble();
}

int SelectDlg::getDirection()
{
    if(ui->start->isChecked())
        direction = start;
    else if(ui->end->isChecked())
        direction = end;
    return direction;
}

void SelectDlg::on_Accept_clicked()
{
    accept();
}

void SelectDlg::on_Cancel_clicked()
{
    close();
}
