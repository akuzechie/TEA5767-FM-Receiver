# TEA5767 FM Receiver
List of public functions:
1. initializeI2C(): sets serial clock frequency (400 kHz) of ATmega328
2. standby(): puts receiver in standby mode (audio muted)
3. setFrequency(): plays FM station @ given frequency
4. getFrequency(): returns frequency of FM station
5. getSignalLevel(): returns signal level (percentage) of FM station
