#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstdint>
#include <SDL.h>

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

#define DEBUG 'p'

extern uint8_t keystates[16];
extern uint8_t specialKeys[2];

class Keyboard
{
    public:

        Keyboard(SDL_Event *_event);
        ~Keyboard();
        void pollKeyboard(bool *isRunning);

    private:

        SDL_Event    *event;
        void    Keypressed(char key);
        void    Keyreleased(char key);
};

#endif // KEYBOARD_H
