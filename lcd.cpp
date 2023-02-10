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
    this->r = r;
    this->g = g;
    this->b = b;
    robotLCD.setRGB(r, g, b);
}

void LCD::saveRGB() {
    EEPROM.put(EEPROM_LCD_R, r);
    EEPROM.put(EEPROM_LCD_G, g);
    EEPROM.put(EEPROM_LCD_B, b);
}

void LCD::loadRGB() {
    r = EEPROM.read(EEPROM_LCD_R);
    g = EEPROM.read(EEPROM_LCD_G);
    b = EEPROM.read(EEPROM_LCD_B);
    setRGB(r, g, b);
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
