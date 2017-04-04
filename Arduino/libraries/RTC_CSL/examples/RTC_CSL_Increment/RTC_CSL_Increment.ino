#include <RTC_CSL.h>
#define Serial SerialUSB

RTC_CSL rtc;


//Toggle Servo Power Supply
int servoPowerPin = 16;
bool servoPower = false;

void setup() {
  Serial.begin(115200);                           // Set up the serial port for test purposes
  rtc.begin();

  
  pinMode(servoPowerPin,OUTPUT);
}

void loop() {
  Serial.print("Current Count: ");    
  Serial.println(rtc.getCount());   
  digitalWrite(servoPowerPin, HIGH); 
  delay(500);
  digitalWrite(servoPowerPin, LOW);     
  delay(500);                                    // Wait 1 second
}


