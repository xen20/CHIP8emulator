#include <cstdio>

#include "src/core.h"

int main(int argc, char **argv)
{
    Core emulator;
    emulator.loadROM(argv[1]);
    emulator.initSDL();

    while (emulator.isRunning) {
        emulator.pollKeyboard();
    }

    printf("I am running\n");
}
