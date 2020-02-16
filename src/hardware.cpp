#include <cstring>

#include "hardware.h"
#include "fontset.h"


Hardware::Hardware()
{
    programCounter = 0x200;
    opcode         = 0;
    stackPtr       = 0;
    indexRegister  = 0;

    memset(memory, 0, sizeof(memory));
    memset(stack, 0, sizeof(stack));
    memset(V, 0, sizeof(V));
    memset(screen, 0, sizeof(screen));
    loadFontset();
    drawFlag  = 0;
}

Hardware::~Hardware()
{
}

void Hardware::loadFontset(void)
{
    size_t fontset_size = sizeof(chip8_fontset) / sizeof(chip8_fontset[0]);

    for (uint8_t idx = 0; idx < fontset_size; ++idx)
        memory[idx] = chip8_fontset[idx];
}
