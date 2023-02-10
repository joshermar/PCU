#include "Arduino.h"
#include "EEPROM.h"
#include "FastLED.h"
#include "pcu.h"
#include "fan.h"


Fan::Fan() {
    FastLED.addLeds<WS2811, FAN_DATAPIN, GRB>(leds, FAN_NUMLEDS);
}

void Fan::setRGB(uint8_t r, uint8_t g, uint8_t b) {
    CRGB color(r, g, b);

    for (int i=0; i<FAN_NUMLEDS; i++) {
        leds[i] = color;
    }

    FastLED.show();
}

void Fan::saveRGB() {
    EEPROM.put(EEPROM_FAN_R, leds[0].r);
    EEPROM.put(EEPROM_FAN_G, leds[0].g);
    EEPROM.put(EEPROM_FAN_B, leds[0].b);
}

void Fan::loadRGB() {
    uint8_t r = EEPROM.read(EEPROM_FAN_R);
    uint8_t g = EEPROM.read(EEPROM_FAN_G);
    uint8_t b = EEPROM.read(EEPROM_FAN_B);
    setRGB(r, g, b);
}

void Fan::setAnimation(animation a) {
    animSetting = a;
    animFrame = 0;

    if (NONE == a) {
        loadRGB();
    } else {
        setRGB(0, 0, 0);
    }

}


void Fan::animate(void) {
    switch (animSetting) {
    case NONE:
        break;
    case SLEEP:
        animSleep();
        break;
    }
}


void Fan::animSleep(void) {
    if(!animTimer.isReady()) return;

    animTimer.reset();

    static CRGB color;
    static CRGB color;

    if(animFrame++ >= 16) {
        setRGB(0, 0, 0);
        color = CRGB(0,0,12);
        leds
    } else {

    }
}


// FanAnimation::FanAnimation(const CRGB* fanLeds, const CRGB color, uint16_t interval) {
//     this->fanLeds = fanLeds;
//     this->color = color;
//     this->interval = interval;

//     fanIdx = 0;
//     lastAnimation = 0;
// }

// FanAnimation::animate() {
//     if (!checkTimer()) return;

//     fanLeds[fanIdx] = CRGB(0, 0, 0);
//     fanIdx = (fanIdx+1)%FAN_NUMLEDS;
//     fanLeds[fanIdx] = color;

//     lastAnimation = millis();
// }

// FanAnimation::checkTimer() {
//     return (millis()-(lastAnimation) >= (uint32_t)(interval));
// }




const PROGMEM int animation_sleep[][3] {
    {0,0,12}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,11}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,10}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,9}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,8}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,7}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,6}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,5}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,4}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,3}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,2}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,1},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
};