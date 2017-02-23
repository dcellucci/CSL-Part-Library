/*----------------------------------------------------------
  @DESCRIPTION : INA3221 driver
  
  @DATASHEET   : http://www.ti.com/lit/ds/sbos576b/sbos576b.pdf
----------------------------------------------------------*/

#include <Wire.h>
#include <INA3221.h>


// DATA REGISTERS
static const uint8_t      CONFIG = 0x00;
static const uint8_t    SHUNT_V1 = 0x01;
static const uint8_t      BUS_V1 = 0x02;
static const uint8_t    SHUNT_V2 = 0x03;
static const uint8_t      BUS_V2 = 0x04;
static const uint8_t    SHUNT_V3 = 0x05;
static const uint8_t      BUS_V3 = 0x06;
static const uint8_t  CRITICAL_1 = 0x07;
static const uint8_t   WARNING_1 = 0x08;
static const uint8_t  CRITICAL_2 = 0x09;
static const uint8_t   WARNING_2 = 0x0A;
static const uint8_t  CRITICAL_3 = 0x0B;
static const uint8_t   WARNING_3 = 0x0C;
static const uint8_t MASK_ENABLE = 0x0F;

void setDefaultCurrentAlertLevels();
void setDefaultCurrentWarningLevels();

/*----------------------------------------------------------
  INA3221
----------------------------------------------------------*/

void INA3221::INA3221(uint16_t address)
{
    _address = address;
}

int INA3221::readConfig()
{
    return readReg(CONFIG);
}

int INA3221::readCriticalVoltage1(){
    return readReg(CRITICAL_1) >> 3;
}

int INA3221::readCriticalVoltage2(){
    return readReg(CRITICAL_2) >> 3;
}

int INA3221::readCriticalVoltage3(){
    return readReg(CRITICAL_3) >> 3;
}

int INA3221::readWarningVoltage1(){
    return readReg(WARNING_1) >> 3;
}

int INA3221::readWarningVoltage2(){
    return readReg(WARNING_2) >> 3;
}

int INA3221::readWarningVoltage3(){
    return readReg(WARNING_3) >> 3;
}

int INA3221::readBusVoltage1(){
    return readReg(BUS_V1) >> 3;
}

int INA3221::readBusVoltage2(){
    return readReg(BUS_V2) >> 3;
}

int INA3221::readBusVoltage3(){
    return readReg(BUS_V3) >> 3;
}

int INA3221::readShuntVoltage1(){
    return readReg(SHUNT_V1) >> 3;
}

int INA3221::readShuntVoltage2(){
    return readReg(SHUNT_V2) >> 3;
}

int INA3221::readShuntVoltage3(){
    return readReg(SHUNT_V3) >> 3;
}

void INA3221::setCriticalVoltage(uint8_t channel, uint16_t current){
    current = current*currentConv;
    
    if (channel==1){
        writeReg(CRITICAL_1_REG, current << 3);
    }
    else if (channel==2){
        writeReg(CRITICAL_2_REG, current << 3);
    }
    else if (channel==3){
        writeReg(CRITICAL_3_REG, current << 3);
    }
}

void INA3221::setWarningVoltage(uint8_t channel, uint16_t current){
    current = current*currentConv;
    
    if (channel==1){
        writeReg(WARNING_1, current << 3);
    }
    else if (channel==2){
        writeReg(WARNING_2, current << 3);
    }
    else if (channel==3){
        writeReg(WARNING_3, current << 3);
    }
}

void init()
{
	Wire.begin();
}


// Wire.h read and write protocols
uint8_t INA3221::readReg(uint8_t reg)
{	
	Wire.beginTransmission(_address);        // Initialize the Tx buffer
	Wire.write(reg);	                     // Put slave register address in Tx buffer
	Wire.endTransmission(false);             // keep connection alive for read process
	
    uint8_t status = Wire.requestFrom(_address, (uint8_t)1);  // Read one byte from slave register address returning status
	if (status)
    {
        return(Wire.read());
    }
    else{
        Serial.print("readReg failed?  status:");
        Serial.println(status, HEX);
    }
    
    return 0xff;
}

void INA3221::writeReg(uint8_t reg, uint8_t val)
{
	Wire.beginTransmission(_address);      // Initialize the Tx buffer
	Wire.write(reg);                       // Put slave register address in Tx buffer
	Wire.write(val);                      // Put data in Tx buffer
	Wire.endTransmission();                // Send the Tx buffer
}

