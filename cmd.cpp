#include "pcu.h"
#include "lcd.h"
#include "fan.h"

#define ISDELIMITER(c)  ('"' == c || '\'' == c)
#define ISWHITESPACE(c) (' ' == c || '\t' == c)

extern Timer sleepTimer;
extern LCD lcd;
extern Fan fan;

bool validint(char* s) {
    if (NULL == s || '\0' == *s) return false;

    while (*s) {
        if (!isdigit(*s++)) return false;
    }

    return true;
}

char* extract_strarg(char* arg_string)
{
    // Advance to first non whitespace character
    while(ISWHITESPACE(*arg_string)) arg_string++;

    // First non space character must be a valid delimeter
    char delim = *arg_string;
    
    if (!ISDELIMITER(delim)) return NULL;  // Missing/invalid starting delimeter

    // Find closing delimeter
    char* close_delim = strchr((arg_string+1), delim);
    if (NULL == close_delim) return NULL;  // Missing closing delimeter

    // Check for garbage after closing delimeter
    for (char* p = close_delim+1; *p; p++) {
        if ('\0' == *p) break;
        if(!ISWHITESPACE(*p)) return NULL;  // Extra tok after closing delim
    }

    // String arg is valid. Mark delimeters and return extracted string.
    *arg_string++ = '\0';
    *close_delim = '\0';

    return arg_string;
}

/*
    PCU Command functions
*/
Response cmd_PCU_READY(const char* args, char* res_detail) {
    
    if(args) {
        strcpy(res_detail, "This command takes no arguments");
        return RES_ERROR;
    }
    return RES_READY;
}

Response cmd_PCU_MODE(const char* args, char* res_detail) {
    char* arg1 = strtok(args, " ");
    if (!arg1 || strtok(NULL, " ")) {
        strcpy(res_detail, "This command exactly 1 argument");
        return RES_ERROR;
    }

    if (0 == strcasecmp("IDLE", arg1)) {
        modeSwitch(MODE_IDLE);
        return RES_OK;
    }

    if (0 == strcasecmp("NORMAL", arg1)) {
        modeSwitch(MODE_NORMAL);
        return RES_OK;
    }

    if (0 == strcasecmp("SLEEP", arg1)) {
        modeSwitch(MODE_SLEEP);
        return RES_OK;
    }

    strcpy(res_detail, "Invalid mode");
    return RES_ERROR;
}

Response cmd_PCU_SLEEP(const char* args, char* res_detail) {
    
    char* arg1 = strtok(args, " ");
    if (!arg1 || strtok(NULL, " ")) {
        strcpy(res_detail, "This command exactly 1 argument");
        return RES_ERROR;
    }
    if (!validint(arg1)) {
        strcpy(res_detail, "Invalid argument type(s)");
        return RES_ERROR;
    }

    sleepTimer.set(atoi(arg1));
    return RES_OK;
}

Response cmd_LCD_HOME(const char* args, char* res_detail) {
    
    if (args) {
        strcpy(res_detail, "This command takes no arguments");
        return RES_ERROR;
    }

    lcd.home();
    return RES_OK;
}

Response cmd_LCD_CLEAR(const char* args, char* res_detail) {
    
    if (args) {
        strcpy(res_detail, "This command takes no arguments");
        return RES_ERROR;
    }

    lcd.clear();
    return RES_OK;
}

Response cmd_LCD_SETCUR(const char* args, char* res_detail) {

    char* arg1 = strtok(args, " ");
    char* arg2 = strtok(NULL, " ");

    if (!arg1 || !arg2 || strtok(NULL, " ")) {
        strcpy(res_detail, "This command takes exactly 2 arguments");
        return RES_ERROR;
    }

    if (!validint(arg1) || !validint(arg2)) {
        strcpy(res_detail, "Invalid argument type(s)");
        return RES_ERROR;
    }

    lcd.setCursor(atoi(arg1), atoi(arg2));
    return RES_OK;
}

Response cmd_LCD_PRINT(const char* args, char* res_detail) {
    char* string_arg = extract_strarg(args);
    if (!string_arg) {
        strcpy(res_detail, "Improperly formatted string");
        return RES_ERROR;}

    lcd.print(string_arg);
    return RES_OK;
}

Response cmd_LCD_GETRGB(const char* args, char* res_detail) {

    if (args) {
        strcpy(res_detail, "This command takes no arguments");
        return RES_ERROR;
    }

    RGBValue rgb = lcd.getRGB();
    sprintf(res_detail, "(%d, %d, %d)", rgb.r, rgb.g, rgb.b);
    return RES_OK;
}

Response cmd_LCD_SETRGB(const char* args, char* res_detail) {

    char* arg1 = strtok(args, " ");
    char* arg2 = strtok(NULL, " ");
    char* arg3 = strtok(NULL, " ");

    if (!arg1 || !arg2 || !arg3 || strtok(NULL, " ")) {
        strcpy(res_detail, "This command takes exactly 3 arguments");
        return RES_ERROR;
    }

    if (!validint(arg1) || !validint(arg2) || !validint(arg3)) {
        strcpy(res_detail, "Invalid argument type(s)");
        return RES_ERROR;
    }

    lcd.setRGB(atoi(arg1), atoi(arg2), atoi(arg3));
    return RES_OK;
}

Response cmd_FAN_GETRGB(const char* args, char* res_detail) {

    if (args) {
        strcpy(res_detail, "This command takes no arguments");
        return RES_ERROR;
    }

    RGBValue rgb = fan.getRGB();
    sprintf(res_detail, "(%d, %d, %d)", rgb.r, rgb.g, rgb.b);
    return RES_OK;
}

Response cmd_FAN_SETRGB(const char* args, char* res_detail) {

    char* arg1 = strtok(args, " ");
    char* arg2 = strtok(NULL, " ");
    char* arg3 = strtok(NULL, " ");

    if (!arg1 || !arg2 || !arg3 || strtok(NULL, " ")) {
        strcpy(res_detail, "This command takes exactly 3 arguments");
        return RES_ERROR;
    }

    if (!validint(arg1) || !validint(arg2) || !validint(arg3)) {
        strcpy(res_detail, "Invalid argument type(s)");
        return RES_ERROR;
    }

    fan.setRGB(atoi(arg1), atoi(arg2), atoi(arg3));
    return RES_OK;
}

Response cmd_RGB_SAVE(const char* args, char* res_detail) {
    
    if (args) {
        strcpy(res_detail, "This command takes no arguments");
        return RES_ERROR;
    }

    lcd.saveRGB();
    fan.saveRGB();
    return RES_OK;
}

Response cmd_RGB_LOAD(const char* args, char* res_detail) {
    
    if (args) {
        strcpy(res_detail, "This command takes no arguments");
        return RES_ERROR;
    }

    lcd.loadRGB();
    fan.loadRGB();
    return RES_OK;
}
