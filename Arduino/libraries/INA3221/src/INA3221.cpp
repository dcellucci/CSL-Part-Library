/*----------------------------------------------------------
  @DESCRIPTION : INA3221 driver
  
  @DATASHEET   : http://www.ti.com/lit/ds/sbos576b/sbos576b.pdf
----------------------------------------------------------*/

#include <Arduino.h>
#include <Wire.h>

#include "INA3221.h"

//Register Map
#define      CONFIG 0x00
#define   SHUNT_V_1 0x01
#define     BUS_V_1 0x02
#define   SHUNT_V_2 0x03
#define     BUS_V_2 0x04
#define   SHUNT_V_3 0x05
#define     BUS_V_3 0x06
#define  CRITICAL_1 0x07
#define   WARNING_1 0x08
#define  CRITICAL_2 0x09
#define   WARNING_2 0x0A
#define  CRITICAL_3 0x0B
#define   WARNING_3 0x0C
#define      MAN_ID 0xFE
#define      DIE_ID 0xFF

//Enable Channels
#define    CH1_EN_POS (14)
#define    CH2_EN_POS (13)
#define    CH3_EN_POS (12)

//Average Mode
#define 	 AVG_0001 0x0
#define 	 AVG_0004 0x1
#define 	 AVG_0016 0x2
#define 	 AVG_0064 0x3
#define 	 AVG_0128 0x4
#define 	 AVG_0256 0x5
#define 	 AVG_0512 0x6
#define 	 AVG_1024 0x7
#define  AVG_MODE_POS (9)

//Voltage Conversion Time
#define      VCT_0140 0x0
#define      VCT_0204 0x1
#define      VCT_0332 0x2
#define      VCT_0588 0x3
#define      VCT_1100 0x4
#define      VCT_2116 0x5
#define      VCT_4156 0x6
#define      VCT_8244 0x7

#define   BUS_VCT_POS (6)
#define  SHNT_VCT_POS (3)

//Operating Modes

#define  POWER_DOWN_MODE 0x0
#define  VSHNT_SING_SHOT 0x1
#define   VBUS_SING_SHOT 0x2
#define  BOTHV_SING_SHOT 0x3
#define  POWER_DOWN_MODE 0x4
#define       VSHNT_CONT 0x5
#define        VBUS_CONT 0x6
#define       BOTHV_CONT 0x7


INA3221::INA3221(uint8_t address){
	_address = address;
}

void INA3221::init(){
	//Wire.begin();
	reset();
  delay(50);
  enableDefaultSettings();
}

void INA3221::reset(){
	WriteReg(CONFIG,(0x1)<<15);
}

void INA3221::enableDefaultSettings(){
	uint16_t toWrite  = (0x7<<CH3_EN_POS);
	         toWrite |= (AVG_0001 << AVG_MODE_POS);
	         toWrite |= (VCT_1100 << BUS_VCT_POS);
	         toWrite |= (VCT_2116 << SHNT_VCT_POS);
	         toWrite |= BOTHV_CONT;
	WriteReg(CONFIG,toWrite);

}

//LSB is 0.008 V and the value is bitshifted 3 positions
//so returned value is in mV
uint16_t INA3221::read_bus_voltage(int channel){
  return ReadReg(0x02+2*(channel-1));
}

//LSB is 40 uV
uint16_t INA3221::read_shunt_voltage(int channel){
  return ReadReg(0x01+2*(channel-1));
}

uint16_t INA3221::read_manID(){
	return ReadReg(MAN_ID);
}

uint16_t INA3221::read_dieID(){
  return ReadReg(DIE_ID);
}

void INA3221::WriteReg(uint8_t reg, uint16_t val)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write((val>>8)&0xFF);
  Wire.write(val&0xFF);
  Wire.endTransmission();
}

uint16_t INA3221::ReadReg(uint8_t reg)
{
  // Wire is awkward because it doesn't really have a register address concept.
  // http://www.arduino.cc/en/Tutorial/SFRRangerReader for reference

  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission(false);// false keeps connection active so we can read.

  uint8_t status = Wire.requestFrom(_address, (uint8_t)2);
  if(status)
  {
    return((Wire.read() << 8) | Wire.read());
  }
  else
  {
#ifdef DEBUG_SERIAL
    SerialUSB.print("readReg failed? status:");
    SerialUSB.println(status, HEX);
#endif
  }
  return 0xff;
}