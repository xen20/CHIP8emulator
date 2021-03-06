#include <cstring>

#include "keyboard.h"

Keyboard::Keyboard(SDL_Event *_event)
{
    size_t  keystate_amount = sizeof(keystates) / sizeof(keystates[0]);

    memset(keystates, 0, keystate_amount);

    debugKeys = {{"Pause", false}, {"Log", false}, {"Hardware", false}, {"Next", false}};
    event = _event;
}

Keyboard::~Keyboard()
{

}

void Keyboard::pollKeyboard(bool *isRunning)
{
    char *fullKeyName = NULL;
    char  pressedKey  = 0;

    if (SDL_PollEvent(*&event))
    {

        switch (event->type)
        {
        case SDL_QUIT:
            *isRunning = false;
            break;

        case SDL_KEYDOWN:
            fullKeyName = (char*)SDL_GetKeyName(event->key.keysym.sym);
            pressedKey  = tolower(fullKeyName[0]);
            Keypressed(pressedKey);
            break;

        case SDL_KEYUP:
            fullKeyName = (char*)SDL_GetKeyName(event->key.keysym.sym);
            pressedKey  = tolower(fullKeyName[0]);
            Keyreleased(pressedKey);
            break;
        }
    }
}

//purely for readability, let us divide two massive switch cases into two functions

void Keyboard::Keypressed(const char key)
{
    switch (key)
    {
    case HEX_0:
        keystates[0] = 1;
        break;

    case HEX_1:
        keystates[0x1] = 1;
        break;

    case HEX_2:
        keystates[0x2] = 1;
        break;

    case HEX_3:
        keystates[0x3] = 1;
        break;

    case HEX_4:
        keystates[0x4] = 1;
        break;

    case HEX_5:
        keystates[0x5] = 1;
        break;

    case HEX_6:
        keystates[0x6] = 1;
        break;

    case HEX_7:
        keystates[0x7] = 1;
        break;

    case HEX_8:
        keystates[0x8] = 1;
        break;

    case HEX_9:
        keystates[0x9] = 1;
        break;

    case HEX_A:
        keystates[0xA] = 1;
        break;

    case HEX_B:
        keystates[0xB] = 1;
        break;

    case HEX_C:
        keystates[0xC] = 1;
        break;

    case HEX_D:
        keystates[0xD] = 1;
        break;

    case HEX_E:
        keystates[0xE] = 1;
        break;

    case HEX_F:
        keystates[0xF] = 1;
        break;

    case PAUSE:
        debugKeys.at("Pause") = !debugKeys.at("Pause");
        break;

    case LOG:
        debugKeys.at("Log") = !debugKeys.at("Log");
        break;

    case HARDWARE:
        debugKeys.at("Hardware") = !debugKeys.at("Hardware");
        break;

    case NEXT:
        debugKeys.at("Next") = true;
        break;
    }
}

void Keyboard::Keyreleased(const char key)
{
    switch (key)
    {
    case HEX_0:
        keystates[0] = 0;
        break;

    case HEX_1:
        keystates[0x1] = 0;
        break;

    case HEX_2:
        keystates[0x2] = 0;
        break;

    case HEX_3:
        keystates[0x3] = 0;
        break;

    case HEX_4:
        keystates[0x4] = 0;
        break;

    case HEX_5:
        keystates[0x5] = 0;
        break;

    case HEX_6:
        keystates[0x6] = 0;
        break;

    case HEX_7:
        keystates[0x7] = 0;
        break;

    case HEX_8:
        keystates[0x8] = 0;
        break;

    case HEX_9:
        keystates[0x9] = 0;
        break;

    case HEX_A:
        keystates[0xA] = 0;
        break;

    case HEX_B:
        keystates[0xB] = 0;
        break;

    case HEX_C:
        keystates[0xC] = 0;
        break;

    case HEX_D:
        keystates[0xD] = 0;
        break;

    case HEX_E:
        keystates[0xE] = 0;
        break;

    case HEX_F:
        keystates[0xF] = 0;
        break;
    }
}
