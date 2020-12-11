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
#include <QTimer>
#include "ui_CmdStatusPanel.h"
#include "dial.h"
#include <QDebug>

namespace Ui {
class CmdStatusPanelWindow;
}

class QDomDocument;
class QDomElement;

class CmdStatusPanelWindow: public ConfigurableWidget
{
    Q_OBJECT

public:
    explicit CmdStatusPanelWindow(QWidget *parent, Backend &backend);
    ~CmdStatusPanelWindow();

    Dial *dial;
    QTimer   *timer;

    Ui::CmdStatusPanelWindow *ui;

private slots:


private:
    Backend &_backend;
};
