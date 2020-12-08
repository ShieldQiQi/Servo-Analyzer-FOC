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

#include <qcustomplot/qcustomplot.h>

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

    void myMoveEvent(QMouseEvent *event);

private slots:

//    void on_Vel_clicked();
//    void on_Pos_clicked();
//    void on_Iq_clicked();
//    void on_Id_clicked();
    void realtimeDataSlot();

private:

    Ui::GraphWindow *ui;
    Backend &_backend;
};
