#include <cstring>        //memset(), size_t
#include <cstdlib>        //exit()
#include <ctime>          //for functions in DebugUtils
#include <cstring>
#include <unordered_map>

#include <SDL.h>

#include "core.h"
#include "random.h"
#include "draw.h"
#include "keyboard.h"
#include "fontset.h"

uint8_t      keystates[16];
uint8_t      specialKeys[2];
bool         _debug;

Hardware     HW;

SDL_Event    event;
SDL_Window   *win = NULL;
SDL_Renderer *ren = NULL;

Draw         Draw(event, win, ren);
Keyboard     Keyboard(event, win, ren);

typedef void    (Interpreter::*interpreterFptr)(void);
typedef          std::unordered_map<uint16_t, interpreterFptr> FunctionMap;

FunctionMap interpreterFunctions;

Hardware::Hardware(){

    programCounter = 0x200;
    opcode         = 0;
    stackPtr       = 0;
    indexRegister  = 0;

    memset(memory, 0, sizeof(memory));
    memset(stack, 0, sizeof (stack));
    memset(V, 0, sizeof(V));
    memset(screen, 0, sizeof(screen));

    loadFontset();
    drawFlag  = 0;
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
    indexOpcodes();
    isRunning = true;
}

Core::~Core(){

}

void Core::loadROM(char *ROM){
    FILE *ROMStream;
    int   ROMChunk = 0, idx = 0x200; //where 0x200 is the beginning of chip8 ROM memory

    ROMStream = fopen(ROM, "rb");

    if(ROMStream == NULL){
        fprintf(stderr, "Failed to load ROM: %s", ROM);
        exit(-1);
    }
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
    Draw.initSDL();
}

void Core::closeSDL(void){
    Draw.closeSDL();
}

void Core::emulateSystem(void){
    Keyboard.pollKeyboard(&isRunning);
    fetchOpcode();
    interpretOpcode();

    if(HW.drawFlag == 1){
        Draw.drawSDL(&HW);
        HW.drawFlag = 0;
    }
}

void Core::fetchOpcode(void){

    /*since opcode is 16-bit and memory is 8-bit,
      each opcode consists of two concequent values in memory
      which need to be added together via bitwise OR op.*/

    HW.opcode = HW.memory[HW.programCounter] << 8 | HW.memory[HW.programCounter+1];
    HW.programCounter += 2;
    //it is likely that all this business about pc+=4 (e.g 4xKK) is unnecessary, since pc+=2 on fetch
}

uint16_t Core::decodeOpcode(void){

    /*we may acquire most necessary opcodes to execute by decoding
      the highest 4 bits and the lowest 4 bits of a given opcode.
      there are a few noted exceptions, where more bits or less
      bits are required.*/

    uint16_t decodedOpcode = 0;
    uint16_t temp = 0;

    decodedOpcode = HW.opcode & 0xF0FF;

    if(decodedOpcode == 0xF055 || decodedOpcode == 0xF065 || decodedOpcode == 0x00E0 || decodedOpcode == 0x00EE){
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
                                       &Interpreter::_8XY1, &Interpreter::_8XY2, &Interpreter::_8XY3, &Interpreter::_8XY4, &Interpreter::_8XY5,
                                       &Interpreter::_8XY6, &Interpreter::_8XY7, &Interpreter::_8XYE, &Interpreter::_9XY0, &Interpreter::_ANNN,
                                       &Interpreter::_BNNN, &Interpreter::_CXKK, &Interpreter::_DXYN, &Interpreter::_EX9E, &Interpreter::_EXA1,
                                       &Interpreter::_FX07, &Interpreter::_FX0A, &Interpreter::_FX15, &Interpreter::_FX18, &Interpreter::_FX1E,
                                       &Interpreter::_FX29, &Interpreter::_FX33, &Interpreter::_FX55, &Interpreter::_FX65};

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
    static int iterations = 0;

    uint16_t rawOpcode = decodeOpcode(); // rawOpcode - opcode stripped of x, y, k values etc.
                                         // simplified for the purpose of accessing function from table

    auto iter = interpreterFunctions.find(rawOpcode);

    _debug = true;

    if(_debug){
        auto n = NNN;
        auto pc = HW.programCounter;

        snprintf(debugBuffer, 100, "iter: %i, opcode: %x, rawopcode: %x, pc: %i, NNN: %x\n", iterations, HW.opcode, rawOpcode, pc, n);
        logger.logCurrentOpcode(debugBuffer);
    }


    if(iter == interpreterFunctions.end()){
        fprintf(stderr, "Invalid opcode detected\n");
        fprintf(stderr, "%i\n", iterations);
        if(!_debug) exit(-2);
    }
    else{
        ++iterations; // Used in debug
        (Interpret.*(iter->second))(); // Execute instruction
    }
}

