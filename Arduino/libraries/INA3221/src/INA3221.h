/*----------------------------------------------------------
  @DESCRIPTION : INA3221 driver
  
  @DATASHEET   : http://www.ti.com/lit/ds/sbos576b/sbos576b.pdf
----------------------------------------------------------*/

// ---------------------------------------------------------
// INA3221 ADDRESS PINS AND SLAVE ADDRESSES
// |----------|---------------|
// |    A0    | SLAVE ADDRESS |
// |----------|---------------|
// |    GND   |    100 0000   |
// |    VS    |    100 0001   |
// |    SDA   |    100 0010   |
// |    SCL   |    100 0011   |
// |----------|---------------|
// ---------------------------------------------------------

#ifndef _INA3221_H_
#define _INA3221_H_

class INA3221
{
public:
    INA3221(uint8_t address = 0x40);
    
    void init();
    
    uint16_t readConfig();

    uint16_t readCriticalVoltage1();
    uint16_t readCriticalVoltage2();
    uint16_t readCriticalVoltage3();

    uint16_t readWarningVoltage1();
    uint16_t readWarningVoltage2();
    uint16_t readWarningVoltage3();

    uint16_t readBusVoltage1();
    uint16_t readBusVoltage2();
    uint16_t readBusVoltage3();

    uint16_t readShuntVoltage1();
    uint16_t readShuntVoltage2();
    uint16_t readShuntVoltage3();

protected:
    uint8_t readReg(uint8_t reg);
    void writeReg(uint_t reg, uint8_t val);
    
    int currentConv = 5;
    int voltageConv = 125;
}

#endif /* _INA3221_H_ */