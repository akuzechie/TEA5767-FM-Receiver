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
        TEA5767_FM_Receiver();
        void initializeI2C();
        void standby();
        void setFrequency(float fRF);
        float getFrequency();
        byte getSignalLevel();
};
//----------------------------------------------------------------------
#endif