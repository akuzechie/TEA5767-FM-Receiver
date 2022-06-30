//----------------------------------------------------
// TEA5767_FM_Receiver.cpp - Source File
// Library for Programming TEA5767 FM Receiver Module
// by: Anas Kuzechie (June, 2022)
//----------------------------------------------------
#include <Arduino.h>
#include <TEA5767_FM_Receiver.h>
#include <EEPROM.h>
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
TEA5767_FM_Receiver::TEA5767_FM_Receiver(byte btnPlay, byte btnSearch)
{
  pinMode(btnPlay, INPUT); _btnPlay = btnPlay;
  pinMode(btnSearch, INPUT); _btnSearch = btnSearch;
}
//========================================================================
// Public Methods
//========================================================================
void TEA5767_FM_Receiver::I2CinitTEA5767()
{
  I2C_init();
}
//========================================================================
void TEA5767_FM_Receiver::standby()
{
  writeTEA5767(0x80, 0, 0, 0x50);       /*mute ON, standby ON*/
}
//========================================================================
void TEA5767_FM_Receiver::searchStation(byte minSL)
{
  byte i=0;
  _minSL = minSL;
  for(float f=88.0; f<=108.0; f+=0.1)   /*scan FM band 88-->108 (MHz)*/
  {
    unsigned int PLL = 122.07*(f+0.225);/*compute PLL*/
    byte PLLlow  = PLL & 0x00FF;        /*extract PLL low byte*/
    byte PLLhigh = (PLL & 0xFF00) >> 8; /*extract PLL high byte*/
    //----------------------------------------------------------------------
    writeTEA5767(PLLhigh, PLLlow, 0xD0, 0x1E);/*write to TEA5767*/
    delay(50);
    readTEA5767();                      /*read stereo bit & signal level*/
    //----------------------------------------------------------------------
    boolean stereo = (readByte[2] & 0x80);/*extract stereo bit*/
    byte SL = (readByte[3] & 0xF0) >> 4;/*extract signal level (0 to 15)*/
    //----------------------------------------------------------------------
    if(SL>=_minSL && stereo)            /*when FM station found*/
    {
      EEPROM.write(i, int(f));          /*store in EEPROM integer part of freq*/
      EEPROM.write(i+1, ((f-int(f))*10)+1);/*then store fraction part*/
      i+=2;                             /*point to next frequency in memory*/
    }
  }
  EEPROM.write(255, i);                 /*store number of used locations*/
}
//========================================================================
void TEA5767_FM_Receiver::setFreq(float fRF)
{
  unsigned int PLL = 122.07*(fRF+0.225);/*compute PLL*/
  byte PLLlow  = PLL & 0x00FF;          /*extract PLL low byte*/
  byte PLLhigh = (PLL & 0xFF00) >> 8;   /*extract PLL high byte*/
  //------------------------------------------------------------------------
  writeTEA5767(PLLhigh, PLLlow, 0xD0, 0x1E); /*write to play FM station*/
}
//========================================================================
byte TEA5767_FM_Receiver::signalLevel()
{
  readTEA5767();
  byte SL = (readByte[3] & 0xF0) >> 4;  /*extract signal level (0 to 15)*/
  SL = (float(SL)/15.0) * 100;          /*and convert to percentage*/
  return SL;
}
//========================================================================
float TEA5767_FM_Receiver::stationFreq()
{
  readTEA5767();
  readByte[0] = readByte[0] & 0x3F;
  double f = readByte[0] << 8 | readByte[1];
  f = (f * 8192 - 225000) / 1000000.0;
  return f;                             /*return FM station frequency*/
}
//========================================================================
byte TEA5767_FM_Receiver::stereoBit()
{
  readTEA5767();
  return (readByte[2] & 0x80)>>7;       /*return stereo bit*/
}
//========================================================================
// Private Methods
//========================================================================
void TEA5767_FM_Receiver::writeTEA5767(byte byte1, byte byte2, byte byte3, byte byte4)
{
  I2C_START();                          /*transmit START condition*/
  I2C_write(0xC0);                      /*send I2C write address*/
  I2C_write(byte1);                     /*send byte 1*/
  I2C_write(byte2);                     /*send byte 2*/
  I2C_write(byte3);                     /*send byte 3*/
  I2C_write(byte4);                     /*send byte 4*/
  I2C_write(0);                         /*send byte 5*/
  I2C_STOP();                           /*transmit STOP condition*/
}
//========================================================================
void TEA5767_FM_Receiver::readTEA5767()
{
  I2C_START();                          /*transmit START condition*/
  I2C_write(0xC1);                      /*send I2C read address*/
  for(byte j=0; j<=3; j++)              /*read & save 4 bytes*/
  {
    I2C_read();
    readByte[j] = TWDR;
  }
}
