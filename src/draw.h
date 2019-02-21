#ifndef DRAW_H
#define DRAW_H

#define WINDOW_WIDTH  64
#define WINDOW_HEIGHT 32
#define WINDOW_TITLE  "CHIP8 Emulator"

#include <SDL2/SDL.h>
#include "core.h"

extern uint8_t chip8_fontset[80];

class Draw
{
public:
    Draw(SDL_Event _event, SDL_Window *_win, SDL_Renderer *_ren);
    void initSDL(void);
    void drawSDL(Hardware *HW);
    void closeSDL(void);
    void clearScreenSDL(void);
private:
    SDL_Event    event;
    SDL_Window   *win;
    SDL_Renderer *ren;
};

#endif // DRAW_H
