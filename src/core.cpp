#include <cstring>  //memset, size_t
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>

#include "core.h"
#include "random.h"
#include "draw.h"
#include "keyboard.h"
#include "fontset.h"

uint8_t c8_keystates[16];

Hardware  HW;

SDL_Event    event;
SDL_Window   *win = NULL;
SDL_Renderer *ren = NULL;

keyboard     pollkey;

Draw Draw(event, win, ren);

typedef void    (Interpreter::*interpreterFptr)(void);
typedef          std::unordered_map<uint16_t, interpreterFptr> FunctionMap;

FunctionMap interpreterFunctions;

Hardware::Hardware(){
    size_t memory_size   = sizeof(memory)/sizeof(memory[0]);
    size_t stack_size    = sizeof(stack)/sizeof(stack[0]);
    size_t register_size = sizeof(V)/sizeof(V[0]); //try removing these in favor
    // of the following syntax: memset(array, 0, sizeof(array));

    programCounter = 0x200;
    opcode         = 0;
    stackPtr       = 0;
    indexRegister  = 0;

    memset(memory, 0, memory_size);
    memset(stack, 0, stack_size);
    memset(V, 0, register_size);
    memset(screen, 0, sizeof(screen));

    loadFontset();
}

Hardware::~Hardware(){

}

void Hardware::loadFontset(void){
    size_t fontset_size = sizeof(chip8_fontset)/sizeof(chip8_fontset[0]);

    for (uint8_t idx = 0; idx < fontset_size; ++idx){
        memory[idx] = chip8_fontset[idx];
    }
}

Core::Core(){

    isRunning = true;
    drawFlag  = 1;
}

Core::~Core(){

}

void Core::loadROM(char *ROM){
    FILE *ROMStream;
    int   ROMChunk = 0, idx = 0x200; //where 0x200 is the beginning of chip8 ROM memory

    ROMStream = fopen(ROM, "rb");

    if(ROMStream == NULL) perror("Invalid ROM path given"); //maybe instead of perror solution, fprintf to stderr solution
    else{
        while(ROMChunk != EOF){
            ROMChunk = fgetc(ROMStream);
            HW.memory[idx] = ROMChunk;
            ++idx;
        }
        fclose(ROMStream);
        printf("ROM Loaded\n");
    }
}

void Core::initSDL(void){
    Draw.c8_initSDL();
    Draw.c8_drawSDL();
}

void Core::closeSDL(void){
    Draw.c8_closeSDL();
}

void Core::emulateSystem(void){

}

void Core::pollKeyboard(void){
    char *fullKeyName = NULL;
    char  pressedKey  = 0;

    if(SDL_PollEvent(&event)){
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                fullKeyName = (char*)SDL_GetKeyName(event.key.keysym.sym);
                pressedKey  = fullKeyName[0];
                pollkey.c8_keyboard(pressedKey, true);
                break;
            case SDL_KEYUP:
                fullKeyName = (char*)SDL_GetKeyName(event.key.keysym.sym);
                pressedKey  = fullKeyName[0];
                pollkey.c8_keyboard(pressedKey, false);
                break;
        }
    }
}

void Core::fetchOpcode(void){

    /*since opcode is 16-bit and memory is 8-bit,
      each opcode consists of two concequent values in memory
      which need to be added together via bitwise OR op.*/

    HW.opcode = HW.memory[HW.programCounter] << 8 | HW.memory[HW.programCounter+1];
    HW.programCounter += 2;
}

uint16_t Core::decodeOpcode(void){

    /*we may acquire most necessary opcodes to execute by decoding
      the highest 4 bits and the lowest 4 bits of a given opcode.
      there are a few noted exceptions, where more bits or less
      bits are required.*/

    uint16_t decodedOpcode = 0;
    uint16_t temp = 0;

    decodedOpcode = HW.opcode & 0xF0FF;

    if((decodedOpcode == 0xF055) || decodedOpcode == 0xF065){
        return decodedOpcode;
    }
    else{
        decodedOpcode = decodedOpcode & 0xF00F;
        temp = decodedOpcode >> 12;
        if(temp == 0x8 || temp == 0xE || temp == 0xF || temp == 0){
            //handle any possible case where two bit sequences are significant (msb and lsb)
            return decodedOpcode;
        }
        else{
            //handle any case where only the most signifcant bits are needed
            return decodedOpcode & 0xF000;
        }
    }
}

