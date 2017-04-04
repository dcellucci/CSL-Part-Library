/*
  RTC library for Arduino Zero.
  Copyright (c) 2015 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef RTC_CSL_H
#define RTC_CSL_H

#include "Arduino.h"

typedef void(*voidFuncPtr)(void);

class RTC_CSL {
public:


  RTC_CSL();
  void begin(bool resetTime = false);

  void attachInterrupt(voidFuncPtr callback);
  void detachInterrupt();

  void standbyMode();

  bool isConfigured() {
    _configured = RTC->MODE0.CTRL.reg & RTC_MODE0_CTRL_ENABLE;
    configureClock();
    return _configured;
  }

  uint32_t getCount();
  void setCount(uint32_t newCount);

private:
  bool _configured;

  void config32kOSC(void);
  void configureClock(void);
  void RTCreadRequest();
  bool RTCisSyncing(void);
  void RTCdisable();
  void RTCenable();
  void RTCreset();
  void RTCresetRemove();
};

#endif // RTC_ZERO_H
