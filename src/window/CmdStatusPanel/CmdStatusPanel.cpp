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
#include "CmdStatusPanel.h"
#include "ui_CmdStatusPanel.h"

#include <QDomDocument>
#include <core/Backend.h>

CmdStatusPanelWindow::CmdStatusPanelWindow(QWidget *parent, Backend &backend) :
    ConfigurableWidget(parent),
    ui(new Ui::CmdStatusPanelWindow),
    _backend(backend)
{
    ui->setupUi(this);


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), ui->dial, SLOT(valueChanged()));
    timer->start(100);
}

CmdStatusPanelWindow::~CmdStatusPanelWindow()
{
    delete ui;
    delete dial;
    delete timer;
}