void Core::indexOpcodes(void){
    interpreterFptr opcodeFuncPtr[] = {&Interpreter::_00E0, &Interpreter::_00EE, &Interpreter::_1NNN, &Interpreter::_2NNN, &Interpreter::_3XKK,
                                       &Interpreter::_4XKK, &Interpreter::_5XY0, &Interpreter::_6XKK, &Interpreter::_7XKK, &Interpreter::_8XY0,
                                       &Interpreter::_8XY1, &Interpreter::_8XY2, &Interpreter::_8XY3, &Interpreter::_8XY4,
                                       &Interpreter::_8XY5, &Interpreter::_8XY6, &Interpreter::_8XY7, &Interpreter::_8XYE, &Interpreter::_9XY0,
                                       &Interpreter::_ANNN, &Interpreter::_BNNN, &Interpreter::_CXKK, &Interpreter::_DXYN, &Interpreter::_EX9E,
                                       &Interpreter::_EXA1, &Interpreter::_FX07, &Interpreter::_FX0A, &Interpreter::_FX15, &Interpreter::_FX18,
                                       &Interpreter::_FX1E, &Interpreter::_FX29, &Interpreter::_FX33, &Interpreter::_FX55, &Interpreter::_FX65};

    uint16_t      opcodeFuncIdx[] =   {0x00E0, 0x00EE, 0x1000, 0x2000, 0x3000,
                                       0x4000, 0x5000, 0x6000, 0x7000, 0x8000,
                                       0x8001, 0x8002, 0x8003, 0x8004, 0x8005,
                                       0x8006, 0x8007, 0x800E, 0x9000, 0xA000,
                                       0xB000, 0xC000, 0xD000, 0xE00E, 0xE001,
                                       0xF007, 0xF00A, 0xF005, 0xF008, 0xF00E,
                                       0xF009, 0xF003, 0xF055, 0xF065};

    size_t        functionAmount = sizeof(opcodeFuncIdx)/sizeof(opcodeFuncIdx[0]);

    for(uint8_t idx = 0; idx < functionAmount; ++idx){
        interpreterFunctions.emplace(opcodeFuncIdx[idx], opcodeFuncPtr[idx]);
    }
}

void Core::interpretOpcode(void){
    Interpreter Interpret;

    auto iter = interpreterFunctions.find(HW.opcode);

    if(iter == interpreterFunctions.end()){
        perror("Invalid opcode detected");
    }
    else{
        (Interpret.*(iter->second))();
    }
}

Interpreter::Interpreter(){

}

Interpreter::~Interpreter(){

}

void Interpreter::_00E0(void){
    Draw.c8_clearScreenSDL();
}

void Interpreter::_00EE(void){

    HW.programCounter = HW.stack[HW.stackPtr];
    --HW.stackPtr;
}

void Interpreter::_1NNN(void){

    HW.programCounter = NNN;
}

void Interpreter::_2NNN(void){

    ++HW.stackPtr;
    HW.stack[HW.stackPtr] = HW.programCounter;
    HW.programCounter = NNN;
}

void Interpreter::_3XKK(void){

    if(HW.V[X] == KK){
        HW.programCounter += 2;
    }
}

void Interpreter::_4XKK(void){

    if(HW.V[X] != KK){
        HW.programCounter += 2;
    }
}

void Interpreter::_5XY0(void){

    if(HW.V[X] == HW.V[Y]){
        HW.programCounter += 2;
    }
}

void Interpreter::_6XKK(void){

    HW.V[X] = KK;
}

void Interpreter::_7XKK(void){

    HW.V[X] += KK;
}

void Interpreter::_8XY0(void){

    HW.V[X] = HW.V[Y];
}

void Interpreter::_8XY1(void){

    HW.V[X] |= HW.V[Y];
}

void Interpreter::_8XY2(void){

    HW.V[X] &= HW.V[Y];
}

void Interpreter::_8XY3(void){

    HW.V[X] ^= HW.V[Y];
}

