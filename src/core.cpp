#include <cstdlib>        //exit()
#include <cstring>
#include <unordered_map>

#include "core.h"
#include "interpreter.h"

uint8_t      specialKeys[2];
bool         _debug;

typedef void (Interpreter::*interpreterFptr)(void);
typedef std::unordered_map<uint16_t, interpreterFptr> FunctionMap;

FunctionMap interpreterFunctions;

Core::Core() : KBD(&this->event), Interpret(&this->HW)
{
    indexOpcodes();

    win = NULL;
    ren = NULL;

    memset(debugBuffer, 0, sizeof(debugBuffer));
    isRunning = true;
}

Core::~Core()
{
}

void Core::loadROM(char *ROM)
{
    FILE *ROMStream;
    int   ROMChunk = 0, idx = 0x200; //where 0x200 is the beginning of chip8 ROM memory

    ROMStream = fopen(ROM, "rb");

    if (ROMStream == NULL)
    {
        fprintf(stderr, "Failed to load ROM: %s\n", ROM);
        exit(-1);
    }
    else
    {
        while (ROMChunk != EOF)
        {
            ROMChunk = fgetc(ROMStream);
            HW.memory[idx] = ROMChunk;
            ++idx;
        }

        fclose(ROMStream);
        printf("ROM Loaded\n");
    }
}

void Core::initSDL(void)
{
    DR.initSDL(&win, &ren);
}

void Core::closeSDL(void)
{
    DR.closeSDL();
}

void Core::emulateSystem(void)
{
    KBD.pollKeyboard(&isRunning);
    fetchOpcode();
    interpretOpcode();

    if (HW.drawFlag == 1)
    {
        DR.drawSDL(&HW);
        HW.drawFlag = 0;
    }
}

void Core::fetchOpcode(void)
{
    /*since opcode is 16-bit and memory is 8-bit,
      each opcode consists of two concequent values in memory
      which need to be added together via bitwise OR op.*/

    HW.opcode = HW.memory[HW.programCounter] << 8 | HW.memory[HW.programCounter + 1];
    HW.programCounter += 2;
    //it is likely that all this business about pc+=4 (e.g 4xKK) is unnecessary, since pc+=2 on fetch
}

uint16_t Core::decodeOpcode(void)
{
    /*we may acquire most necessary opcodes to execute by decoding
      the highest 4 bits and the lowest 4 bits of a given opcode.
      there are a few noted exceptions, where more bits or less
      bits are required.*/

    uint16_t decodedOpcode = 0;
    uint16_t temp = 0;

    decodedOpcode = HW.opcode & 0xF0FF;

    if (decodedOpcode == 0xF055 || decodedOpcode == 0xF065 || decodedOpcode == 0x00E0 || decodedOpcode == 0x00EE)
    {
        return decodedOpcode;
    }
    else
    {
        decodedOpcode = decodedOpcode & 0xF00F;
        temp = decodedOpcode >> 12;

        if (temp == 0x8 || temp == 0xE || temp == 0xF || temp == 0)
        {
            //handle any possible case where two bit sequences are significant (msb and lsb)
            return decodedOpcode;
        }
        else
        {
            //handle any case where only the most signifcant bits are needed
            return decodedOpcode & 0xF000;
        }
    }
}

void Core::indexOpcodes(void)
{
    interpreterFptr opcodeFuncPtr[] = {&Interpreter::_00E0, &Interpreter::_00EE, &Interpreter::_1NNN, &Interpreter::_2NNN, &Interpreter::_3XKK,
                                       &Interpreter::_4XKK, &Interpreter::_5XY0, &Interpreter::_6XKK, &Interpreter::_7XKK, &Interpreter::_8XY0,
                                       &Interpreter::_8XY1, &Interpreter::_8XY2, &Interpreter::_8XY3, &Interpreter::_8XY4, &Interpreter::_8XY5,
                                       &Interpreter::_8XY6, &Interpreter::_8XY7, &Interpreter::_8XYE, &Interpreter::_9XY0, &Interpreter::_ANNN,
                                       &Interpreter::_BNNN, &Interpreter::_CXKK, &Interpreter::_DXYN, &Interpreter::_EX9E, &Interpreter::_EXA1,
                                       &Interpreter::_FX07, &Interpreter::_FX0A, &Interpreter::_FX15, &Interpreter::_FX18, &Interpreter::_FX1E,
                                       &Interpreter::_FX29, &Interpreter::_FX33, &Interpreter::_FX55, &Interpreter::_FX65
                                      };

    uint16_t      opcodeFuncIdx[] =   {0x00E0, 0x00EE, 0x1000, 0x2000, 0x3000,
                                       0x4000, 0x5000, 0x6000, 0x7000, 0x8000,
                                       0x8001, 0x8002, 0x8003, 0x8004, 0x8005,
                                       0x8006, 0x8007, 0x800E, 0x9000, 0xA000,
                                       0xB000, 0xC000, 0xD000, 0xE00E, 0xE001,
                                       0xF007, 0xF00A, 0xF005, 0xF008, 0xF00E,
                                       0xF009, 0xF003, 0xF055, 0xF065
                                      };

    size_t        functionAmount = sizeof(opcodeFuncIdx) / sizeof(opcodeFuncIdx[0]);

    for (uint8_t idx = 0; idx < functionAmount; ++idx)
    {
        interpreterFunctions.emplace(opcodeFuncIdx[idx], opcodeFuncPtr[idx]);
    }
}

void Core::interpretOpcode(void)
{
    static int iterations = 0;

    uint16_t rawOpcode = decodeOpcode(); // rawOpcode - opcode stripped of x, y, k values etc.
    // simplified for the purpose of accessing function from table

    auto iter = interpreterFunctions.find(rawOpcode);

    _debug = true;

    if (_debug)
    {
        auto n = HW.opcode & 0x0FFF;
        auto pc = HW.programCounter;

        snprintf(debugBuffer, 100, "iter: %i, opcode: %x, rawopcode: %x, pc: %i, NNN: %x\n", iterations, HW.opcode, rawOpcode, pc, n);
        logger.logCurrentOpcode(debugBuffer);
    }

    if (iter == interpreterFunctions.end())
    {
        fprintf(stderr, "Invalid opcode detected\n");
        fprintf(stderr, "%i\n", iterations);
        _debug = false;
        exit(-2);
    }
    else
    {
        ++iterations; // Used in debug
        (Interpret.*(iter->second))(); // Execute instruction
    }
}
