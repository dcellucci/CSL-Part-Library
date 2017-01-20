#include <dps310.h>

//default address is 0x77. If you're having trouble talking to the sensor
//it might be because its actually 0x76.
DPS310 sensor(0x76);
int *c;
void setup() {
  // put your setup code here, to run once:
  sensor.init();
  delay(100);
  sensor.enableDefaultSettings();
  c = sensor.getCalibration();
  SerialUSB.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 // SerialUSB.println("====================");
  //sensor.enableDefaultSettings();
  //SerialUSB.print(sensor.readCalibratedTemp());
  //SerialUSB.println(',');
  SerialUSB.println(sensor.readCalibratedPressure());
  /*
  SerialUSB.println('\t');
  for(int i = 0; i < 9; i++){
    SerialUSB.print(c[i]);
    SerialUSB.print("\t");  
  }
  */
  //SerialUSB.println("\n====================");
  delay(200);
  /*

  delay(200);
  */
}
