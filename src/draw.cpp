#include <cstdio>

#include "draw.h"

#define   SCREEN_HEIGHT 640//32
#define   SCREEN_WIDTH  480//64

Draw::Draw(SDL_Event _event, SDL_Window *_win, SDL_Renderer *_ren)
{
    event = _event;
    win = _win;
    ren = _ren;
}

void Draw::initSDL(void){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "SDL video init error %s\n", SDL_GetError());
        SDL_Quit();
    }
    else{
        win = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(win == NULL || ren == NULL){
            SDL_DestroyWindow(win);
            fprintf(stderr, "SDL_CreateWindow or SDL_CreateRenderer error %s\n", SDL_GetError());
            SDL_Quit();
        }
        else{
            SDL_RenderClear(ren);
        }
    }
}

void Draw::drawSDL(Hardware *HW){
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    /*this method is for the prototype stage only,
      considering that drawing a single point every cycle
      is hardly effective, and potentially very slow.
      consider replacing with SDLRenderCopy, SDL Texture or some such*/

    uint8_t current_pixel = 0;
    for(int idxY = 0; idxY < SCREEN_HEIGHT; ++idxY){
        for(int idxX = 0; idxX < SCREEN_WIDTH; ++idxX){
            current_pixel = HW->screen[idxX][idxY];
            if(current_pixel == 1) SDL_RenderDrawPoint(ren, idxX, idxY);
        }
    }
//    for (int idx = 0; idx < WINDOW_WIDTH; ++idx){
//        SDL_RenderDrawPoint(ren, idx, idx);
//    }
    SDL_RenderPresent(ren);
}

void Draw::clearScreenSDL(void){
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
}

void Draw::closeSDL(void){
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
