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
#pragma once

#include <core/Backend.h>
#include <core/ConfigurableWidget.h>
#include <core/MeasurementSetup.h>
#include <QFile>
#include <QTextStream>
#include <QQueue>
#include <qcustomplot/qcustomplot.h>
#include <QDebug>


// the CAN-Lib header files
#include <core/Backend.h>
#include <core/ConfigurableWidget.h>
#include <core/MeasurementSetup.h>
#include <driver/CanInterface.h>
#include <core/CanTrace.h>

namespace Ui {
class GraphWindow;
}

class GraphWindow : public ConfigurableWidget
{
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent, Backend &backend);
    ~GraphWindow();

    // use qcustomplot for graph analyzer
    int paint_Flag = 0;
    int timer_count = 0;
    QCustomPlot *customPlot;
    QCPCurve *tar_Vel;
    QCPCurve *actual_Vel;
    QCPCurve *tar_Pos;
    QCPCurve *actual_Pos;
    QCPCurve *tar_Iq;
    QCPCurve *actual_Iq;
    QCPCurve *tar_Id;
    QCPCurve *actual_Id;
    QTimer   *dataTimer;
    QTimer   *easterEggTimer;
    short    frameCount = 0;

    QTextStream *textStream;
    QFile *file;
    bool isFileOpened = 0;
    bool isPlayEasterEgg  = 0;

    uint32_t checkBoxStateBus = 0;

    // manual mode parameters
    float xAxisRange = 5;
    float targetPos = 0.0;
    float targetVel = 0.0;

    // parameters from devices
    QQueue<float> targetPosQueue;
    QQueue<float> actualPosQueue;
    QQueue<float> targetVelQueue;
    QQueue<float> actualVelQueue;
    QQueue<float> targetIdQueue;
    QQueue<float> actualIdQueue;
    QQueue<float> targetIqQueue;
    QQueue<float> actualIqQueue;

    void sendCmdCANMsg(void);

    void myMoveEvent(QMouseEvent *event);
    Ui::GraphWindow *ui;

private slots:

//    void on_Vel_clicked();
//    void on_Pos_clicked();
//    void on_Iq_clicked();
//    void on_Id_clicked();
    void realtimeDataSlot();
    void easterEggDataSlot();

public slots:

    void startEasterEggSlot();

    void tarPosDataSlot(int state);
    void actuPosDataSlot(int state);
    void actuVelDataSlot(int state);
    void tarVelDataSlot(int state);
    void tarIqDataSlot(int state);
    void actuIqDataSlot(int state);
    void tarIdDataSlot(int state);
    void actuIdDataSlot(int state);
    void posModeChanged(int state);
    void dynamicModeChanged(int state);
    void manualModeChanged(int state);

    void sliderXValueChanged(int value);
    void sliderPosValueChanged(int value);
    void sliderVelValueChanged(int value);

    void DecodeCANMsg(QString string);

private:

    Backend &_backend;
};
