//--------------------------------------------------
// Search, Store & Play FM Stations
// using TEA5767_FM_Receiver.h & EEPROM.h Libraries
// by: Anas Kuzechie (June, 2022)
//--------------------------------------------------
#include <TEA5767_FM_Receiver.h>
#include <LCD_1602.h>
#include <EEPROM.h>
//--------------------------------------------------------------------------
TEA5767_FM_Receiver radio(2, 9);    /*Play & Search button connections*/
/* LCD lines D4 D5 D6 D7 EN RS*/
LCD_1602 lcd(3, 4, 5, 6, 7, 8);     /*LCD lines connected to digital pins*/
//--------------------------------------------------------------------------
byte i=0, j=0;                      /*index for EEPROM memory location*/
unsigned long millisCapture; int keyPressPeriod;
//==========================================================================
void setup()
{
  pinMode(11, OUTPUT);              /*stereo LED*/
  lcd.InitLCD();                    /*initialize 1602 LCD*/
  radio.I2CinitTEA5767();           /*initialize I2C for TEA5767*/
  radio.standby();                  /*TEA5767 in standby mode*/
  dispLCD_title();                  /*show title text on LCD*/
}
//==========================================================================
void loop()
{
  if(digitalRead(9))                /*when search button is pressed*/
  {
    delay(250);                     /*wait to debounce button press*/
    radio.searchStation(10);        /*search stations at min signal level*/
    radio.standby();                /*TEA5767 in standby mode*/
    dispLCD_title();                /*show title text on LCD*/
    i=0; j=0;                       /*point to EEPROM memory location 0*/
  }
  //------------------------------------------------------------------------
  if(digitalRead(2))                /*when play station button is pressed*/
  {
    byte memUsed = EEPROM.read(255);/*get memory used from station search*/
    millisCapture = millis();       /*measure button press period*/
    while(digitalRead(2)) {};
    keyPressPeriod = millis()-millisCapture;
    //----------------------------------------------------------------------
    if(keyPressPeriod > 500)        /*if long press of button > 0.5s*/
    {
      radio.standby();              /*TEA5767 in standby mode*/
      dispLCD_title();              /*show title text on LCD*/
      i=0; j=0;                     /*reset counters*/
      digitalWrite(11, LOW);        /*stereo LED OFF*/
    }
    else                            /*if short press of button < 0.5s*/
    {
      delay(250);                   /*wait to debounce button press*/
      digitalWrite(11, LOW);        /*stereo LED OFF*/
      float stnFreq=EEPROM.read(i); /*get integer part of stored freq*/
      stnFreq = stnFreq+EEPROM.read(i+1)/10.0;/*& add it to fraction part*/
      radio.setFreq(stnFreq);       /*write freq to TEA5767 to play stn*/
      digitalWrite(11, radio.stereoBit()); /*stereo LED ON or OFF*/
      delay(50);
      dispLCD_StnFreq();            /*show freq & signal level on LCD*/
      i+=2;                         /*update counter*/
      if(i==memUsed){i=0; j=0;}     /*point to location 0 when all stns played*/
    }    
  }
}
//==========================================================================
void dispLCD_title()
{
  lcd.writeByte(0, 0x01);
  delay(500);
  lcd.writeText(" TEA5767 Module ");
  lcd.writeByte(0, 0xC0);
  lcd.writeText("--FM  Receiver--");
}
//==========================================================================
void dispLCD_searching()
{
  lcd.writeByte(0, 0x01);
  delay(500);
  lcd.writeText("---searching!---");
  lcd.writeByte(0, 0xC0);
  lcd.writeText("--FM stations!--"); 
}
//==========================================================================
void dispLCD_StnFreq()
{
  byte f_MSD = int(radio.stationFreq())/100;
  byte f_MID = (int(radio.stationFreq())/10)%10;
  byte f_LSD;
  if(radio.stationFreq()<100) f_LSD = int(radio.stationFreq())%10;
  else f_LSD = int(radio.stationFreq())%100;
  byte f_fraction = ((radio.stationFreq()-int(radio.stationFreq()))*10)+1;
  j++;
  //------------------------------------------------------------------------
  lcd.writeByte(0, 0x01); delay(500);
  lcd.writeByte(0, 0x80);
  lcd.writeText("FM St");
  lcd.writeByte(1, j+48);
  lcd.writeText(": ");
  lcd.writeByte(1, f_MSD+48);
  lcd.writeByte(1, f_MID+48);
  lcd.writeByte(1, f_LSD+48);
  lcd.writeByte(1, '.');
  lcd.writeByte(1, f_fraction+48);
  lcd.writeText("MHz");
  //------------------------------------------------------------------------
  byte SL_MSD = radio.signalLevel()/10;
  byte SL_LSD = radio.signalLevel()%10;
  //------------------------------------------------------------------------
  lcd.writeByte(0, 0xC0);
  byte bar = ceil((radio.signalLevel()*12)/100);
  for(byte j=1; j<=bar; j++) lcd.writeByte(1, 255);
  if(radio.signalLevel() == 100) lcd.writeText("100");
  else
  {
    lcd.writeByte(1, SL_MSD+48);
    lcd.writeByte(1, SL_LSD+48);
  }
  lcd.writeByte(1, '%');
}