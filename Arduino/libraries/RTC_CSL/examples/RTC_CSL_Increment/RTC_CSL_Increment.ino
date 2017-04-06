#include <RTC_CSL.h>
#define Serial SerialUSB

RTC_CSL rtc;


//Toggle Servo Power Supply
int servoPowerPin = 16;
bool servoPower = true;
unsigned long cur, prev;

unsigned long curtime, rtctime, sysupdatetime;
void setup() {
  Serial.begin(115200);                           // Set up the serial port for test purposes
  rtc.begin();

  cur = 0;
  prev = 0;
  pinMode(servoPowerPin,OUTPUT);
}
 
void loop() {
  curtime = micros();
  if(curtime - rtctime > 10000){
    
  }
  cur = rtc.getCount();
  Serial.print("Current Count: ");  
  Serial.println(cur - prev);
  prev = cur; 
  digitalWrite(servoPowerPin, HIGH); 
  delay(500);
  digitalWrite(servoPowerPin, LOW);     
  delay(500);                                    // Wait 1 second
}


