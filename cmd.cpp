#include "pcu.h"
#include "lcd.h"
#include "fan.h"


#define ISDELIMITER(c)  ('"' == c || '\'' == c)
#define ISWHITESPACE(c) (' ' == c || '\t' == c)

extern Timer sleepTimer;
extern LCD lcd;
extern Fan fan;


char* PCUMODES[] = {
    [IDLE] = "IDLE",
    [NORMAL] = "NORMAL",
    [SLEEP] = "SLEEP",
};


bool validint(char *s) {
    if (NULL == s || '\0' == *s) return false;

    while (*s) {
        if (!isdigit(*s++)) return false;
    }

    return true;
}

char *extract_strarg(char *arg_string)
{
    // Advance to first non whitespace character
    while(ISWHITESPACE(*arg_string)) arg_string++;

    // First non space character must be a valid delimeter
    char delim = *arg_string;
    
    if (!ISDELIMITER(delim)) return NULL;  // Missing/invalid starting delimeter

    // Find closing delimeter
    char *close_delim = strchr((arg_string+1), delim);
    if (NULL == close_delim) return NULL;  // Missing closing delimeter

    // Check for garbage after closing delimeter
    for (char *p = close_delim+1; *p; p++) {
        if ('\0' == *p) break;
        if(!ISWHITESPACE(*p)) return NULL;  // Extra tok after closing delim
    }

    // String arg is valid. Mark delimeters and return extracted string.
    *arg_string++ = '\0';
    *close_delim = '\0';

    return arg_string;
}


/* PCU Command functions*/


uint8_t cmd_PCU_READY(char *args) {
    
    // CMD params: none
    if(args) return RES_BADARGS;

    return RES_READY;
}


uint8_t cmd_PCU_MODE(char *args) {
    
    // CMD params: 1 str arg
    char *arg1 = strtok(args, " ");
    if (!arg1) return RES_BADARGS;
    if(strtok(NULL, " ")) return RES_BADARGS;  // Do not accept extra args!

    for (int i=0; i<ARRLEN(PCUMODES); i++) {
        if(0 == strcasecmp(PCUMODES[i], arg1)) {
            mode_switch(i);
            return RES_OK;
        }
    }
    return RES_BADARGS;
}


uint8_t cmd_SET_SLEEP(char *args) {
    
    // CMD params: 1 valid int
    char *arg1 = strtok(args, " ");
    if(!validint(arg1)) return RES_BADARGS;
    if(strtok(NULL, " ")) return RES_BADARGS;

    sleepTimer.set(atoi(arg1));
    return RES_OK;
}


uint8_t cmd_LCD_HOME(char *args) {
    
    // CMD params: none
    if(args) return RES_BADARGS;

    lcd.home();
    return RES_OK;
}


uint8_t cmd_LCD_CLEAR(char *args) {
    
    // CMD params: none
    if(args) return RES_BADARGS;

    lcd.clear();
    return RES_OK;
}


uint8_t cmd_LCD_SETRGB(char *args) {

    // CMD params: 3 valid ints
    char *arg1 = strtok(args, " ");
    if(!validint(arg1)) return RES_BADARGS;
    byte r = atoi(arg1);

    char *arg2 = strtok(NULL, " ");
    if(!validint(arg2)) return RES_BADARGS;
    byte g = atoi(arg2);

    char *arg3 = strtok(NULL, " ");
    if(!validint(arg3)) return RES_BADARGS;
    byte b = atoi(arg3);

    if(strtok(NULL, " ")) return RES_BADARGS;

    lcd.setRGB(r, g, b);
    
    return RES_OK;
}


uint8_t cmd_LCD_SETCUR(char *args) {

    // CMD params: 2 valid ints
    char *arg1 = strtok(args, " ");
    if(!validint(arg1)) return RES_BADARGS;

    char *arg2 = strtok(NULL, " ");
    if(!validint(arg2)) return RES_BADARGS;

    if(strtok(NULL, " ")) return RES_BADARGS;

    lcd.setCursor(atoi(arg1), atoi(arg2));
    return RES_OK;
}


uint8_t cmd_LCD_PRINT(char *args) {
    
    // CMD params: 1 str
    if(!args) return RES_BADARGS;

    char *string_arg = extract_strarg(args);
    if (!string_arg) return RES_BADARGS;

    lcd.print(string_arg);
    return RES_OK;
}


uint8_t cmd_FAN_SETRGB(char *args) {

    // CMD params: 3 valid ints
    char *arg1 = strtok(args, " ");
    if(!validint(arg1)) return RES_BADARGS;
    byte r = atoi(arg1);

    char *arg2 = strtok(NULL, " ");
    if(!validint(arg2)) return RES_BADARGS;
    byte g = atoi(arg2);

    char *arg3 = strtok(NULL, " ");
    if(!validint(arg3)) return RES_BADARGS;
    byte b = atoi(arg3);

    if(strtok(NULL, " ")) return RES_BADARGS;

    fan.setRGB(r, g, b);
    return RES_OK;
}

uint8_t cmd_RGB_SAVE(char *args) {
    
    // CMD params: none
    if(args) return RES_BADARGS;

    lcd.saveRGB();
    fan.saveRGB();
    // TODO: the same for lcd
    return RES_OK;
}


uint8_t cmd_RGB_LOAD(char *args) {
    
    // CMD params: none
    if(args) return RES_BADARGS;

    lcd.loadRGB();
    fan.loadRGB();
    // TODO: the same for lcd
    return RES_OK;
}

