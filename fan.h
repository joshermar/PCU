#ifndef FAN_H
#define FAN_H

#include "Arduino.h"
#include "FastLED.h"
// #include "pcu.h"

#define FAN_LEDPIN  A3
#define FAN_NUMLEDS 12


class Fan
{
public:
    Fan();
    RGBValue getRGB(void) { return {leds[0].r, leds[0].g, leds[0].b}; }
    void setRGB(uint8_t r, uint8_t g, uint8_t b);
    void saveRGB(void);
    void loadRGB(void);
    void animateSleep(void);
private:
    uint8_t animFrame;
    CRGB leds[FAN_NUMLEDS];
    Timer animTimer = Timer(120);
};

#endif /* FAN_H */