Interpreter::Interpreter(){

}

Interpreter::~Interpreter(){

}

// Full documentation available here as of 09.08.2019 :
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.0

// Clear the display
void Interpreter::_00E0(void){

    memset(HW.screen, 0, sizeof(HW.screen)); //clear the screen of pixels
    Draw.clearScreenSDL();                   //clear existing image w/SDL
}

// Return from subroutine
void Interpreter::_00EE(void){

    HW.programCounter = HW.stack[HW.stackPtr];
    --HW.stackPtr;
}

// Jump to memory location at NNN
void Interpreter::_1NNN(void){

    HW.programCounter = NNN;
}

// Call subroutine at NNN
void Interpreter::_2NNN(void){

    HW.stack[HW.stackPtr] = HW.programCounter;
    ++HW.stackPtr;
    HW.programCounter = NNN;
}

// Skip next instruction if V[X] == KK
void Interpreter::_3XKK(void){

    if(HW.V[X] == KK) HW.programCounter += 2;
}

// SKip next instruction if V[X] != KK
void Interpreter::_4XKK(void){

    if(HW.V[X] != KK) HW.programCounter += 2;
}

// Skip next instruction if V[X] == V[Y]
void Interpreter::_5XY0(void){

    if(HW.V[X] == HW.V[Y]) HW.programCounter += 2;
}

// Self-explanatory
void Interpreter::_6XKK(void){

    HW.V[X] = KK;
}

void Interpreter::_7XKK(void){

    HW.V[X] += KK;
}

void Interpreter::_8XY0(void){

    HW.V[X] = HW.V[Y];
}

// Bitwise OR on V[X] and V[Y], store result in V[X]
void Interpreter::_8XY1(void){

    HW.V[X] |= HW.V[Y];
}

// Bitwise AND on V[X] and V[Y], store result in V[X]
void Interpreter::_8XY2(void){

    HW.V[X] &= HW.V[Y];
}

// Bitwise XOR on V[X] and V[Y], store result in V[X]
void Interpreter::_8XY3(void){

    HW.V[X] ^= HW.V[Y];
}

/*The values of V[X] and V[Y] are added together. If the result is greater than 8 bits
 * (i.e., > 255,) V[F] is set to 1, otherwise 0.
 * Only the lowest 8 bits of the result are kept, and stored in V[X].*/
void Interpreter::_8XY4(void){

    uint16_t  temp = 0;

    temp = HW.V[X] + HW.V[Y];

    if(temp > 0xFF) HW.V[0xF] = 1;
    else            HW.V[0xF] = 0;

    HW.V[X] = temp >> 8;
}

/*If V[X] > V[Y], then V[F] is set to 1, otherwise 0.
 * Then V[Y] is subtracted from V[X], and the results stored in V[X].*/
void Interpreter::_8XY5(void){

    if(HW.V[X] > HW.V[Y]) HW.V[0xF] = 1;
    else                  HW.V[0xF] = 0;

    HW.V[X] -= HW.V[Y];
}

// If LSB of V[X] is 1, V[F] = 1, then divide V[X] by 2
void Interpreter::_8XY6(void){

    if((HW.V[X] & 0x1) == 1) HW.V[0xF] = 1;
    else                     HW.V[0xF] = 0;

    HW.V[X] >>= 1; // Neat bitwise way of dividing by two
}

// V[X] = V[Y] - V[X], V[F] = NOT Borrow
void Interpreter::_8XY7(void){

    if(HW.V[Y] > HW.V[X]) HW.V[0xF] = 1;
    else                  HW.V[0xF] = 0;

    HW.V[X] = HW.V[Y] - HW.V[X];
}

// If MSB of V[X} = 1, V[F] = 1, then multiply V[X] by 2
void Interpreter::_8XYE(void){

    //maybe take a look at this again and see if desired value is acquired!

    if((HW.V[X] >> 7) == 1 ) HW.V[0xF] = 1;
    else                     HW.V[0xF] = 0;

    HW.V[X] <<= 1;  // Bitwise *2
}

// If V[X] != V[Y], skip next inst.
void Interpreter::_9XY0(void){

    if(HW.V[X] != HW.V[Y]) HW.programCounter += 2;
}

// Self-explanatory
void Interpreter::_ANNN(void){

    HW.indexRegister = NNN;
}

