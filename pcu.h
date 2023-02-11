#ifndef COMMAND_H
#define COMMAND_H

#include "Arduino.h"

#define ARRLEN(a) (sizeof a / sizeof a[0])

class Timer {
public:
    Timer() {timeoutMillis=0;}
    Timer(uint16_t millis) : timeoutMillis(millis) {}
    
    void set(uint16_t millis) {
        timeoutMillis=millis;
        reset();
    }
    
    void reset() {
        if (isSet()) startMillis = millis();
    }
    
    bool isReady() {
        return isSet() && (millis()-startMillis >= timeoutMillis);
    }

private:
    bool isSet() {return (bool)timeoutMillis;}
    uint16_t timeoutMillis;
    uint32_t startMillis;
};

struct RGBValue {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

enum {
    EEPROM_LCD_R,
    EEPROM_LCD_G,
    EEPROM_LCD_B,
    EEPROM_FAN_R, 
    EEPROM_FAN_G,
    EEPROM_FAN_B,
};

enum Response {
    RES_OK = 0,
    RES_READY,
    RES_ERROR,
};

enum PCUMode {
    MODE_IDLE,
    MODE_NORMAL,
    MODE_SLEEP,
};

void modeSwitch(PCUMode);

#endif  /* COMMAND_H */