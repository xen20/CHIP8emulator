#ifndef DRAW_H
#define DRAW_H

#define SCREEN_HEIGHT 640
#define SCREEN_WIDTH  480
#define WINDOW_TITLE  "CHIP8 Emulator"

#include <SDL.h>
#include "hardware.h"

extern uint8_t chip8_fontset[80];

class Draw
{
public:

    Draw();
    void initSDL(SDL_Window **_win, SDL_Renderer **_ren);
    void drawSDL(Hardware *HW);
    void closeSDL(void);
    void clearScreenSDL(void);

private:

    SDL_Window   *win;
    SDL_Renderer *ren;
};

#endif // DRAW_H
