#ifndef CLOCK_H
#define CLOCK_H

#include <QWidget>
#include <QtGui>
#include <math.h>

class Clock : public QWidget
{
    Q_OBJECT
    
public:
    Clock(QWidget *parent = 0);
    ~Clock();

    void paintEvent(QPaintEvent * e);
    void drawFourLines(QPainter &paint);
    void drawBase(QPainter &paint);
    void drawHours(QPainter &paint, double alpha);
    void drawMinutes(QPainter &paint, double alpha);
    void drawSeconds(QPainter &paint, double alpha);
    void drawCenter(QPainter &paint);
    void clear(QPainter &paint);
    void timerEvent(QTimerEvent *);
    void initTime();
    void recountTime();
    QPair<double, double> getPoint(double nowX, double nowY, double alpha);

private:
    int Ox,Oy;
    int seconds,minutes,hours;
    double upX,upY;
    double alphaSec,alphaMin,alphaHours;
    bool first;
};

#endif // CLOCK_H
