#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstdint>

#define HEX_1 '1'
#define HEX_2 '2'
#define HEX_3 '3'
#define HEX_C '4'

#define HEX_4 'q'
#define HEX_5 'w'
#define HEX_6 'e'
#define HEX_D 'r'

#define HEX_7 'a'
#define HEX_8 's'
#define HEX_9 'd'
#define HEX_E 'f'

#define HEX_A 'z'
#define HEX_0 'x'
#define HEX_B 'c'
#define HEX_F 'v'

extern uint8_t c8_keystates[16];

class keyboard
{
public:
    keyboard();
    void    c8_keyboard(char key, bool state);
private:
    void    c8_keypressed(char key);
    void    c8_keyreleased(char key);
};

#endif // KEYBOARD_H
