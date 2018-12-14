#include <SDL2/SDL.h>
#include <cstdio>

#include "draw.h"

uint8_t chip8_fontset[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Draw::Draw(SDL_Event _event, SDL_Window *_win, SDL_Renderer *_ren)
{
    event = _event;
    win = _win;
    ren = _ren;
}

void Draw::c8_initSDL(void){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL video init error %s\n", SDL_GetError());
        SDL_Quit();
    }
    else{
        win = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(win == NULL || ren == NULL){
            SDL_DestroyWindow(win);
            printf("SDL_CreateWindow or SDL_CreateRenderer error %s\n", SDL_GetError());
            SDL_Quit();
        }
        else{
            SDL_RenderClear(ren);
        }
    }
}

void Draw::c8_drawSDL(void){
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    for (int idx = 0; idx < WINDOW_WIDTH; ++idx){
        SDL_RenderDrawPoint(ren, idx, idx);
    }
    SDL_RenderPresent(ren);
}

void Draw::c8_closeSDL(void){
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
