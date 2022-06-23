//----------------------------------------------------
// TEA5767_FM_Receiver.cpp - Source File
// Library for Programming TEA5767 FM Receiver Module
// by: Anas Kuzechie (June, 2022)
//----------------------------------------------------
#include <Arduino.h>
#include <TEA5767_FM_Receiver.h>
//========================================================================
// Function Prototypes
//========================================================================
extern "C"
{
  void I2C_init();                      /*initialize ATmega328 I2C*/
  void I2C_START();                     /*send START condition*/
  void I2C_write(byte);                 /*write byte to TEA5767*/
  void I2C_read();                      /*read byte from TEA5767*/
  void I2C_STOP();                      /*send STOP condition*/
}
//========================================================================
// Constructor
//========================================================================
TEA5767_FM_Receiver::TEA5767_FM_Receiver() {}
//========================================================================
// Public Methods
//========================================================================
void TEA5767_FM_Receiver::initializeI2C()
{
  I2C_init();
}
//========================================================================
void TEA5767_FM_Receiver::standby()
{
  I2C_START();                          /*transmit START condition*/
  I2C_write(0xC0);                      /*send I2C write address*/
  I2C_write(0x80);                      /*mute ON, PLL high byte = 0*/
  I2C_write(0);                         /*PLL low byte = 0*/
  I2C_write(0xD8);                      /*high side injection, mono*/
  I2C_write(0x59);                      /*standby ON*/
  I2C_write(0x00);                      /*de-emphasis time const=50us*/
  I2C_STOP();                           /*transmit STOP condition*/ 
}
//========================================================================
void TEA5767_FM_Receiver::setFrequency(float fRF)
{
  unsigned int PLL = 122.07*(fRF+0.225);/*compute PLL*/
  byte PLLlow  = PLL & 0x00FF;          /*extract PLL low byte*/
  byte PLLhigh = (PLL & 0xFF00) >> 8;   /*extract PLL high byte*/
  //-------------------------------------------------------------------
  I2C_START();                          /*transmit START condition*/
  I2C_write(0xC0);                      /*send I2C write address*/
  I2C_write(PLLhigh);                   /*send high byte of PLL*/
  I2C_write(PLLlow);                    /*send low byte of PLL*/
  I2C_write(0xD8);                      /**/
  I2C_write(0x19);                      /*standby OFF*/
  I2C_write(0x00);                      /**/
  I2C_STOP();                           /*transmit STOP condition*/
}
//========================================================================
float TEA5767_FM_Receiver::getFrequency()
{
  byte firstByte, secondByte;
  I2C_START();                          /*transmit START condition*/
  I2C_write(0xC1);                      /*send I2C read address*/
  I2C_read();
  firstByte = TWDR;
  I2C_read();
  secondByte = TWDR;
  //----------------------------------------------------------------------
  firstByte = firstByte & 0x3F;
  double f = firstByte << 8 | secondByte;
  f = (f * 8192 - 225000) / 1000000.0;  /*compute FM station frequency*/
  return f;
}
//========================================================================
byte TEA5767_FM_Receiver::getSignalLevel()
{
  byte fourthByte;
  I2C_START();                          /*transmit START condition*/
  I2C_write(0xC1);                      /*send I2C read address*/
  I2C_read();
  I2C_read();
  I2C_read();
  I2C_read();
  fourthByte = TWDR;
  //----------------------------------------------------------------------
  byte SL = (fourthByte & 0xF0) >> 4;   /*extract signal level (0 to 15)*/
  SL = (float(SL)/15.0) * 100;          /*and convert to percentage*/
  return SL;
}