#include <cstdio>

#include "src/core.h"

int main(int argc, char **argv)
{
    Core emulator;
    emulator.loadROM(argv[1]);
    //loadrom
    emulator.initSDL();
    //cycle emu while nonquit
    //pollkey

    while (emulator.isRunning) {
        emulator.pollKeyboard();
    }

    printf("I am running\n");
}
