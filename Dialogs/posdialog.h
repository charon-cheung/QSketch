#ifndef POSDIALOG_H
#define POSDIALOG_H

#include <QDialog>
#include <QDoubleValidator>
#include "myscene.h"
namespace Ui {
class PosDialog;
}

class MyScene;
class PosDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PosDialog(QWidget *parent = 0);
    ~PosDialog();
    QPointF getPt();
    QList<QPointF> getLine();
    void showPt();
    void showPtAngle();
    void showLineXY();
    void showLineAH();
    void showRectXY();
    void showEllipse();

    float getAngle();
    float getLength();
    float* getWH();
private slots:
    void on_accept_clicked();

    void on_cancel_clicked();

private:
    Ui::PosDialog *ui;
    QDoubleValidator *X, *Y, *Positive, *Angle;
    QPointF pt1,pt2;
    QFont font;
    int angle,length;
};

#endif // POSDIALOG_H
