/******************************************************************************
Lp5521.cpp
Implementation file for Lp5521 breakout board Arduino Library.

Byron Jacquot @ SparkFun Electronics
October 21, 2016
https://github.com/sparkfun/SparkFun_Lp5521_Arduino_Library

Arduino library supporting Lp5521 breakout board.

Can be instanciated two different ways:
As an Lp5521 object, offering simple control over the IC, with minimum ROM footprint.
As an Lp5521Engines object, adding diagnostic and execution engine support.

Resources:
Written using SparkFun Pro Micro controller, with an Lp5521 breakout board.
Development environment specifics:
Written using Arduino 1.6.5

This code is released under the [MIT License](http://opensource.org/licenses/MIT).

Please review the LICENSE.md file included with this example. If you have any questions
or concerns with licensing, please contact techsupport@sparkfun.com.

Distributed as-is; no warranty is given.
******************************************************************************/


#include <Arduino.h>
#include <Wire.h>

#include "lp5521.h"

// register stuff
static const uint8_t REG_ENABLE = 0x00;
static const uint8_t REG_OPMODE = 0x01;
static const uint8_t REG_CONFIG = 0x08;
static const uint8_t REG_STATUS = 0x0C;
static const uint8_t REG_RESET  = 0x0D;


//LED control registers
static const uint8_t REG_R_PWM  = 0x02;
static const uint8_t REG_G_PWM  = 0x03;
static const uint8_t REG_B_PWM  = 0x04;
static const uint8_t REG_R_CUR  = 0x05;
static const uint8_t REG_G_CUR  = 0x06;
static const uint8_t REG_B_CUR  = 0x07;
static const uint8_t REG_R_PC   = 0x09;
static const uint8_t REG_G_PC   = 0x0A;
static const uint8_t REG_B_PC   = 0x0B;

/********************************************************************************/
//  Lp5521: The Simple base class.
//
// allows direct control over the LED outputs, and basic chip featurers like output current setting,
/********************************************************************************/

Lp5521::Lp5521(uint8_t address)
{
  _address = address;
}

void Lp5521::Begin()
{
  Wire.begin();

  Reset();
}

void Lp5521::Enable()
{
  // Set enable bit
  WriteReg(REG_ENABLE, 0x40);
  WriteReg(REG_OPMODE, 0x3F);

  // enable internal clock & charge pump & write auto increment
  WriteReg(REG_CONFIG, 0x13);
}

uint32_t Lp5521::getStatus()
{
  return (ReadReg(REG_ENABLE)<<16)|(ReadReg(REG_OPMODE)<<8)|(ReadReg(REG_CONFIG));
}

void Lp5521::Disable()
{
  uint8_t val;

  val = ReadReg(REG_ENABLE);
  val &= ~0x40;
  WriteReg(REG_ENABLE, val);
}

void Lp5521::Reset()
{
  // force reset
  WriteReg(REG_RESET, 0xff);
}

Lp5521::lp_err_code Lp5521::SetChannelPWM(uint8_t channel, uint8_t value)
{
  if(channel >= NumChannels)
  {
    return LP_ERR_INVALID_CHANNEL;
  }

  WriteReg(REG_R_PWM + channel, value);
  return LP_ERR_NONE;
}

Lp5521::lp_err_code Lp5521::SetLogBrightness(bool enable)
{
  uint8_t regVal, bitVal;

  regVal = ReadReg(REG_ENABLE);
  bitVal = enable?0x80:0x00;
  regVal &= ~0x80;
  regVal |= bitVal;
  WriteReg(REG_ENABLE, regVal);

  return LP_ERR_NONE;
}

Lp5521::lp_err_code Lp5521::SetDriveCurrent(uint8_t channel, uint8_t value)
{
  if(channel >= NumChannels)
  {
    return LP_ERR_INVALID_CHANNEL;
  }

  WriteReg(REG_R_CUR + channel, value);
  return LP_ERR_NONE;
}

/********************************************************************************/
/**  private base class member functions. **/
/********************************************************************************/

uint8_t Lp5521::ReadReg(uint8_t reg)
{
  // Wire is awkward because it doesn't really have a register address concept.
  // http://www.arduino.cc/en/Tutorial/SFRRangerReader for reference

  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission(false);// false keeps connection active so we can read.

  delayMicroseconds(10);

  uint8_t status = Wire.requestFrom(_address, (uint8_t)1);
  if(status)
  {
    return(Wire.read());
  }
  else
  {
    Serial.print("readReg failed? status:");
    Serial.println(status, HEX);
  }
  return 0xff;
}

void Lp5521::WriteReg(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

