/*

  Copyright (c) 2016 Hubert Denkmair <hubert@denkmair.de>

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

#pragma once

#include <core/Backend.h>
#include <driver/CanDriver.h>
#include "api/candle.h"

class CandleApiInterface;
class GenericCanSetupPage;

class CandleApiDriver : public CanDriver
{
public:
    CandleApiDriver(Backend &backend);

    virtual QString getName();
    virtual bool update();

private:
    GenericCanSetupPage *setupPage;
    CandleApiInterface *findInterface(candle_handle dev);

};

