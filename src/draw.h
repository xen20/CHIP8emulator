#ifndef DRAW_H
#define DRAW_H

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE  "Chip8 E-mo-la-tor"

#include <SDL2/SDL.h>

extern uint8_t chip8_fontset[80];

class Draw
{
public:
    Draw(SDL_Event _event, SDL_Window *_win, SDL_Renderer *_ren);
    void c8_initSDL(void);
    void c8_drawSDL(void);
    void c8_closeSDL(void);
private:
    SDL_Event    event;
    SDL_Window   *win;
    SDL_Renderer *ren;
};

#endif // DRAW_H
