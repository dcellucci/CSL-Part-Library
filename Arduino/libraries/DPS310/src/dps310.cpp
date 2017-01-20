/******************************************************************************
dps310.h
Implementation file for DPS310 breakout board Arduino Library.

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

#include <Arduino.h>
#include <Wire.h>

#include "dps310.h"

// DATA REGISTERS

// Pressure
static const uint8_t     PSR_B2 = 0x00;
static const uint8_t     PSR_B1 = 0x01;
static const uint8_t     PSR_B0 = 0x02;

// Temperature
static const uint8_t     TMP_B2 = 0x03;
static const uint8_t     TMP_B1 = 0x04;
static const uint8_t     TMP_B0 = 0x05;

// Config
static const uint8_t    PRS_CFG = 0x06;
static const uint8_t    TMP_CFG = 0x07;
static const uint8_t   MEAS_CFG = 0x08;
static const uint8_t    CFG_REG = 0x09;

// Status
static const uint8_t    INT_STS = 0x0A;
static const uint8_t   FIFO_STS = 0x0B;

// Reset
static const uint8_t  REG_RESET = 0x0C;

// Information
static const uint8_t PRODUCT_ID = 0x0D;

// Coeff 
static const uint8_t COEFF_START= 0x10;
static const uint8_t COEFF_END  = 0x22;


//Code courtesy of Andrew Cooper
//http://stackoverflow.com/questions/13713506/converting-from-int-to-binary-string-to-int-using-2s-complement#13713606
int toSignedInt(uint32_t value, uint8_t bitLength){
  int signedValue = value;
  if(value >> (bitLength-1))
    signedValue |= -1 << bitLength;
  return signedValue;
}

long toSignedLong(uint32_t value, uint8_t bitLength){
  long signedValue = value;
  if(value >> (bitLength-1))
    signedValue |= -1 << bitLength;
  return signedValue;
}

/********************************************************************************/
//  DPS310
//
/********************************************************************************/

DPS310::DPS310(uint8_t address)
{
  _address = address;
}

void DPS310::init()
{
  Wire.begin();
  reset();
}

void DPS310::enableDefaultSettings()
{
  readCoeff();
  
  // pressure configuration
  // 8 measurements/second at 64 samples/measurement (high precision)
  WriteReg(PRS_CFG,0x36);

  // temperature configuration
  // 8 measurements/second at 8 samples/measurement (high precision)
  WriteReg(TMP_CFG,0x33);

  //Measurement Configuration
  uint8_t meas_cfg_val = ReadReg(MEAS_CFG);
  meas_cfg_val = (meas_cfg_val&0xF8)|0x07;
  WriteReg(MEAS_CFG,meas_cfg_val);

  //Overall Configuration
  WriteReg(CFG_REG,0x04);
}

int DPS310::readPressureRaw(){
  uint32_t rawPReg = (ReadReg(PSR_B2)<<16)|(ReadReg(PSR_B1)<<8)|(ReadReg(PSR_B0));
  return toSignedInt(rawPReg,24);
}

int DPS310::readTemperatureRaw(){
  return toSignedInt((ReadReg(TMP_B2)<<16)|(ReadReg(TMP_B1)<<8)|(ReadReg(TMP_B0)),24);
}

float DPS310::readCalibratedTemp(){
  int traw = readTemperatureRaw();
  float scaledT = (float)traw/tscalefactor;
  return 0.5*coeff[0]+1.0*coeff[1]*scaledT;
}

double DPS310::readCalibratedPressure(){
  int traw = readTemperatureRaw();
  int praw = readPressureRaw();
  double scaledT = (double)traw/tscalefactor;
  double scaledP = (double)praw/pscalefactor;
  return coeff[2] + scaledP*(coeff[3]+scaledP*(coeff[6]+scaledP*coeff[8])) + scaledT*coeff[4] + scaledT*scaledP*(coeff[5]+scaledP*coeff[7]);
}

int* DPS310::getCalibration(){
  return coeff;
}

void DPS310::reset()
{
  // force reset
  WriteReg(REG_RESET, 0x09);
}

/********************************************************************************/
/**  private base class member functions. **/
/********************************************************************************/

uint8_t DPS310::ReadReg(uint8_t reg)
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

void DPS310::WriteReg(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

/*
Probably a more efficient way to go from 2s complement to signed ints??
*/
void DPS310::readCoeff(){
  uint8_t calibration[18];
  for(uint8_t i = COEFF_START; i < COEFF_END; i++){
    calibration[i-COEFF_START] = ReadReg(i);
    delay(1);
  }
  uint32_t tcoeff[9];
  tcoeff[0] = ((calibration[0])<<4) | ((calibration[1]>>4)&0x0F);
  tcoeff[1] = ((0x0F&calibration[1])<<8)|(calibration[2]);
  tcoeff[2] = (calibration[3]<<12)|(calibration[4]<<4)|(calibration[5]>>4);
  tcoeff[3] = ((0x0F&calibration[5])<<16)|(calibration[6]<<8)|(calibration[7]);
  tcoeff[4] = (calibration[8]<<8)|(calibration[9]);
  tcoeff[5] = (calibration[10]<<8)|(calibration[11]);
  tcoeff[6] = (calibration[12]<<8)|(calibration[13]);
  tcoeff[7] = (calibration[14]<<8)|(calibration[15]);
  tcoeff[8] = (calibration[16]<<8)|(calibration[17]);

  int size[9] = {12,12,20,20,16,16,16,16,16};
  for(uint8_t i = 0; i < 9; i++){
    coeff[i] = toSignedInt(tcoeff[i],size[i]);
  }
}



