/******************************************************************************
lp55231.h
Header file for LP55231 breakout board Arduino Library.

Byron Jacquot @ SparkFun Electronics
October 21, 2016
https://github.com/sparkfun/SparkFun_LP55231_Arduino_Library

Arduino library supporting LP55231 breakout board.

Can be instanciated two different ways:
As an Lp55231 object, offering simple control over the IC, with minimum ROM footprint.
As an Lp55231Engines object, adding diagnostic and execution engine support.

Resources:
Written using SparkFun Pro Micro controller, with an LP55231 breakout board.
Development environment specifics:
Written using Arduino 1.6.5

This code is released under the [MIT License](http://opensource.org/licenses/MIT).

Please review the LICENSE.md file included with this example. If you have any questions
or concerns with licensing, please contact techsupport@sparkfun.com.

Distributed as-is; no warranty is given.
******************************************************************************/

#pragma once

#ifndef _LP5521_H_
#define _LP5521_H_

#include <stdint.h>


struct coefficients
{
  uint32_t c0,
  uint32_t c1,
  uint32_t c00,
  uint32_t c10,
  uint32_t c11,
  uint32_t c20,
  uint32_t c21,
  uint32_t c30,
};



class Lp5521
{
public:
  Lp5521(uint8_t address = 0x64);

  coefficients *coeff


  // Initialization routines
  void Begin();
  void Enable();
  void Disable();
  void Reset();
  uint32_t getStatus();

  // control outputs directly
  lp_err_code SetChannelPWM(uint8_t channel, uint8_t value);
  lp_err_code SetMasterFader(uint8_t fader, uint8_t value);

  // More detailed channel configuration
  lp_err_code SetLogBrightness( bool enable);
  lp_err_code SetDriveCurrent(uint8_t channel, uint8_t value);


protected:
  // private methods

  uint8_t ReadReg(uint8_t reg);
  void    WriteReg(uint8_t reg, uint8_t val);

  // private data
  uint8_t _address;

};

#endif
