# TEA5767 FM Receiver (updated June 29, 2022)
List of public functions:
1. I2CinitTEA5767(): sets I2C serial clock frequency (400 kHz) of ATmega328
2. standby(): puts receiver in standby mode (audio muted)
3. searchStation(): searches FM stations & stores detected frequencies in ATmega328 EEPROM
4. setFreq(): plays FM station @ given frequency
5. stationFreq(): returns frequency (in MHz) of FM station
6. SignalLevel(): returns signal level (in percentage) of FM station
7. stereoBit(): returns 1 if signal stereo, otherwise 0 if mono

NOTE: Before running the included example sketch, see circuit diagram of the FM receiver.
