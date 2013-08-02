/**
 * Write any data structure or variable, using any number of bytes 
 * of EEPROM to do it (of course, only as many bytes as your chip holds), 
 * all in a single call.
 *
 * Source: http://playground.arduino.cc/Code/EEPROMWriteAnything
 */

#include <EEPROM.h>
#include <Arduino.h>

template <class T> int EEPROM_writeAnything(int ee, const T& value) {
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
        EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value) {
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
        *p++ = EEPROM.read(ee++);
    return i;
}
