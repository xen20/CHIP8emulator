#include <cstdio>

#include "src/core.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: [PROGRAM] [ROM]\n");
        return -1;
    }

    char *romToLoad = argv[1];
    Core emulator;

    emulator.loadROM(romToLoad);
    emulator.initSDL();

    while (emulator.isRunning)
        emulator.emulateSystem();

    return 0;
}



