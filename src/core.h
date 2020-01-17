#ifndef CORE_H
#define CORE_H

#include <cstdint>

#include <SDL.h>

#include "hardware.h"
#include "interpreter.h"
#include "draw.h"
#include "keyboard.h"
#include "debugutils.h"

class Core
{
    public:

        Core();
        ~Core();
        void loadROM(char *ROM);
        void initSDL(void);
        void drawSDL(void);
        void closeSDL(void);
        void emulateSystem(void);
        bool isRunning;

    private:

        void fetchOpcode(void);
        uint16_t decodeOpcode(void);
        void indexOpcodes(void);
        void interpretOpcode(void);
        char debugBuffer[100];

        SDL_Event    event;
        SDL_Window   *win;
        SDL_Renderer *ren;

        Hardware HW;
        Interpreter Interpret;
        Draw DR;
        Keyboard KBD;
        DebugUtils logger;
};

#endif // CORE_H
