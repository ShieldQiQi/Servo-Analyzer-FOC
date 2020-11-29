/*

  Copyright (c) 2015, 2016 Hubert Denkmair <hubert@denkmair.de>

  This file is part of cangaroo.

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

#include "ConfigurableWidget.h"
#include <QDomDocument>
#include <core/Backend.h>

ConfigurableWidget::ConfigurableWidget(QWidget *parent) : QWidget(parent)
{

}

bool ConfigurableWidget::saveXML(Backend &backend, QDomDocument &xml, QDomElement &root)
{
    (void) backend;
    (void) xml;
    (void) root;
    return true;
}

bool ConfigurableWidget::loadXML(Backend &backend, QDomElement &el)
{
    (void) backend;
    (void) el;
    return true;
}
