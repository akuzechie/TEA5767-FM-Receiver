//------------------------------------------------------
// TEA5767 Plays FM Stations & Station Data on 1602 LCD
// using TEA5767_FM_Receiver.h & LCD_1602.h Libraries
// by: Anas Kuzechie (June, 2022)
//------------------------------------------------------
#include <TEA5767_FM_Receiver.h>
#include <LCD_1602.h>
//------------------------------
TEA5767_FM_Receiver radio;
/* LCD lines D4 D5 D6 D7 EN RS*/
LCD_1602 lcd(3, 4, 5, 6, 7, 8);
//------------------------------
#define BTN  2
float freq_RF[3] = {101.3,104.5,107.9}; /*RF of local FM stations*/
unsigned long millisCapture; int keyPressPeriod; byte i;
//=====================================================================
void setup()
{
  pinMode(BTN, INPUT);                /*initialize for i/p (button)*/
  radio.initializeI2C();              /*initialize I2C of ATmega328*/
  lcd.InitLCD();                      /*initialize 16x2 LCD*/
  radio.standby();                    /*put TEA5767 in standby mode*/
  dispTitle();                        /*display title message*/
}
//=====================================================================
void loop()
{
  if(digitalRead(BTN))                /*when button is pressed*/
  {
    millisCapture = millis();         /*measure button press period*/
    while(digitalRead(BTN)) {};       /*while button is pressed*/
    keyPressPeriod = millis() - millisCapture;
    //-----------------------------------------------------------------
    if(keyPressPeriod > 500)
    {
      delay(200);
      radio.standby();                /*standby if button long press*/
      dispTitle();
      i = 0;
    }
    else
    {
      delay(200);
      radio.setFrequency(freq_RF[i]); /*play FM station if short press*/
      dispData(radio.getFrequency(), radio.getSignalLevel());
      i++;
      if(i==3) i=0;                   /*reset if last station*/
    }
  }
}
//=====================================================================
void dispTitle()
{
  lcd.writeByte(0, 0x01);             /*clear LCD*/
  delay(1000);
  //-------------------------------------------------------------------
  lcd.writeText(" TEA5767 Module ");
  lcd.writeByte(0, 0xC0);             /*beginning of 2nd line*/
  lcd.writeText("--FM  Receiver--");
}
//=====================================================================
void dispData(double fRF, byte sl)
{
  lcd.writeByte(0, 0x01);             /*clear LCD*/
  delay(1000);
  //-------------------------------------------------------------------
  byte f_MSD = int(fRF)/100;
  byte f_MID = (int(fRF)/10) % 10;
  byte f_LSD;
  if(fRF < 100) f_LSD = int(fRF) % 10;
  else f_LSD = int(fRF) % 100;
  byte f_fraction = (fRF - int(fRF)) * 10;
  //-------------------------------------------------------------------
  lcd.writeByte(0, 0x80);             /*display FM station frequency*/
  lcd.writeText("FM St");
  lcd.writeByte(1, i+1+48);
  lcd.writeText(":");
  lcd.writeByte(1, f_MSD + 48);
  lcd.writeByte(1, f_MID + 48);
  lcd.writeByte(1, f_LSD + 48);
  lcd.writeByte(1, '.');
  lcd.writeByte(1, f_fraction + 48);
  lcd.writeText(" MHz");
  //-------------------------------------------------------------------
  byte SL_MSD = sl/10;
  byte SL_LSD = sl%10;
  //-------------------------------------------------------------------
  lcd.writeByte(0, 0xC0);             /*display signal level on line2*/
  byte bar = ceil((sl*12)/100);
  for(byte j=1; j<=bar; j++) lcd.writeByte(1, 255);
  if(sl == 100) lcd.writeText("100");
  else
  {
    lcd.writeByte(1, SL_MSD + 48);
    lcd.writeByte(1, SL_LSD + 48);
  }
  lcd.writeByte(1, '%');
}
