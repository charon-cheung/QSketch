#ifndef SELECTDLG_H
#define SELECTDLG_H

#include <QDialog>

namespace Ui {
class SelectDlg;
}

class SelectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDlg(QWidget *parent = 0);
    ~SelectDlg();
    enum Direction
    {
        start,
        end
    };

    qreal getLength();
    int getDirection();
private slots:
    void on_Accept_clicked();

    void on_Cancel_clicked();

private:
    Ui::SelectDlg *ui;
    int direction;
};

#endif // SELECTDLG_H