void Interpreter::_8XY4(void){
    //implemented as idea on website, but may need a rewrite (see "base" chip8 program)
    uint16_t  temp = 0;

    temp = HW.V[X] + HW.V[Y];

    if(temp > 0xFF){
        HW.V[0xF] = 1;
    }
    else HW.V[0xF] = 0;

    HW.V[X] = temp >> 8;
}

void Interpreter::_8XY5(void){

    if(HW.V[X] > HW.V[Y]){
        HW.V[0xF] = 1;
    }
    else HW.V[0xF] = 0;

    HW.V[X] -= HW.V[Y];
}

void Interpreter::_8XY6(void){

    if((HW.V[X] & 0x1) == 1){
        HW.V[0xF] = 1;
    }
    else HW.V[0xF] = 0;

    HW.V[X] >>= 1; //another way to divide by 2
}

void Interpreter::_8XY7(void){

    if(HW.V[Y] > HW.V[X]){
        HW.V[0xF] = 1;
    }
    else HW.V[0xF] = 0;

    HW.V[X] = HW.V[Y] - HW.V[X];
}

void Interpreter::_8XYE(void){

    //maybe take a look at this again and see if desired value is acquired!

    if(((HW.V[X] >> 3) & 0x1) == 1 ){
        HW.V[0xF] = 1;
    }
    else HW.V[0xF] = 0;

    HW.V[X] <<= 1;  //another way to mul by 2
}

void Interpreter::_9XY0(void){

    if(HW.V[X] != HW.V[Y]) HW.programCounter += 2;
}

void Interpreter::_ANNN(void){

    HW.indexRegister = NNN;
}

void Interpreter::_BNNN(void){

    HW.programCounter = NNN+HW.V[0];
}

void Interpreter::_CXKK(void){
    uint8_t randomNum = genRandomNum();

    HW.V[X] = (randomNum & KK);
}

void Interpreter::_DXYN(void){
    //maximum sprite size is 8x15, where it is the height that is adjustable
    HW.V[0xF] = 0;
    uint8_t current_byte = 0;

    for(int idxY = 0; idxY < N; ++idxY){
        current_byte = HW.memory[HW.indexRegister+idxY];
        for(int idxX = 0; idxX < 8; ++idxX){
            if((current_byte >> idxX ^ HW.screen[HW.V[X]+idxX][HW.V[Y]+idxY]) == 1){
                HW.V[0xF] = 1;
            }
        }
    };
}

void Interpreter::_EX9E(void){
    if(c8_keystates[HW.V[X]] == 1){
        HW.programCounter += 2;
    }
}

void Interpreter::_EXA1(void){
    if(c8_keystates[HW.V[X]] == 0){
        HW.programCounter += 2;
    }
}

void Interpreter::_FX07(void){

    HW.V[X] = HW.delayTimer;
}

void Interpreter::_FX0A(void){
    int temp = -1;
    for (int idx=0; idx<16; ++idx) {
        if (c8_keystates[idx]) {
            temp = idx;
            break;
        }
    }
    if (temp != -1)
        HW.V[X] = temp;
    else
        HW.programCounter -= 2;
}

void Interpreter::_FX15(void){

    HW.delayTimer = HW.V[X];
}

void Interpreter::_FX18(void){

    HW.soundTimer = HW.V[X];
}

void Interpreter::_FX1E(void){

    HW.indexRegister += HW.V[X];
}

void Interpreter::_FX29(void){

}

void Interpreter::_FX33(void){
    //Refer wikipedia/BCD 8421
    HW.memory[HW.indexRegister]     =  HW.V[X] / 100;
    HW.memory[HW.indexRegister + 1] = (HW.V[X] / 10) % 10;
    HW.memory[HW.indexRegister + 2] = (HW.V[X] % 100) % 10;
    HW.programCounter += 2;
}

void Interpreter::_FX55(void){
    for(uint8_t idx = 0; idx <= X; ++idx){
        HW.memory[HW.indexRegister + idx] = HW.V[idx];
    }
}

void Interpreter::_FX65(void){
    for(uint8_t idx = 0; idx <= X; ++idx){
        HW.V[idx] = HW.memory[HW.indexRegister + idx];
    }
}
