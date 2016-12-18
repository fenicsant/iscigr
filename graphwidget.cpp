#include "graphwidget.h"
#include "settingfile.h"
#include <QDebug>
#include <QPainter>

#define TestGenerator

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent), frequency(120), dataPos(0), data(frequency * 2)
{
    load();
}

bool GraphWidget::load()
{
    startTimer(repaintInterval);
#ifdef TestGenerator
    startTimer(30);
//    startTimer(5);
//    startTimer(2);
//    startTimer(1);
#endif
    return true;
}

void GraphWidget::addValue(DataType v)
{
    int pos = dataPos + 1;
    if (pos >= data.size()) pos = 0;
    data[dataPos = pos] = v;
}

void GraphWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,paintBuffer);
}

void GraphWidget::resizeEvent(QResizeEvent *)
{
    update();
}

#include <math.h>
#include <QTimerEvent>

void GraphWidget::timerEvent(QTimerEvent * e)
{
    if (e->timerId() == 1) {
        repaintBuffer();
        update();
    }

#ifdef TestGenerator
    static int aaa = 0;
//    aaa = ++aaa % 20;
    static double x = 0;
    if (aaa==0) addValue(500 * sin(x+=.1) + 100.0 * rand()/RAND_MAX + 250 * sin(x*3) + 200 * sin(x* 9 / 2));
#endif
}

void GraphWidget::repaintBuffer()
{
    // make next bitmap
    QPixmap tmpBuff = QPixmap(size());
    QPainter paint(&tmpBuff);
    tmpBuff.fill(Qt::black);

    // out background lines
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setColor(Qt::darkBlue);
    paint.setPen(pen);

    int lwidth = width();
    int lheight = height();
    int l4height = lheight / 4;
    paint.drawLine(0, l4height * 2, lwidth, l4height * 2);
    paint.drawLine(0, l4height, lwidth, l4height);
    paint.drawLine(0, l4height * 3, lwidth, l4height * 3);
    paint.drawLine(lwidth / 4, 0, lwidth / 4, l4height * 4);
    paint.drawLine(lwidth * 3 / 4, 0, lwidth * 3 / 4, l4height * 4);


    // calculations

    // get Maximum and Minimum
    DataType locMax = data[dataPos];
    DataType locMin = data[dataPos];
    for (int i = 1; i < frequency; ++i) {
        DataType v = getPrevVal(i);
        if (locMax < v) locMax = v;
        if (locMin > v) locMin = v;
    }

    if (locMax == locMin) {
        locMax += 1;
        locMin += 1;
    }

    // get Midle value
    DataType locMid = (locMax - locMin) / 2 + locMin;


    // find syncronization offset
    int locStOffset = 0;
    while (locStOffset < frequency && ! (getPrevVal(locStOffset) < locMid && getPrevVal(locStOffset +3) > locMid)) {
        ++locStOffset;
    }
    if (locStOffset == frequency) locStOffset = 0;

    double screenStep = (double)1.0 * width() / frequency;
    double screenScale = (double)1.0 * (height() - 10) / (locMax - locMin);


    QPolygon polyg = QPolygon(frequency);
    for (int i = 0; i < frequency; ++i) {
        polyg.setPoint(i, QPoint(width() - (int)(screenStep*i), height() - 5 - (int)(screenScale*(getPrevVal(i + locStOffset) - locMin))));
    }

    paint.setPen(QColor(80, 255, 80));
    paint.drawPolyline(polyg);

    paint.setPen(Qt::white);
    paint.fillRect(0,0,30,18, Qt::black);
    paint.drawText(2, 15, QString::number(locMax));
    paint.fillRect(0, height() - 18, 30, height(), Qt::black);
    paint.drawText(2, height() - 3, QString::number(locMin));
//    paint.drawText(2, height() - 3, QString::number(periodStep));
    maxVal = locMax;
    minVal = locMin;
    paintBuffer = tmpBuff;
}

