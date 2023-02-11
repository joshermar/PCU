#include "Arduino.h"
#include "EEPROM.h"
#include "DFRobot_LCD.h"
#include "pcu.h"
#include "lcd.h"


void LCD::start(void) {
    robotLCD.init();
    clear();
    loadRGB();
}

void LCD::setRGB(uint8_t r, uint8_t g, uint8_t b) {
    rgb.r = r;
    rgb.g = g;
    rgb.b = b;
    robotLCD.setRGB(rgb.r, rgb.g, rgb.b);
}

void LCD::saveRGB() {
    EEPROM.put(EEPROM_LCD_R, rgb.r);
    EEPROM.put(EEPROM_LCD_G, rgb.g);
    EEPROM.put(EEPROM_LCD_B, rgb.b);
}

void LCD::loadRGB() {
    rgb.r = EEPROM.read(EEPROM_LCD_R);
    rgb.g = EEPROM.read(EEPROM_LCD_G);
    rgb.b = EEPROM.read(EEPROM_LCD_B);
    robotLCD.setRGB(rgb.r, rgb.g, rgb.b);
}

void LCD::setAnimation(char* baseStr, char* animStr, uint16_t animInterval) {
    this->baseStr = baseStr;
    this->animStr = animStr;
    timer.set(animInterval);
    animStartPos = strlen(baseStr);
    clear();
    print(baseStr);
    setCursor(animStartPos, 0);
    animIdx = 0;
}

void LCD::animate() {
    if (!timer.isReady()) return;

    timer.reset();

    if (animIdx >= strlen(animStr)) {
        setCursor(animStartPos, 0);
        for (int i=0; i<strlen(animStr); i++) robotLCD.print(" ");
        setCursor(animStartPos, 0);
        animIdx = 0;
    } else {
        robotLCD.print(animStr[animIdx++]);
    }
}
