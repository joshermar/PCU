#ifndef FAN_H
#define FAN_H

#include "FastLED.h"

#define FAN_DATAPIN A3
#define FAN_NUMLEDS 12


class Fan
{
public:
    Fan();
    void setRGB(uint8_t r, uint8_t g, uint8_t b);
    void saveRGB(void);
    void loadRGB(void);
    // void setAnimation(animation a);
    // void animate(void);
    void animateSleep(void);
private:
    uint8_t animFrame;
    // animation animSetting;
    CRGB leds[FAN_NUMLEDS];
    Timer animTimer = Timer(120);
};

#endif /* FAN_H */
