/*

  Copyright (c) 2020 Xiaofang Qi <qi.shield95@foxmail.com>

  This file is part of cangaroo specific for foc drive.

  cangaroo is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  cangaroo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with cangaroo.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "GraphWindow.h"
#include "ui_GraphWindow.h"

#include <core/Backend.h>

double velocity = 0;

GraphWindow::GraphWindow(QWidget *parent, Backend &backend) :
    ConfigurableWidget(parent),
    ui(new Ui::GraphWindow),
    _backend(backend)
{
    ui->setupUi(this);

    customPlot = new QCustomPlot(this);

    tar_Vel = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    tar_Vel->setName("tar_Vel");
    tar_Vel->setPen(QPen(Qt::blue,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    actual_Vel = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    actual_Vel->setName("actual_Vel");
    actual_Vel->setPen(QPen(Qt::red,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    tar_Pos = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    tar_Pos->setName("tar_Pos");
    tar_Pos->setPen(QPen(Qt::darkYellow,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    actual_Pos = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    actual_Pos->setName("actual_Pos");
    actual_Pos->setPen(QPen(Qt::darkCyan,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    tar_Iq = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    tar_Iq->setName("tar_Iq");
    tar_Iq->setPen(QPen(Qt::darkYellow,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    actual_Iq = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    actual_Iq->setName("actual_Iq");
    actual_Iq->setPen(QPen(Qt::darkCyan,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    tar_Id = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    tar_Id->setName("tar_Id");
    tar_Id->setPen(QPen(Qt::darkYellow,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    actual_Id = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    actual_Id->setName("actual_Id");
    actual_Id->setPen(QPen(Qt::darkCyan,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    customPlot->setGeometry(5,5,1150,700);
    customPlot->legend->setVisible(true);
    customPlot->legend->setBrush(QColor(255,255,255,0));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    customPlot->rescaleAxes();
    // add the cursorXY
    customPlot->addGraph();
    customPlot->graph(0)->setName("Cursor_x");
    customPlot->addGraph();
    customPlot->graph(1)->setName("Cursor_y");
    // add realtime data demo
    customPlot->addGraph();
    customPlot->graph(2)->setPen(QPen(QColor(40, 110, 255)));
    customPlot->addGraph();
    customPlot->graph(3)->setPen(QPen(QColor(255, 110, 40)));
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-1.2, 1.2);
    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer->start(0); // Interval 0 means to refresh as fast as possible

    customPlot->replot();
    connect(customPlot,&QCustomPlot::mouseMove,this,&GraphWindow::myMoveEvent);

}

void GraphWindow::realtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      velocity = qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843);
      customPlot->graph(2)->addData(key, velocity);
      customPlot->graph(3)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(2)->rescaleValueAxis();
      //ui->customPlot->graph(3)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    customPlot->xAxis->setRange(key, 16, Qt::AlignRight);
    customPlot->replot();

}

void GraphWindow::myMoveEvent(QMouseEvent *event)
{
    double x = event->pos().x();
    double x_ = customPlot->xAxis->pixelToCoord(x);

    double y = event->pos().y();
    double y_ = customPlot->yAxis->pixelToCoord(y);

    //更新光标位置
    QVector<double> x_1(101), y_1(101);
    for (int i=0; i<101; ++i)
    {
        x_1[i] = customPlot->xAxis->range().lower+
                i*(customPlot->xAxis->range().upper-customPlot->xAxis->range().lower)/101;
        y_1[i] = y_;
    }
    customPlot->graph(0)->setData(x_1,y_1);
    customPlot->graph(0)->setPen(QPen(Qt::green,1.5, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));

    QVector<double> x_2(101), y_2(101);
    for (int i=0; i<101; ++i)
    {
        x_2[i] = x_;
        y_2[i] = customPlot->yAxis->range().lower+
                i*(customPlot->yAxis->range().upper-customPlot->yAxis->range().lower)/101;
    }
    customPlot->graph(1)->setData(x_2,y_2);
    customPlot->graph(1)->setPen(QPen(Qt::green,1.5, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));

    customPlot->replot();

    QString *str = new QString;
    *str = QString("Hello QCustomplot!");
    QToolTip::showText(cursor().pos(),*str);
    delete str;
}

GraphWindow::~GraphWindow()
{
    delete ui;
    delete customPlot;
    delete tar_Vel;
    delete actual_Vel;
    delete tar_Pos;
    delete actual_Pos;
    delete tar_Iq;
    delete actual_Iq;
    delete tar_Id;
    delete actual_Id;
    delete dataTimer;
}
