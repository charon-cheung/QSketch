#include "clock.h"
#include <QPair>

Clock::Clock(QWidget *parent)
    : QWidget(parent)
{
    resize(200,200);
    Ox = 100; Oy = 100;
    upX = 100; upY = 5;
    first = true;
    initTime();
    startTimer(1000);
}

void Clock::timerEvent(QTimerEvent *)
{
    update();
    recountTime();
}

Clock::~Clock()
{

}

void Clock::initTime()
{
    QTime time = QTime::currentTime();
    seconds = time.second();
    minutes = time.minute();
    hours = time.hour();
    alphaSec = 6.0*seconds;
    alphaMin = 6.0*minutes+(seconds*0.1);
    alphaHours = 30.0*hours+(minutes*0.5)+(0.025*seconds/3);
}

void Clock::recountTime()
{
    seconds++;
    minutes += (seconds/60);
    seconds %= 60;
    hours += (minutes/60);
    minutes %= 60;

    alphaSec += 6.0;
    if(alphaSec == 360.0)
        alphaSec = 0.0;

    if(!first){
        if(!(seconds%5))
            alphaMin += 0.5;
        if(alphaMin == 360.0)
            alphaMin = 0.0;

        if(!(seconds%12))
            alphaHours += 0.1;
        if(alphaHours == 360.0)
            alphaHours = 0.0;
    }
    else
        first = false;
}

QPair<double,double> Clock::getPoint(double nowX, double nowY, double alpha)
{
    double retX = (nowX-Ox)*cos(alpha*M_PI/180.0) - (nowY-Oy)*sin(alpha*M_PI/180.0) + Ox;
    double retY = (nowY-Oy)*cos(alpha*M_PI/180.0) + (nowX-Ox)*sin(alpha*M_PI/180.0) + Oy;
    if(abs(retX)<1e-9)
        retX=0;
    if(abs(retY)<1e-9)
        retY=0;
    if(retX<0)
        retX = 200+retX;
    if(retY<0)
        retY = 200+retY;
    return QPair<double,double>(retX,retY);
}

void Clock::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);
    paint.setRenderHint(QPainter::HighQualityAntialiasing);
    clear(paint);
    drawBase(paint);
    drawHours(paint,alphaHours);
    drawMinutes(paint,alphaMin);
    drawSeconds(paint,alphaSec);
    drawCenter(paint);
}

void Clock::drawBase(QPainter &paint)
{
    paint.setPen(QPen(Qt::black,3,Qt::SolidLine));
    paint.setRenderHint(QPainter::Antialiasing, true);
    paint.drawEllipse(5,5,190,190);

    double nowUpX=195,nowUpY=100,nowDownX=180,nowDownY=100;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);

    nowUpX=100; nowUpY=195; nowDownX=100; nowDownY=180;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);

    nowUpX=5; nowUpY=100; nowDownX=20; nowDownY=100;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);

    nowUpX=100; nowUpY=5; nowDownX=100; nowDownY=20;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);

    nowDownY -= 10;
    for(int i=1;i<=12;++i){
        QPair<double,double> tmp = getPoint(nowUpX,nowUpY,30.0);
        nowUpX = tmp.first; nowUpY = tmp.second;

        tmp = getPoint(nowDownX,nowDownY,30.0);
        nowDownX = tmp.first; nowDownY = tmp.second;

        if(i==12)
            continue;
        paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);
    }

    nowDownY -= 5;
    for(int i=1;i<=60;++i){
        QPair<double,double> tmp = getPoint(nowUpX,nowUpY,6.0);
        nowUpX = tmp.first; nowUpY = tmp.second;

        tmp = getPoint(nowDownX,nowDownY,6.0);
        nowDownX = tmp.first; nowDownY = tmp.second;

        paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);
    }
}


void Clock::drawSeconds(QPainter &paint, double alpha)
{
    QPair<double,double> tmp = getPoint(upX,upY+15,alpha);
    paint.setPen(QPen(Qt::green,1,Qt::SolidLine));
    paint.setRenderHint(QPainter::Antialiasing, true);
    paint.drawLine(Ox,Oy,tmp.first,tmp.second);
    tmp = getPoint(upX,100-30,alpha+180.0);
    paint.drawLine(Ox,Oy,tmp.first,tmp.second);
}

void Clock::drawMinutes(QPainter &paint, double alpha)
{
    QPair<double,double> tmp = getPoint(upX,upY+15,alpha);
    paint.setPen(QPen(Qt::red,2,Qt::SolidLine));
    paint.setRenderHint(QPainter::Antialiasing, true);
    paint.drawLine(Ox,Oy,tmp.first,tmp.second);
}

void Clock::drawHours(QPainter &paint, double alpha)
{
    QPair<double,double> tmp = getPoint(upX,upY+50,alpha);
    paint.setPen(QPen(Qt::blue,2,Qt::SolidLine));
    paint.setRenderHint(QPainter::Antialiasing, true);
    paint.drawLine(Ox,Oy,tmp.first,tmp.second);
}

void Clock::drawCenter(QPainter &paint)
{
    paint.setPen(QPen(Qt::black,6,Qt::SolidLine,Qt::RoundCap));
    paint.setRenderHint(QPainter::Antialiasing, true);
    paint.drawPoint(100,100);
}

void Clock::clear(QPainter &paint)
{
    paint.eraseRect(0,0,200,200);
}
