#include "pcu.h"
#include "lcd.h"
#include "fan.h"

#define BAUDRATE    9600
#define CMDBUFSIZE  128

#define CHECK_TIMER(start, interval) (millis()-(start) >= (uint32_t)(interval))

void mode_switch(pcumode);
static void exec(char*);
static void printres(response res);

// Macro template for cmd declarations and array definition
#define _COMMANDS       \
    _CMD(PCU_READY)     \
    _CMD(PCU_MODE)      \
    _CMD(SET_SLEEP)     \
    _CMD(FAN_SETRGB)    \
    _CMD(LCD_HOME)      \
    _CMD(LCD_CLEAR)     \
    _CMD(LCD_SETRGB)    \
    _CMD(LCD_SETCUR)    \
    _CMD(LCD_PRINT)     \
    _CMD(RGB_SAVE)      \
    _CMD(RGB_LOAD)      \

// Declare function prototypes
#define _CMD(name) uint8_t cmd_##name(char *);
_COMMANDS
#undef _CMD

// Define array of CMD name-function pairs from macro template
#define _CMD(name) {#name, cmd_##name},
struct {
    char *name;
    void *f;
} cmd_funcs[] = { _COMMANDS };


static uint8_t sleep_timeout_secs;
static uint32_t sleep_timer_start;

pcumode current_mode;

Timer sleepTimer;
LCD lcd;
Fan fan;


void setup() {
    Serial.begin(BAUDRATE);
    lcd.start();
    fan.loadRGB();

    mode_switch(IDLE);
    printres(RES_READY);
}

void loop() {
    char cmdline[CMDBUFSIZE];
    uint8_t cmd_idx = 0;
    cmdline[cmd_idx] = '\0';
    int ch;

    for (;;) {

        for (;;) {

            if (SLEEP != current_mode && sleepTimer.isReady()) mode_switch(SLEEP);
            
            if (NORMAL != current_mode) lcd.animate();
            if (SLEEP == current_mode) fan.animateSleep();

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
        Serial.print((char *)&ch);
    }

    sleepTimer.reset();

    // Newline: PCU goes into NORMAL mode
    if (current_mode != NORMAL) mode_switch(NORMAL);

    if (cmd_idx) exec(cmdline);
}


void mode_switch(pcumode new_mode) {
    switch(new_mode) {

    case IDLE:
        lcd.setAnimation("", ".....", 500);
        current_mode = IDLE;
        break;

    case NORMAL:
        lcd.clear();
        fan.loadRGB();
        current_mode = NORMAL;
        break;

    case SLEEP:
        lcd.setRGB(0, 0, 32);
        lcd.setAnimation("Zzzz", "...", 800);
        fan.setRGB(0, 0, 0);
        current_mode = SLEEP;
        break;
    }

}


static void exec(char *cmdline) {
    char *command = strtok(cmdline, " ");
    char *args = strtok(NULL, "");

    uint8_t (*cmd_f)(char *) = NULL;

    for (int i=0; i<ARRLEN(cmd_funcs); i++) {
        if (0 == strcasecmp(command, cmd_funcs[i].name)) {
            cmd_f = cmd_funcs[i].f;
            break;
        }
    }

    if (cmd_f) {
        printres(cmd_f(args));
    }
    else {
        printres(RES_BADCMD);
    }
}


static void printres(response res) {
    char *RES_STRS[] = {
        [RES_OK]      = "[OK]",
        [RES_READY]   = "[READY]",
        [RES_BADCMD]  = "[ERROR]: Invalid command",
        [RES_BADARGS] = "[ERROR]: Invalid argument(s)",
    };
    Serial.println(RES_STRS[res]);
}