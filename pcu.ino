#include "pcu.h"
#include "lcd.h"
#include "fan.h"

#define BAUDRATE    9600
#define CMDBUFSIZE  128

#define CHECK_TIMER(start, interval) (millis()-(start) >= (uint32_t)(interval))

static void exec(char*);
static void printResponse(Response res, char* detail);

// Macro template for cmd declarations and array definition
#define _COMMANDS       \
    _CMD(PCU_READY)     \
    _CMD(PCU_MODE)      \
    _CMD(PCU_SLEEP)     \
    _CMD(LCD_HOME)      \
    _CMD(LCD_CLEAR)     \
    _CMD(LCD_SETCUR)    \
    _CMD(LCD_PRINT)     \
    _CMD(LCD_GETRGB)    \
    _CMD(LCD_SETRGB)    \
    _CMD(FAN_GETRGB)    \
    _CMD(FAN_SETRGB)    \
    _CMD(RGB_SAVE)      \
    _CMD(RGB_LOAD)      \

// Declare function prototypes
#define _CMD(name) Response cmd_##name(const char*, char*);
_COMMANDS
#undef _CMD

// Define array of CMD name-function pairs from macro template
#define _CMD(name) {#name, cmd_##name},
struct {
    char* name;
    void *f;
} cmd_funcs[] = { _COMMANDS };

static uint8_t sleep_timeout_secs;
static uint32_t sleep_timer_start;

PCUMode current_mode;

Timer sleepTimer;
LCD lcd;
Fan fan;

void setup() {
    Serial.begin(BAUDRATE);
    lcd.start();
    fan.loadRGB();

    delay(1000);  // Mobo takes a sec to power the ARGB header. Without this, FAN RGB stays off.

    modeSwitch(MODE_IDLE);
    printResponse(RES_READY, "");
}

void loop() {
    char cmdline[CMDBUFSIZE];
    uint8_t cmd_idx = 0;
    cmdline[cmd_idx] = '\0';
    int ch;

    for (;;) {

        for (;;) {

            if (MODE_SLEEP != current_mode && sleepTimer.isReady()) modeSwitch(MODE_SLEEP);
            
            if (MODE_NORMAL != current_mode) lcd.animate();
            if (MODE_SLEEP == current_mode) fan.animateSleep();

            if ((ch = Serial.read()) >= 0) break;
        }

        // Escape: Discard any sequence immediately(ish) following
        if ('\e' == ch) {
            do {
                delay(BAUDRATE/1000+1);
            } while (Serial.read() != -1);
            continue;
        }

        // Newline
        if (ch == '\r') {
            Serial.println();
            break;
        }

        // Backspace
        if (ch == 0x7F) {
            if (cmd_idx > 0) {
                cmdline[cmd_idx--] = '\0';
                Serial.print("\b \b");
            }
            continue;
        }

        // Is there room in the buffer?
        if (cmd_idx >= (CMDBUFSIZE-1)) continue;

        // Record and echo char
        cmdline[cmd_idx++] = ch;
        cmdline[cmd_idx] = '\0';
        Serial.print((char*)&ch);
    }

    sleepTimer.reset();

    // Newline: PCU goes into NORMAL mode
    if (current_mode != MODE_NORMAL) modeSwitch(MODE_NORMAL);

    if (cmd_idx) exec(cmdline);
}

void modeSwitch(PCUMode new_mode) {
    switch(new_mode) {

    case MODE_IDLE:
        lcd.setAnimation("", ".....", 500);
        current_mode = MODE_IDLE;
        break;

    case MODE_NORMAL:
        fan.loadRGB();
        lcd.loadRGB();
        lcd.clear();
        current_mode = MODE_NORMAL;
        break;

    case MODE_SLEEP:
        lcd.setRGB(0, 0, 32);
        lcd.setAnimation("Zzzz", "...", 800);
        fan.setRGB(0, 0, 0);
        current_mode = MODE_SLEEP;
        break;
    }
}

static void exec(char* cmdline) {

    char* command = strtok(cmdline, " ");
    uint8_t (*cmd_f)(char*, char*) = NULL;

    for (int i=0; i<ARRLEN(cmd_funcs); i++) {
        if (0 == strcasecmp(command, cmd_funcs[i].name)) {
            cmd_f = cmd_funcs[i].f;
            break;
        }
    }

    char* args = strtok(NULL, "");
    char detail[64];
    detail[0] = '\0';

    if (cmd_f) {
        Response res = cmd_f(args, detail);
        printResponse(res, detail);
    } else {
        printResponse(RES_ERROR, "Invalid command");
    }
}

static void printResponse(Response res, char* detail) {
    switch(res) {
    case RES_OK:
        Serial.print("[OK]");
        break;
    case RES_READY:
        Serial.print("[READY]");
        break;
    case RES_ERROR:
        Serial.print("[ERROR]");
        break;
    }

    if (strlen(detail)) {
        Serial.print(": ");
        Serial.print(detail);
    }
    Serial.println("");
}