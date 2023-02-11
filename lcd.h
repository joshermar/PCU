#ifndef LCD_H
#define LCD_H

#include "Arduino.h"
#include "DFRobot_LCD.h"

class LCD
{
public:
    void start(void);
    void home(void) {robotLCD.home();}
    void clear(void) {robotLCD.clear();}
    void setCursor(uint8_t col, uint8_t row) {robotLCD.setCursor(col, row);}
    void print(char* s) {robotLCD.print(s);}
    void print(char c) {robotLCD.print(c);}
    void setRGB(uint8_t r, uint8_t g, uint8_t b);
    RGBValue getRGB(void) {return rgb;}
    void saveRGB(void);
    void loadRGB(void);
    void setAnimation(char* baseStr, char* animStr, uint16_t animInterval);
    void animate(void);
private:
    RGBValue rgb;
    DFRobot_LCD robotLCD = DFRobot_LCD(16, 2);
    Timer timer;
    uint8_t animStartPos;
    uint8_t animIdx;
    char* baseStr;
    char* animStr;
};

#endif /*LCD_H*/
