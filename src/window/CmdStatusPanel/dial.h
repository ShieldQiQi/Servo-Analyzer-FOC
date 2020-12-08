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
#ifndef DIAL_H
#define DIAL_H

#include <QWidget>
#include <QDial>

class Dial : public QWidget
{
    Q_OBJECT
public:
    explicit Dial(QWidget *parent = nullptr);

private:
    int value;
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void valueChanged();
};

#endif // DIAL_H
