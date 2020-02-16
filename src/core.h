#ifndef CORE_H
#define CORE_H

#include <cstdint>
#include <SDL.h>

#include "hardware.h"
#include "interpreter.h"
#include "draw.h"
#include "keyboard.h"
#include "debugutils.h"
#include "sound.h"

typedef void (Interpreter::*interpreterFptr)(void);
typedef std::map<uint16_t, interpreterFptr> FunctionMap;

class Core
{
    public:

        Core();
        virtual ~Core();
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
        void emulateSingleCycle(void);
        void handleTimers(void);
        void getDebugInfo(void);
        void getRegisterInfo(void);

        SDL_Event    event;
        SDL_Window   *win;
        SDL_Renderer *ren;

        Hardware hardware;
        FunctionMap interpreterFunctions;
        Interpreter interpret;
        Draw draw;
        Keyboard keyboard;
        Sound sound;
        Logger logger;
        int iterations;
};

#endif // CORE_H
