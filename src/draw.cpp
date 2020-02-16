#include <cstdio>

#include "draw.h"

Draw::Draw()
{

}

Draw::~Draw()
{

}

void Draw::initSDL(SDL_Window **_win, SDL_Renderer **_ren)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fprintf(stderr, "SDL could not init %s\n", SDL_GetError());
        SDL_Quit();
    }
    else
    {
        *_win = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_SHOWN);
        *_ren = SDL_CreateRenderer(*_win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        win = *_win;
        ren = *_ren;

        if (win == NULL || ren == NULL)
        {
            SDL_DestroyWindow(win);
            fprintf(stderr, "SDL_CreateWindow or SDL_CreateRenderer error %s\n", SDL_GetError());
            SDL_Quit();
        }
        else
        {
            SDL_RenderClear(ren);
        }
    }
}

void Draw::drawSDL(Hardware *HW)
{
    uint32_t pixels[2048];

    SDL_Texture* sdlTexture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

    HW->drawFlag = 0;

    // Store pixels in temporary buffer
    for (int i = 0; i < 2048; ++i)
    {
        uint8_t pixel = HW->screen[i];
        pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
    }

    // Update SDL texture
    SDL_UpdateTexture(sdlTexture, NULL, pixels, 64 * sizeof(uint32_t));
    // Clear screen and render
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, sdlTexture, NULL, NULL);
    SDL_RenderPresent(ren);
}

void Draw::clearScreenSDL(void)
{
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
}

void Draw::closeSDL(void)
{
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
