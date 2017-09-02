#ifndef BRUSHDLG_H
#define BRUSHDLG_H

#include <QDialog>

namespace Ui {
class BrushDlg;
}

class BrushDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BrushDlg(QWidget *parent = 0);
    ~BrushDlg();
    Qt::BrushStyle getStyle();
    QColor getColor();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_ColorPick_clicked();

private:
    Ui::BrushDlg *ui;
    Qt::BrushStyle style;
    QColor color;
};

#endif // BRUSHDLG_H
