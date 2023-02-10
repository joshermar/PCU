#ifndef FAN_H
#define FAN_H

#include "FastLED.h"

#define FAN_DATAPIN A3
#define FAN_NUMLEDS 12

enum animation {
    NONE,
    SLEEP,
};


class Fan
{
public:
    Fan();
    void setRGB(uint8_t r, uint8_t g, uint8_t b);
    void saveRGB(void);
    void loadRGB(void);
    void setAnimation(animation a);
    void animate(void);
private:
    uint8_t animFrame;
    animation animSetting;
    CRGB leds[FAN_NUMLEDS];
    Timer animTimer = Timer(100);
    void animSleep(void);
};


// class FanAnimation
// {
// public:
//     FanAnimation(const CRGB* fanLeds, const CRGB color, uint16_t interval);
//     ~Animation();
//     animate();

// private:

//     bool checkTimer();
//     uint8_t fanIdx;
//     uint32_t lastAnimation;
//     CRGB* fanLeds;
//     CRGB color;
//     uint16_t interval;
// };











#endif /* FAN_H */
