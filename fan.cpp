#include "Arduino.h"
#include "EEPROM.h"
#include "FastLED.h"
#include "pcu.h"
#include "fan.h"


Fan::Fan() {
    FastLED.addLeds<WS2812B, FAN_LEDPIN, GRB>(leds, FAN_NUMLEDS);
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


void Fan::animateSleep(void) {
    if(!animTimer.isReady()) return;

    animTimer.reset();

    static CRGB color = CRGB(0,0,3);
    static uint8_t led_idx;
    static uint8_t increment = 1;
    static uint8_t dark_timeout = 0;

    if (0 == color.b) {
        if (dark_timeout++ < 20) return;
        dark_timeout = 0;
        increment = 1;
    }

    if (3 == color.b) increment = -1;

    color.b += increment;

    leds[led_idx] = CRGB(0,0,0);
    led_idx = (led_idx + 1) % FAN_NUMLEDS;
    leds[led_idx] = color;

    FastLED.show();
}
