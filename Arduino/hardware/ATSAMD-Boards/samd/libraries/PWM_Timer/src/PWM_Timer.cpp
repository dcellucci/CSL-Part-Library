/*
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

#if defined(ARDUINO_ARCH_SAMD)

#include <Arduino.h>
#include <WVariant.h>
#include <PWM_Timer.h>
#include <TCC0_Config.h>

#define usToTicks(_us)    ((clockCyclesPerMicrosecond() * _us) / 16)                 // converts microseconds to tick
#define ticksToUs(_ticks) (((unsigned) _ticks * 16) / clockCyclesPerMicrosecond())   // converts from ticks back to microseconds

#define TRIM_DURATION  5                                   // compensation ticks to trim adjust for digitalWrite delays

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)   // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)   // maximum value in uS for this servo

//Borrowed from ASF
#define TCC_CONFIGURATION(n)                                                                                           \
  {                                                                                                                  \
    n, TCC_CTRLA_PRESCALER(CONF_TCC##n##_PRESCALER) | (CONF_TCC##n##_RUNSTDBY << TCC_CTRLA_RUNSTDBY_Pos)           \
           | TCC_CTRLA_PRESCSYNC(CONF_TCC##n##_PRESCSYNC) | (CONF_TCC##n##_ALOCK << TCC_CTRLA_ALOCK_Pos)           \
           | (CONF_TCC##n##_CPTEN0 << TCC_CTRLA_CPTEN0_Pos) | (CONF_TCC##n##_CPTEN1 << TCC_CTRLA_CPTEN1_Pos)       \
           | (CONF_TCC##n##_CPTEN2 << TCC_CTRLA_CPTEN2_Pos) | (CONF_TCC##n##_CPTEN3 << TCC_CTRLA_CPTEN3_Pos),      \
        (CONF_TCC##n##_LUPD << TCC_CTRLBSET_LUPD_Pos), (CONF_TCC##n##_DBGRUN << TCC_DBGCTRL_DBGRUN_Pos),           \
        TCC_EVCTRL_CNTSEL(CONF_TCC##n##_CNTSEL) | (CONF_TCC##n##_OVFEO << TCC_EVCTRL_OVFEO_Pos)                    \
            | (CONF_TCC##n##_TRGEO << TCC_EVCTRL_TRGEO_Pos) | (CONF_TCC##n##_CNTEO << TCC_EVCTRL_CNTEO_Pos)        \
            | (CONF_TCC##n##_MCEO0 << TCC_EVCTRL_MCEO0_Pos) | (CONF_TCC##n##_MCEO1 << TCC_EVCTRL_MCEO1_Pos)        \
            | (CONF_TCC##n##_MCEO2 << TCC_EVCTRL_MCEO2_Pos) | (CONF_TCC##n##_MCEO3 << TCC_EVCTRL_MCEO3_Pos)        \
            | (CONF_TCC##n##_MCEI0 << TCC_EVCTRL_MCEI0_Pos) | (CONF_TCC##n##_MCEI1 << TCC_EVCTRL_MCEI1_Pos)        \
            | (CONF_TCC##n##_MCEI2 << TCC_EVCTRL_MCEI2_Pos) | (CONF_TCC##n##_MCEI3 << TCC_EVCTRL_MCEI3_Pos)        \
            | (CONF_TCC##n##_TCEI0 << TCC_EVCTRL_TCEI0_Pos) | (CONF_TCC##n##_TCEI1 << TCC_EVCTRL_TCEI1_Pos)        \
            | (CONF_TCC##n##_TCINV0 << TCC_EVCTRL_TCINV0_Pos) | (CONF_TCC##n##_TCINV1 << TCC_EVCTRL_TCINV1_Pos)    \
            | TCC_EVCTRL_EVACT1(CONF_TCC##n##_EVACT1) | TCC_EVCTRL_EVACT0(CONF_TCC##n##_EVACT0),                   \
        CONF_TCC##n##_PER, (CONF_TCC##n##_WAVEGEN << TCC_WAVE_WAVEGEN_Pos), CONF_TCC##n##_CC0, CONF_TCC##n##_CC1, CONF_TCC##n##_CC2, CONF_TCC##n##_CC3              \
  }

#define TCC_PWM_CONFIGURATION(n)                                                                                       \
  {                                                                                                                  \
    n, CONF_TCC##n##_SEL_CH, CONF_TCC##n##_PER_REG, CONF_TCC##n##_CCX_REG,                                         \
        (CONF_TCC##n##_WAVEGEN << TCC_WAVE_WAVEGEN_Pos),                                                           \
  }

struct tcc_configuration {
   uint8_t number;
  uint32_t ctrl_a;
   uint8_t ctrl_b;
   uint8_t dbg_ctrl;
  uint32_t event_ctrl;
  uint32_t per;
  uint32_t wave;
  uint32_t cc0;
  uint32_t cc1;
  uint32_t cc2;
  uint32_t cc3;
};

static struct tcc_configuration tcc0_config = TCC_CONFIGURATION(0);

static inline void tcc_wait_for_sync(const void *const hw, uint32_t reg)
{
  while (((Tcc *)hw)->SYNCBUSY.reg & reg) {
  };
}


/****************** end of static functions ******************************/

