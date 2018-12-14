#include <cstring>
#include <cstdio>

#include "keyboard.h"

keyboard::keyboard(){
    size_t  keystate_amount = sizeof(c8_keystates)/sizeof(c8_keystates[0]);

    memset(c8_keystates, 0, keystate_amount);
}

void keyboard::c8_keyboard(const char key, bool state){
    if(state == true){
        c8_keypressed(key);
    }
    else if(state == false){
        c8_keyreleased(key);
    }
    printf("%c", key); //debug
}

//purely for readability, let us divide two massive switch cases into two functions

void keyboard::c8_keypressed(const char key){
    switch(key){
        case HEX_0:
            c8_keystates[0] = 1;
        case HEX_1:
            c8_keystates[0x1] = 1;
        case HEX_2:
            c8_keystates[0x2] = 1;
        case HEX_3:
            c8_keystates[0x3] = 1;
        case HEX_4:
            c8_keystates[0x4] = 1;
        case HEX_5:
            c8_keystates[0x5] = 1;
        case HEX_6:
            c8_keystates[0x6] = 1;
        case HEX_7:
            c8_keystates[0x7] = 1;
        case HEX_8:
            c8_keystates[0x8] = 1;
        case HEX_9:
            c8_keystates[0x9] = 1;
        case HEX_A:
            c8_keystates[0xA] = 1;
        case HEX_B:
            c8_keystates[0xB] = 1;
        case HEX_C:
            c8_keystates[0xC] = 1;
        case HEX_D:
            c8_keystates[0xD] = 1;
        case HEX_E:
            c8_keystates[0xE] = 1;
        case HEX_F:
            c8_keystates[0xF] = 1;
    }
}

void keyboard::c8_keyreleased(const char key){
    switch(key){
        case HEX_0:
            c8_keystates[0] = 0;
        case HEX_1:
            c8_keystates[0x1] = 0;
        case HEX_2:
            c8_keystates[0x2] = 0;
        case HEX_3:
            c8_keystates[0x3] = 0;
        case HEX_4:
            c8_keystates[0x4] = 0;
        case HEX_5:
            c8_keystates[0x5] = 0;
        case HEX_6:
            c8_keystates[0x6] = 0;
        case HEX_7:
            c8_keystates[0x7] = 0;
        case HEX_8:
            c8_keystates[0x8] = 0;
        case HEX_9:
            c8_keystates[0x9] = 0;
        case HEX_A:
            c8_keystates[0xA] = 0;
        case HEX_B:
            c8_keystates[0xB] = 0;
        case HEX_C:
            c8_keystates[0xC] = 0;
        case HEX_D:
            c8_keystates[0xD] = 0;
        case HEX_E:
            c8_keystates[0xE] = 0;
        case HEX_F:
            c8_keystates[0xF] = 0;
    }
}
