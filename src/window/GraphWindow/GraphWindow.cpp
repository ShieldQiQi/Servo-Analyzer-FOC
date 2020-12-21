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

    tar_Pos = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    tar_Pos->setName("tar_Pos");
    tar_Pos->setPen(QPen(Qt::red,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    actual_Pos = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    actual_Pos->setName("actual_Pos");
    actual_Pos->setPen(QPen(Qt::blue,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    tar_Vel = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    tar_Vel->setName("tar_Vel");
    tar_Vel->setPen(QPen(Qt::darkGreen,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    actual_Vel = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    actual_Vel->setName("actual_Vel");
    actual_Vel->setPen(QPen(Qt::yellow,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    tar_Iq = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    tar_Iq->setName("tar_Iq");
    tar_Iq->setPen(QPen(Qt::cyan,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    actual_Iq = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    actual_Iq->setName("actual_Iq");
    actual_Iq->setPen(QPen(Qt::darkYellow,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    tar_Id = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    tar_Id->setName("tar_Id");
    tar_Id->setPen(QPen(Qt::magenta,1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-5, 5);

    // add a Easter egg (●'◡'●)
    customPlot->addGraph();
    customPlot->graph(2)->setPen(QColor(50, 50, 50, 255));
    customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, 4));
    customPlot->graph(2)->setName("極楽浄土");
    file = new QFile("E:/FOC-Analyzer-CANOPEN/src/assets/pointXY.csv");
    textStream = new QTextStream;
    if(!file->open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::information(this,"","Can not load the csv file",NULL);
        isFileOpened = 0;
    }else{
        isFileOpened = 1;
        textStream->setDevice(file);
    }

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer->start(0); // Interval 0 means to refresh as fast as possible

    easterEggTimer = new QTimer(this);
    connect(easterEggTimer, SIGNAL(timeout()), this, SLOT(easterEggDataSlot()));

    customPlot->replot();
    connect(customPlot,&QCustomPlot::mouseMove,this,&GraphWindow::myMoveEvent);

}

void GraphWindow::realtimeDataSlot()
{
    if(!isPlayEasterEgg)
    {
        // rescale value (vertical) axis to fit the current data:
        tar_Pos->rescaleValueAxis(true);
        // make key axis range scroll with the data (at a constant range size of 8):

        customPlot->replot();
    }
}

void GraphWindow::DecodeCANMsg(QString string)
{
    // decode actual pos,velocity,Id,Iq from CAN Msg
    QStringList sections = string.split(QRegExp("[ ,*/^]"));
    targetPosQueue.append((sections[0]+sections[1]).toInt(nullptr,16));
    actualPosQueue.append((sections[2]+sections[3]).toInt(nullptr,16));
    targetVelQueue.append(sections[4].toInt(nullptr,16)/10.0);
    actualVelQueue.append(sections[5].toInt(nullptr,16)/10.0);
    targetIqQueue.append(sections[6].toInt(nullptr,16)/10.0);
    actualIqQueue.append(sections[7].toInt(nullptr,16)/10.0);

    velocity = sections[5].toInt(nullptr,16)/10.0;

    // add data to lines:
    static double key = 0;
    if(!targetPosQueue.isEmpty())
    {
//        qDebug()<<targetPosQueue.size()<<endl;
        key += 0.04;

        if((checkBoxStateBus & 3) == 2)
            tar_Pos->addData(key,targetPosQueue.front());
        if((checkBoxStateBus & 12) >> 2 == 2)
            actual_Pos->addData(key,actualPosQueue.front());
        if((checkBoxStateBus & 48) >> 4 == 2)
            tar_Vel->addData(key,targetVelQueue.front()/10.0);
        if((checkBoxStateBus & 192) >> 6 == 2)
            actual_Vel->addData(key,actualVelQueue.front()/10.0);
        if((checkBoxStateBus & 768) >> 8 == 2)
            tar_Iq->addData(key,targetIqQueue.front()/10.0);
        if((checkBoxStateBus & 3072) >> 10 == 2)
            actual_Iq->addData(key,actualIqQueue.front()/10.0);
//                if((checkBoxStateBus & 12288) >> 12 == 2)
//                    tar_Id->addData(key,targetIdQueue.front());
//                if((checkBoxStateBus & 49152) >> 14 == 2)
//                    actual_Id->addData(key,actualIdQueue.front());
        targetPosQueue.pop_front();
        actualPosQueue.pop_front();
        targetVelQueue.pop_front();
        actualVelQueue.pop_front();
        targetIqQueue.pop_front();
        actualIqQueue.pop_front();

        if(((checkBoxStateBus & 786432) >> 18 == 2))
            customPlot->xAxis->setRange(key, xAxisRange, Qt::AlignRight);

    }
}

// save memory, from me on!
void GraphWindow::tarPosDataSlot(int state)
{
    checkBoxStateBus &= (uint32_t)(0xFFFFFFFF-3);
    checkBoxStateBus |= (uint32_t)state;
}

void GraphWindow::actuPosDataSlot(int state)
{
    checkBoxStateBus &= (uint32_t)(0xFFFFFFFF-12);
    checkBoxStateBus |= ((uint32_t)state << 2);
}

void GraphWindow::tarVelDataSlot(int state)
{
    checkBoxStateBus &= (uint32_t)(0xFFFFFFFF-48);
    checkBoxStateBus |= ((uint32_t)state << 4);
}

void GraphWindow::actuVelDataSlot(int state)
{
    checkBoxStateBus &= (uint32_t)(0xFFFFFFFF-192);
    checkBoxStateBus |= ((uint32_t)state << 6);
}

void GraphWindow::tarIqDataSlot(int state)
{
    checkBoxStateBus &= (uint32_t)(0xFFFFFFFF-768);
    checkBoxStateBus |= ((uint32_t)state << 8);
}

void GraphWindow::actuIqDataSlot(int state)
{
    checkBoxStateBus &= (uint32_t)(0xFFFFFFFF-3072);
    checkBoxStateBus |= ((uint32_t)state << 10);
}

void GraphWindow::tarIdDataSlot(int state)
{
    checkBoxStateBus &= (uint32_t)(0xFFFFFFFF-12288);
    checkBoxStateBus |= ((uint32_t)state << 12);
}

void GraphWindow::actuIdDataSlot(int state)
{
    checkBoxStateBus &= (uint32_t)(0xFFFFFFFF-49152);
    checkBoxStateBus |= ((uint32_t)state << 14);
}

void GraphWindow::posModeChanged(int state)
{
    checkBoxStateBus &= (uint32_t)(0xFFFFFFFF-196608);
    checkBoxStateBus |= ((uint32_t)state << 16);
}

void GraphWindow::dynamicModeChanged(int state)
{
    checkBoxStateBus &= (uint32_t)(0xFFFFFFFF-786432);
    checkBoxStateBus |= ((uint32_t)state << 18);
}

void GraphWindow::manualModeChanged(int state)
{
    checkBoxStateBus &= (uint32_t)(0xFFFFFFFF-3145728);
    checkBoxStateBus |= ((uint32_t)state << 20);
}

void GraphWindow::sliderXValueChanged(int value)
{
    xAxisRange = value+1;
}

void GraphWindow::sliderPosValueChanged(int value)
{
    targetPos = value/99.0*360;
    if(((checkBoxStateBus & 3145728) >> 20 == 2) && ((checkBoxStateBus & 196608) >> 16 == 2))
        sendCmdCANMsg();
}

void GraphWindow::sliderVelValueChanged(int value)
{
    targetVel = value/99.0*20;
    if(((checkBoxStateBus & 3145728) >> 20 == 2) && ((checkBoxStateBus & 196608) >> 16 == 0))
        sendCmdCANMsg();
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
    customPlot->graph(0)->setPen(QPen(Qt::black,1.5, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));

    QVector<double> x_2(101), y_2(101);
    for (int i=0; i<101; ++i)
    {
        x_2[i] = x_;
        y_2[i] = customPlot->yAxis->range().lower+
                i*(customPlot->yAxis->range().upper-customPlot->yAxis->range().lower)/101;
    }
    customPlot->graph(1)->setData(x_2,y_2);
    customPlot->graph(1)->setPen(QPen(Qt::black,1.5, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));

    customPlot->replot();

    QString str = QString("  X: %1\n""  Y: %2").arg(QString::number(x_,10,3)).arg(QString::number(y_,10,3));
    QToolTip::showText(cursor().pos(),str);
}

void GraphWindow::easterEggDataSlot()
{
    // read the csv pointXYdata
    // read a frame each slot
    if(isFileOpened)
    {
        // read a line which contains the data in this frame
        QStringList stringList = textStream->readLine().split(",");
        QVector<double> point_x((stringList.size()-1)/2), point_y((stringList.size()-1)/2);
        for (int i=0; i<stringList.size()-1-1; i+=2)
        {
            point_y[i/2] = (1.2-stringList[i].toShort()/1080.0*2.4);
            point_x[i/2] = (stringList[i+1].toShort()/1920.0*16);
        }
        customPlot->graph(2)->setData(point_x,point_y);
        customPlot->replot();
    }
    // the easter egg is finished
    if(++frameCount == 675){
        frameCount = 0;
        isPlayEasterEgg = 0;
        textStream->seek(0);
        customPlot->graph(2)->data().clear();
        customPlot->replot();
        easterEggTimer->stop();
    }
}

void GraphWindow::startEasterEggSlot()
{
    short static PosionedCounter = 0;
    if(frameCount == 0){
        easterEggTimer->start(50);
        isPlayEasterEgg = 1;
        customPlot->xAxis->setRange(16, 16, Qt::AlignRight);
        customPlot->replot();
        if((PosionedCounter++)%2 == 0)
            QMessageBox::information(this,"","恭喜你发现了宝藏！",NULL);
        else
            QMessageBox::information(this,"","中毒了吧！再看一遍？",NULL);
    }
}

void GraphWindow::sendCmdCANMsg(void)
{
    static CanMessage msg;
    static uint8_t data_int[8];
    static uint32_t address;
    static bool en_extended = false;
    static uint8_t dlc = 8;
    if((checkBoxStateBus & 196608) >> 16 == 2){
//        Pos Cmd Mode
        data_int[0] = (uint8_t)(((uint16_t)(targetPos*10))>>8);
        data_int[1] = (uint8_t)((uint16_t)(targetPos*10));
        data_int[2] = 0;
        data_int[3] = 0;
        data_int[4] = 0;
        data_int[5] = 0;
        data_int[6] = 0;
        data_int[7] = 0;
        address = 0x121;
    }else{
//        Velocity Cmd Mode
        data_int[0] = 0;
        data_int[1] = 0;
        data_int[2] = 0;
        data_int[3] = 0;
        data_int[4] = 0;
        data_int[5] = 0;
        data_int[6] = 0;
        data_int[7] = 0;
        address = 0x212;
    }

    // If address is beyond std address namespace, force extended
    if(address > 0x7ff)
    {
        en_extended = true;
    }

    msg.setData(data_int[0],data_int[1],data_int[2],data_int[3],data_int[4],data_int[5],data_int[6],data_int[7]);
    msg.setId(address);
    msg.setLength(dlc);

    msg.setExtended(en_extended);
    msg.setRTR(false);
    msg.setErrorFrame(false);

    CanTrace *trace = _backend.getTrace();
    foreach (CanInterfaceId ifid, _backend.getInterfaceList()) {
        CanInterface *intf = _backend.getInterfaceById(ifid);
        int tx_ret = intf->sendMessage(msg);
        if (tx_ret) {
            //msg.setTX(true);
            //trace->enqueueMessage(msg, false);
        }

        char outmsg[256];
        snprintf(outmsg, 256, "Send %s [%s] to %d on port %s [ext=%u rtr=%u err=%u] ",
                 tx_ret ? "ok" : "err",msg.getDataHexString().toLocal8Bit().constData(), msg.getId(), _backend.getInterfaceById(ifid)->getName().toLocal8Bit().constData(),
                 msg.isExtended(), msg.isRTR(), msg.isErrorFrame());

        if (tx_ret)
            log_info(outmsg);
        else
            log_error(outmsg);
    }
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

    delete easterEggTimer;
    delete file;
    delete textStream;
}
