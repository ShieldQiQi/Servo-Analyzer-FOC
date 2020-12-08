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
#include "dial.h"
#include<QPainter>
#include<QDebug>
#include<QSize>

extern double velocity;

Dial::Dial(QWidget *parent) : QWidget(parent){}

void Dial::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter.translate(this->width()/2, this->height()/2);
    QPixmap img = QPixmap(":/assets/background.png");
    painter.drawPixmap(-img.width()/2, -img.height()/2, img);

    QPixmap needle;
    needle.load(":/assets/needle.png");
    painter.rotate(value);
    painter.drawPixmap(-needle.width()/2, -needle.height(), needle);

    QPixmap needle_shadow;
    needle_shadow.load(":/assets/needle_shadow.png");
    painter.drawPixmap(-needle_shadow.width()/2, -needle_shadow.height(), needle_shadow);

    painter.rotate(-value);
    QPixmap overlay = QPixmap(":/assets/overlay.png");
    painter.drawPixmap(-img.width()/2 + 22, -img.height()/2 + 20, overlay);

    painter.restore();
}

void Dial::valueChanged()
{
    this->value = int(velocity*90);
    update();
}
