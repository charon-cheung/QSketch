#include "clock.h"
#include <QPair>

Clock::Clock(QWidget *parent)
    : QWidget(parent)
{
    resize(256,256);
    Ox = 100;   Oy = 100;
    upX = 100;  upY = 5;
    first = true;

    // 背景透明, 妨碍了图片的加载
    setStyleSheet("Clock{ background-color:rgba(0,0,0,0%);}  ");
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
    Q_UNUSED(e);
    QPainter paint(this);
    paint.setRenderHint(QPainter::HighQualityAntialiasing);
    clear(paint);
    drawBase(paint);
//    drawGraduation(paint);
//    drawValue(paint);
    drawHours(paint,alphaHours);
    drawMinutes(paint,alphaMin);
//    drawSeconds(paint,alphaSec);
    drawCenter(paint);
}

void Clock::drawBase(QPainter &paint)
{
    QPen p;
    p.setColor(QColor(234,234,234));
    p.setWidthF(3);
    p.setStyle(Qt::SolidLine);
    paint.setPen(p);
    paint.setRenderHint(QPainter::HighQualityAntialiasing, true);
    //外圈渐变色
    qreal radius = 95;    // 半径
    QPointF center(100, 100);  // 圆心
    QPointF focus(100, 100); // 焦点
    QRadialGradient outter(center, radius, focus);
    outter.setColorAt(0.94, QColor(255,255,255));
    outter.setColorAt(1.0, QColor(90,90,90));
    paint.setBrush(QBrush(outter));

    QPen outPen;
    outPen.setWidth(4);
    outPen.setColor(QColor(88,88,88));
    paint.setPen(outPen);
    paint.drawEllipse(center, radius, radius);
}

void Clock::drawGraduation(QPainter &paint)
{
    QPen p;
    p.setWidthF(1.5);
    p.setColor(QColor(190,190,190));
    p.setStyle(Qt::SolidLine);
    paint.setRenderHint(QPainter::HighQualityAntialiasing, true);
    paint.setPen(p);
    //右
    double nowUpX=185,nowUpY=100, nowDownX=145,nowDownY=100;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);
    //下
    nowUpX=100; nowUpY=185; nowDownX=100; nowDownY=145;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);
    //左
    nowUpX=15; nowUpY=100; nowDownX=55; nowDownY=100;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);
    //上
    nowUpX=100; nowUpY=15; nowDownX=100; nowDownY=55;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);
    // 补充8条线
    nowUpX=135; nowUpY=120; nowDownX=170; nowDownY=140;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);

    nowUpX=120; nowUpY=135; nowDownX=140; nowDownY=170;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);

    nowUpX=170; nowUpY=140; nowDownX=135; nowDownY=120;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);

    nowUpX=135; nowUpY=80; nowDownX=170; nowDownY=60;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);

    nowUpX=120; nowUpY=65; nowDownX=140; nowDownY=30;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);

    nowUpX=80; nowUpY=65; nowDownX=60; nowDownY=30;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);

    nowUpX=65; nowUpY=80; nowDownX=30; nowDownY=60;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);

    nowUpX=65; nowUpY=120; nowDownX=30; nowDownY=140;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);

    nowUpX=80; nowUpY=135; nowDownX=60; nowDownY=170;
    paint.drawLine(nowUpX,nowUpY,nowDownX,nowDownY);
}

void Clock::drawValue(QPainter &paint)
{
    QPen p;
    paint.setRenderHint(QPainter::HighQualityAntialiasing, true);
    p.setWidthF(3.5);
    p.setColor(QColor(90,90,90));
    p.setStyle(Qt::SolidLine);
    paint.setPen(p);
    paint.drawText(QRect(130,95,10,10),"3");
    paint.drawText(QRect(97,130,10,10),"6");
    paint.drawText(QRect(65,95,10,10),"9");
    paint.drawText(QRect(95,65,15,10),"12");
}

void Clock::drawSeconds(QPainter &paint, double alpha)
{
    QPair<double,double> tmp = getPoint(upX,upY+20,alpha);
    paint.setPen(QPen(QColor(245,88,80),3.2,Qt::SolidLine));
    paint.setRenderHint(QPainter::HighQualityAntialiasing, true);
    // 长侧
    paint.drawLine(Ox,Oy,tmp.first,tmp.second);
    paint.drawEllipse(QPoint(100,100),5,5);
    // 短侧
    tmp = getPoint(upX,100-20,alpha+180.0);
    paint.drawLine(Ox,Oy,tmp.first,tmp.second);
}

void Clock::drawMinutes(QPainter &paint, double alpha)
{
    QPair<double,double> tmp = getPoint(upX,upY+15,alpha);
    paint.setPen(QPen(QColor(25,25,25),4,Qt::SolidLine));
    paint.setRenderHint(QPainter::HighQualityAntialiasing, true);
    paint.drawLine(Ox,Oy,tmp.first,tmp.second);
}

void Clock::drawHours(QPainter &paint, double alpha)
{
    QPair<double,double> tmp = getPoint(upX,upY+50,alpha);
    paint.setPen(QPen(QColor(25,25,25),6,Qt::SolidLine));
    paint.setRenderHint(QPainter::HighQualityAntialiasing, true);
    paint.drawLine(Ox,Oy,tmp.first,tmp.second);
}

void Clock::drawCenter(QPainter &paint)
{
    paint.setPen(QPen(Qt::white,1,Qt::SolidLine));
    paint.setRenderHint(QPainter::HighQualityAntialiasing, true);
    paint.drawEllipse(QPoint(100,100),2,2);
}

void Clock::clear(QPainter &paint)
{
    paint.eraseRect(0,0,200,200);
}
