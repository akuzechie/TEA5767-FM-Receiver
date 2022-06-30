//----------------------------------------------------
// TEA5767_FM_Receiver.h - Header File
// Library for Programming TEA5767 FM Receiver Module
// by: Anas Kuzechie (June, 2022)
//----------------------------------------------------
//header guards
#ifndef TEA5767_FM_Receiver_h
#define TEA5767_FM_Receiver_h
//-----------------------------------------------------------------------
//class definition
class TEA5767_FM_Receiver
{
    public:
        TEA5767_FM_Receiver(byte btnPlay, byte btnSearch);
        void I2CinitTEA5767();
        void standby();
        void searchStation(byte minSL);
        void setFreq(float fRF);
        float stationFreq();
        byte signalLevel();
        byte stereoBit();
    private:
        byte _btnPlay, _btnSearch, _minSL, readByte[5];
        void writeTEA5767(byte byte1, byte byte2, byte byte3, byte byte4);
        void readTEA5767();
};
//----------------------------------------------------------------------
#endif
