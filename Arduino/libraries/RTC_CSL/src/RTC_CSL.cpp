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

#include <time.h>

#include "RTC_CSL.h"

voidFuncPtr RTC_callBack = NULL;

RTC_CSL::RTC_CSL()
{
  _configured = false;

}

void RTC_CSL::begin(bool resetTime)
{
  uint16_t tmp_reg = 0;

  PM->APBAMASK.reg |= PM_APBAMASK_RTC; // turn on digital interface clock

  //We should only do this if we haven't configured a 32k oscillator already
  //config32kOSC();

  // If the RTC is in clock mode and the reset was
  // not due to POR or BOD, preserve the clock time
  // POR causes a reset anyway, BOD behaviour is?
  bool validTime = false;
  RTC_MODE0_COUNT_Type oldCount;

  if ((!resetTime) && (PM->RCAUSE.reg & (PM_RCAUSE_SYST | PM_RCAUSE_WDT | PM_RCAUSE_EXT))) {
    if (RTC->MODE0.CTRL.reg & RTC_MODE0_CTRL_MODE_CLOCK) {
      validTime = true;
      oldCount.reg = RTC->MODE0.COUNT.reg;
    }
  }

  // Setup clock GCLK2 with OSC32K divided by 32
  //And here as well...
  configureClock();

  RTCdisable();

  RTCreset();

  tmp_reg |= RTC_MODE0_CTRL_MODE_COUNT32; // set clock operating mode
  tmp_reg |= RTC_MODE0_CTRL_PRESCALER_DIV128; // set prescaler to 1024 for MODE2
  tmp_reg &= ~RTC_MODE0_CTRL_MATCHCLR; // disable clear on match


  RTC->MODE0.READREQ.reg &= ~RTC_READREQ_RCONT; // disable continuously mode

  RTC->MODE0.CTRL.reg = tmp_reg;
  while (RTCisSyncing())
    ;

  //NVIC_EnableIRQ(RTC_IRQn); // enable RTC interrupt
  //NVIC_SetPriority(RTC_IRQn, 0x00);

  //RTC->MODE0.INTENSET.reg |= RTC_MODE0_INTENSET_CMP0; // enable alarm interrupt

  //while (RTCisSyncing())
  //  ;

  RTCenable();
  RTCresetRemove();

  // If desired and valid, restore the time value
  if ((!resetTime) && (validTime)) {
    RTC->MODE0.COUNT.reg = oldCount.reg;
    while (RTCisSyncing())
      ;
  }
  else
    setCount(0);

  _configured = true;
}

//
// Get/Set functions
//

uint32_t RTC_CSL::getCount(){
  RTCreadRequest();
  return RTC->MODE0.COUNT.bit.COUNT;
}

void RTC_CSL::setCount(uint32_t newCount){
  if(_configured){
    RTC->MODE0.COUNT.bit.COUNT = newCount;
    while(RTCisSyncing())
      ;
  }
}

void RTC_Handler(void)
{
  if (RTC_callBack != NULL) {
    RTC_callBack();
  }

  RTC->MODE0.INTFLAG.reg = RTC_MODE0_INTFLAG_CMP0; // must clear flag at end
}

void RTC_CSL::attachInterrupt(voidFuncPtr callback)
{
  RTC_callBack = callback;
}

void RTC_CSL::detachInterrupt()
{
  RTC_callBack = NULL;
}

void RTC_CSL::standbyMode()
{
  // Entering standby mode when connected
  // via the native USB port causes issues.
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  __WFI();
}

/* Attach peripheral clock to 32k oscillator */
void RTC_CSL::configureClock() {
  //GCLK->GENDIV.reg = GCLK_GENDIV_ID(2)|GCLK_GENDIV_DIV(0);
  //while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
  //  ;
  //GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_XOSC32K | GCLK_GENCTRL_ID(4));// | GCLK_GENCTRL_DIVSEL );
  //while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
  //  ;
  GCLK->CLKCTRL.reg = (uint32_t)((GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK1 | (RTC_GCLK_ID << GCLK_CLKCTRL_ID_Pos)));
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;
}

/*
 * Private Utility Functions
 */

/* Configure the 32768Hz Oscillator */
void RTC_CSL::config32kOSC()
{
  SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_ONDEMAND |
                         SYSCTRL_XOSC32K_RUNSTDBY |
                         SYSCTRL_XOSC32K_EN32K |
                         SYSCTRL_XOSC32K_XTALEN |
                         SYSCTRL_XOSC32K_STARTUP(6) |
                         SYSCTRL_XOSC32K_ENABLE;
}

/* Synchronise the CLOCK register for reading*/
inline void RTC_CSL::RTCreadRequest() {
  if (_configured) {
    RTC->MODE0.READREQ.reg = RTC_READREQ_RREQ;
    while (RTCisSyncing())
      ;
  }
}

/* Wait for sync in write operations */
inline bool RTC_CSL::RTCisSyncing()
{
  return (RTC->MODE0.STATUS.bit.SYNCBUSY);
}

void RTC_CSL::RTCdisable()
{
  RTC->MODE0.CTRL.reg &= ~RTC_MODE0_CTRL_ENABLE; // disable RTC
  while (RTCisSyncing())
    ;
}

void RTC_CSL::RTCenable()
{
  RTC->MODE0.CTRL.reg |= RTC_MODE0_CTRL_ENABLE; // enable RTC
  while (RTCisSyncing())
    ;
}

void RTC_CSL::RTCreset()
{
  RTC->MODE0.CTRL.reg |= RTC_MODE0_CTRL_SWRST; // software reset
  while (RTCisSyncing())
    ;
}

void RTC_CSL::RTCresetRemove()
{
  RTC->MODE0.CTRL.reg &= ~RTC_MODE0_CTRL_SWRST; // software reset remove
  while (RTCisSyncing())
    ;
}
