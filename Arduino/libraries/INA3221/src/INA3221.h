/*----------------------------------------------------------
  @DESCRIPTION : INA3221 driver

  @DATASHEET   : http://www.ti.com/lit/ds/sbos576b/sbos576b.pdf
----------------------------------------------------------*/


// INA3221 ADDRESS PINS AND SLAVE ADDRESSES
// |----------|---------------|
// |    A0    | SLAVE ADDRESS |
// |----------|---------------|
// |    GND   |    100 0000   |
// |    VS    |    100 0001   |
// |    SDA   |    100 0010   |
// |    SCL   |    100 0011   |
///|----------|---------------|

#pragma once

#include <Wire.h>

#ifndef INA3221_H
#define INA3221_H

#define INA3221_ADDR 0x40

/*----------------------------------------------------------
  @Functions
----------------------------------------------------------*/

#ifndef _INA3221_H_
#define _INA3221_H_

class INA3221
{
public:
<<<<<<< HEAD
	INA3221(uint8_t address = INA3221_ADDR);

	void init();
	void enableDefaultSettings();
	void reset();

	uint16_t read_config();

	uint16_t read_bus_voltage(int channel);
	uint16_t read_shunt_voltage(int channel);
	uint16_t read_critical_voltage(int channel);
	uint16_t read_warning_voltage(int channel);

	uint16_t read_manID();
	uint16_t read_dieID();

protected:
	// private methods
	uint16_t ReadReg(uint8_t reg);
	void     WriteReg(uint8_t reg, uint16_t val);

	// private data
	uint8_t  _address;
};
#endif /* INA3221_H */