PWM_Timer::PWM_Timer()
{
#ifndef TCC_ENABLED
  #define TCC_ENABLED
  
  //Enable bus clock for the power manager
  uint32_t peripheral = ((uint32_t)TCC0 & 0x0000ff00) >> 10;
  PM->APBCMASK.reg |= 1 << peripheral;
  
  //Enable GClk channel
  GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_ID(TCC0_GCLK_ID) | GCLK_CLKCTRL_GEN(GCLK_CLKCTRL_GEN_GCLK0_Val) | (1 << GCLK_CLKCTRL_CLKEN_Pos)) ;
  
  //Initialize PWM functionality
  tcc_wait_for_sync(TCC0,TCC_SYNCBUSY_SWRST);
  //set software reset bit on the Control A register
  TCC0->CTRLA.reg |= TCC_CTRLA_SWRST;  
  tcc_wait_for_sync(TCC0,TCC_SYNCBUSY_SWRST);
  //Write Control A Register
  TCC0->CTRLA.reg  = tcc0_config.ctrl_a;
  //Set Control B Register
  TCC0->CTRLBSET.reg = tcc0_config.ctrl_b;
  //Debug Control Register
  TCC0->DBGCTRL.reg = tcc0_config.dbg_ctrl;
  //Event Control Register
  TCC0->EVCTRL.reg = tcc0_config.event_ctrl;
  //Waveform register
  TCC0->WAVE.reg = tcc0_config.wave;
  //Period Register
  TCC0->PER.reg = tcc0_config.per;
  TCC0->CC[0].reg = 20000;

  //Clear lock update for the Control B register
  TCC0->CTRLBCLR.reg = TCC_CTRLBSET_LUPD;

  

  tcc_wait_for_sync(TCC0,TCC_SYNCBUSY_SWRST | TCC_SYNCBUSY_ENABLE);
  TCC0->CTRLA.reg |= TCC_CTRLA_ENABLE;


  
  num_channels = 0;
#endif

}

uint8_t PWM_Timer::attach(int pin){
  PinDescription pinDesc = g_APinDescription[pin];

  //Set pin functionality (crustom)
  if(num_channels < MAX_PWM_CHANNELS && chan_pin_map[(pinDesc.ulTCChannel)] == 0){
    chan_pin_map[pinDesc.ulTCChannel] = pin;
    uint8_t tmp = PORT->Group[pinDesc.ulPort].PINCFG[pinDesc.ulPin].reg;
    tmp &= ~PORT_PINCFG_PMUXEN;
    tmp |= 1 << PORT_PINCFG_PMUXEN_Pos;
    PORT->Group[pinDesc.ulPort].PINCFG[pinDesc.ulPin].reg = tmp;

    if (pin & 1) {
      // Odd numbered pin
      tmp = PORT->Group[pinDesc.ulPort].PMUX[pinDesc.ulPin>>1].reg;
      tmp &= ~PORT_PMUX_PMUXO_Msk;
      tmp |= PORT_PMUX_PMUXO((uint8_t)(PINMUX_PA04F_TCC0_WO0 & 0xffff));
      PORT->Group[pinDesc.ulPort].PMUX[pinDesc.ulPin>>1].reg = tmp;
    } else {
      // Even numbered pin
      tmp = PORT->Group[pinDesc.ulPort].PMUX[pinDesc.ulPin>>1].reg;
      tmp &= ~PORT_PMUX_PMUXE_Msk;
      tmp |= PORT_PMUX_PMUXE((uint8_t)(PINMUX_PA04F_TCC0_WO0 & 0xffff));
      PORT->Group[pinDesc.ulPort].PMUX[pinDesc.ulPin>>1].reg = tmp;
    }

    tcc_vals[pinDesc.ulTCChannel] = usToTicks(DEFAULT_PULSE_WIDTH);

    update_tcc_configuration();

    num_channels++;
    return pin;
  }
  else{
    return -1;
  }
}



void PWM_Timer::detach(int pin)
{
  
}

void PWM_Timer::write(int pin, int value)
{
  // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
  if (value < MIN_PULSE_WIDTH)
  {
    if (value < 0)
      value = 0;
    else if (value > 180)
      value = 180;

    value = map(value, 0, 180, SERVO_MIN(), SERVO_MAX());
  }
  writeMicroseconds(pin,value);
}

void PWM_Timer::writeMicroseconds(int pin, int value){
  int chan = channel_from_pin(pin);
  if(chan != -1){
    tcc_vals[chan] = usToTicks(value);  
    update_tcc_configuration();
  }


}

void PWM_Timer::update_tcc_configuration(){
  tcc0_config.cc0 = tcc_vals[0];
  tcc0_config.cc1 = tcc_vals[1];
  tcc0_config.cc2 = tcc_vals[2];
  tcc0_config.cc3 = tcc_vals[3];

  TCC0->CCB[0].reg = tcc0_config.cc0;
  TCC0->CCB[1].reg = tcc0_config.cc1;
  TCC0->CCB[2].reg = tcc0_config.cc2;
  TCC0->CCB[3].reg = tcc0_config.cc3;
}

uint8_t PWM_Timer::channel_from_pin(int pin){
  for(int i = 0; i<4; i++)
    if(chan_pin_map[i]==pin)
      return i;
  return -1;
}

#endif // ARDUINO_ARCH_SAMD
