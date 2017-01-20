/******************************************************************************
dps310.h
Header file for DPS310 breakout board Arduino Library.

Daniel Cellucci, Coded Structures Lab, NASA Ames Research Center
October 21, 2016

Arduino library supporting DPS310 breakout board.

Resources:
Written using Sensor Sticker V1 with a DPS310 breakout board.

Development environment specifics:
Written using Arduino 1.8.1

This code is released under the [MIT License](http://opensource.org/licenses/MIT).

Distributed as-is; no warranty is given.
******************************************************************************/

/*
TODO
  1) More intelligent mapping between oversampling rate register and
     scale factor (see table 4.9.3)
*/


#pragma once

#ifndef _DPS310_H_
#define _DPS310_H_

#include <stdint.h>

class DPS310
{
public:
  DPS310(uint8_t address = 0x77);

  // Initialization routines
  void    init();
  void    enableDefaultSettings();
  void    reset();

  int     readPressureRaw();
  int     readTemperatureRaw(); 

  float  readCalibratedTemp();
  double  readCalibratedPressure();

  int* getCalibration();

protected:
  // private methods
  void    readCoeff();
  uint8_t ReadReg(uint8_t reg);
  void    WriteReg(uint8_t reg, uint8_t val);
  int coeff[9];

  float pscalefactor = 1040384.0;
  float tscalefactor = 7864320.0;

  // private data
  uint8_t _address;


};

#endif