void Interpreter::_BNNN(void){

    HW.programCounter = NNN+HW.V[0];
}

/* Generate random num between 0 and 255, bitwise AND that
   Value with KK, result in V[X]*/
void Interpreter::_CXKK(void){

    uint8_t randomNum = genRandomNum();

    HW.V[X] = (randomNum & KK);
}

/*Display n-byte sprite starting at memory location I at (V[X], V[Y]), V[F] = collision.
The interpreter reads n bytes from memory, starting at the address stored in I.
These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
Sprites are XORed onto the existing screen.
If this causes any pixels to be erased, V[F] is set to 1, otherwise it is set to 0.
If the sprite is positioned so part of it is outside the coordinates of the display,
it wraps around to the opposite side of the screen.*/
void Interpreter::_DXYN(void){

    //maximum sprite size is 8x15, where it is the height that is adjustable
    HW.V[0xF] = 0;
    uint8_t current_pixel = 0;

    for(int idxY = 0; idxY < N; ++idxY){
        current_pixel = HW.memory[HW.indexRegister+idxY];
        for(int idxX = 0; idxX < 8; ++idxX){
            if((current_pixel & (0x80 >> idxX)) != 0){
                //check if pixel already exists at location
                if(HW.screen[HW.V[X]+idxX][HW.V[Y]+idxY] == 1){
                    HW.V[0xF] = 1;
                }
                HW.screen[HW.V[X]+idxX][HW.V[Y]+idxY] ^= 1;
            }
        }
    };
    HW.drawFlag = 1;
}

// Skip next instruction if key with the value of V[X] is pressed
void Interpreter::_EX9E(void){
    if(keystates[HW.V[X]] == 1) HW.programCounter += 2;
}

// Same as above if not pressed
void Interpreter::_EXA1(void){
    if(keystates[HW.V[X]] == 0) HW.programCounter += 2;
}

// Self-explanatory
void Interpreter::_FX07(void){
    HW.V[X] = HW.delayTimer;
}

//Wait for a key press, store the value of the key in V[X]
void Interpreter::_FX0A(void){
    //if buggy, take a look at this again and see if a delay/exec stop can
    //be written using some SDL function instead of rolling PC back while
    //no key has been pressed

    int temp = -1;
    for (int idx=0; idx<16; ++idx) {
        if (keystates[idx]) {
            temp = idx;
            break;
        }
    }
    if (temp != -1) HW.V[X] = temp;
    else HW.programCounter -= 2;
}

// Self-explanatory
void Interpreter::_FX15(void){

    HW.delayTimer = HW.V[X];
}

void Interpreter::_FX18(void){

    HW.soundTimer = HW.V[X];
}

// I = I + V[X]
void Interpreter::_FX1E(void){

    HW.indexRegister += HW.V[X];
}

// I = location of sprite for digit V[X]
void Interpreter::_FX29(void){
    HW.indexRegister = HW.V[X] * 0x5;
}

// Store BCD representation of V[X] in memory locations I, I+1, and I+2
void Interpreter::_FX33(void){
    //Refer wikipedia/BCD 8421
    HW.memory[HW.indexRegister]     =  HW.V[X] / 100;
    HW.memory[HW.indexRegister + 1] = (HW.V[X] / 10) % 10;
    HW.memory[HW.indexRegister + 2] = (HW.V[X] % 100) % 10;
    HW.programCounter += 2;
}

// Store register contents V[0] through V[X] in memory starting at location I
void Interpreter::_FX55(void){
    for(int idx = 0; idx <= X; ++idx){
        HW.memory[HW.indexRegister + idx] = HW.V[idx];
    }
}

// Read register contents V[0] through V[X] from memory starting at location I
void Interpreter::_FX65(void){
    for(int idx = 0; idx <= X; ++idx){
        HW.V[idx] = HW.memory[HW.indexRegister + idx];
    }
}

DebugUtils::DebugUtils(){
    getCurrentTime();
}

DebugUtils::~DebugUtils(){

}

void DebugUtils::logCurrentOpcode(char *data){

    char buffer[20] = "opcode_log";
    strcat(buffer, currentTime);
    char *finalName = strcat(buffer, "_.txt");

    FILE *fp = NULL;

    fp = fopen(finalName, "a+");

    fprintf(fp, data);

    fclose(fp);
}

void DebugUtils::getCurrentTime(void){

    time_t timer;
    struct tm *timeInfo;

    time(&timer);
    timeInfo = localtime(&timer);

    strftime(currentTime, 10, "%H%M%S", timeInfo);
}
