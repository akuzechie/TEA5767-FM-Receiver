//---------------------------------------------
// Demo Sketch: Play Selected Local FM Station
// using TEA5767_FM_Receiver.h Library
// by: Anas Kuzechie (June, 2022)
//----------------------------------------------
#include <TEA5767_FM_Receiver.h>
//------------------------------
TEA5767_FM_Receiver radio;
//==================================================================
void setup()
{
  pinMode(2, INPUT);
  //---------------------------------------------------------
  radio.initializeI2C();
  radio.setFrequency(101.3);
  //---------------------------------------------------------
  Serial.begin(9600);
  Serial.print("FM Station  : ");
  Serial.print(radio.getFrequency()); Serial.println(" MHz");
  Serial.print("Signal Level: ");
  Serial.print(radio.getSignalLevel()); Serial.println(" %");
}
//==================================================================
void loop()
{
  while(!digitalRead(2));
  delay(200);
  radio.standby();
  while(!digitalRead(2));
  delay(200);
  radio.setFrequency(101.3);
}