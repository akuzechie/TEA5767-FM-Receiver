//--------------------------------------------------
// Search, Store & Play FM Stations
// using TEA5767_FM_Receiver.h & EEPROM.h Libraries
// by: Anas Kuzechie (June, 2022)
//--------------------------------------------------
#include <TEA5767_FM_Receiver.h>
#include <EEPROM.h>
//--------------------------------------------------------------------------
TEA5767_FM_Receiver radio(2, 9);    /*Play & Search button connections*/
//--------------------------------------------------------------------------
byte i=0, j=0;                      /*index for EEPROM memory location*/
unsigned long millisCapture; int keyPressPeriod;
//==========================================================================
void setup()
{
  Serial.begin(9600);
  pinMode(11, OUTPUT);              /*stereo LED*/
  radio.I2CinitTEA5767();           /*initialize I2C for TEA5767*/
  radio.standby();                  /*TEA5767 in standby mode*/
}
//==========================================================================
void loop()
{
  if(digitalRead(9))                /*when search button is pressed*/
  {
    delay(250);                     /*wait to debounce button press*/
    radio.searchStation(10);        /*search stations at min signal level*/
    radio.standby();                /*TEA5767 in standby mode*/
    i=0; j=0;                       /*point to EEPROM memory location 0*/
  }
  //------------------------------------------------------------------------
  if(digitalRead(2))                /*when play button is pressed*/
  {
    byte memUsed = EEPROM.read(255);/*get memory used from station search*/
    millisCapture = millis();       /*measure button press period*/
    while(digitalRead(2)) {};
    keyPressPeriod = millis()-millisCapture;
    //----------------------------------------------------------------------
    if(keyPressPeriod > 500)        /*execute when button press > 0.5s*/
    {
      radio.standby();              /*TEA5767 in standby mode*/
      i=0; j=0;                     /*point to EEPROM memory location 0*/
      digitalWrite(11, LOW);        /*stereo LED OFF*/
    }
    else                            /*execute if button press < 0.5s*/
    {
      delay(250);                   /*wait to debounce button press*/
      digitalWrite(11, LOW);
      float stnFreq=EEPROM.read(i); /*get integer part of stored freq*/
      stnFreq = stnFreq+EEPROM.read(i+1)/10.0;/*& add it to fraction part*/
      radio.setFreq(stnFreq);       /*write freq to TEA5767 to play stn*/
      digitalWrite(11, radio.stereoBit()); /*stereo bit ON*/
      j++;
      //--------------------------------------------------------------------
      Serial.print("FM Station "); Serial.print(j); Serial.print(": ");
      Serial.print(radio.stationFreq()); Serial.print(" MHz @ ");
      Serial.print(radio.signalLevel()); Serial.println("%");
      //--------------------------------------------------------------------
      delay(50);
      i+=2;                         /*point to next frequency in EEPROM*/
      if(i==memUsed){i=0; j=0;}     /*point to location 0 when all stns played*/
    }    
  }
}