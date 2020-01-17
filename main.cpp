#include <cstdio>

#include "src/core.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "USAGE: [PROGRAM] [ROM]\n");
        return -1;
    }

    char *rom_to_load = argv[1];
    Core emulator;

    emulator.loadROM(rom_to_load);
    emulator.initSDL();

    while (emulator.isRunning)
    {
        emulator.emulateSystem();
    }

    return 0;
}
