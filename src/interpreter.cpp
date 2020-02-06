#include <cstring>

#include "interpreter.h"
#include "random.h"

uint8_t keystates[16];

Interpreter::Interpreter(Hardware *mainHardware)
{
    HW = mainHardware;
}

Interpreter::~Interpreter()
{

}

// Full documentation available here as of 09.08.2019 :
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.0

// Clear the display
void Interpreter::_00E0(void)
{
    memset(HW->screen, 0, sizeof(HW->screen)); //clear the screen of pixels
}

// Return from subroutine
void Interpreter::_00EE(void)
{
    HW->programCounter = HW->stack[--HW->stackPtr];
}

// Jump to memory location at NNN
void Interpreter::_1NNN(void)
{
    HW->programCounter = NNN;
}

// Call subroutine at NNN
void Interpreter::_2NNN(void)
{
    HW->stack[HW->stackPtr] = HW->programCounter;
    ++HW->stackPtr;
    HW->programCounter = NNN;
}

// Skip next instruction if V[X] == KK
void Interpreter::_3XKK(void)
{
    if (HW->V[X] == KK)
        HW->programCounter += 2;
}

// SKip next instruction if V[X] != KK
void Interpreter::_4XKK(void)
{
    if (HW->V[X] != KK)
        HW->programCounter += 2;
}

// Skip next instruction if V[X] == V[Y]
void Interpreter::_5XY0(void)
{
    if (HW->V[X] == HW->V[Y])
        HW->programCounter += 2;
}

// Self-explanatory
void Interpreter::_6XKK(void)
{
    HW->V[X] = KK;
}

void Interpreter::_7XKK(void)
{
    HW->V[X] += KK;
}

void Interpreter::_8XY0(void)
{
    HW->V[X] = HW->V[Y];
}

// Bitwise OR on V[X] and V[Y], store result in V[X]
void Interpreter::_8XY1(void)
{
    HW->V[X] |= HW->V[Y];
}

// Bitwise AND on V[X] and V[Y], store result in V[X]
void Interpreter::_8XY2(void)
{
    HW->V[X] &= HW->V[Y];
}

// Bitwise XOR on V[X] and V[Y], store result in V[X]
void Interpreter::_8XY3(void)
{
    HW->V[X] ^= HW->V[Y];
}

/*The values of V[X] and V[Y] are added together. If the result is greater than 8 bits
 * (i.e., > 255,) V[F] is set to 1, otherwise 0.
 * Only the lowest 8 bits of the result are kept, and stored in V[X].*/
void Interpreter::_8XY4(void)
{
    uint16_t  temp = 0;

    temp = HW->V[X] + HW->V[Y];

    if (temp > 0xFF)
        HW->V[0xF] = 1;
    else
        HW->V[0xF] = 0;

    HW->V[X] = temp >> 8;
}

/*If V[X] > V[Y], then V[F] is set to 1, otherwise 0.
 * Then V[Y] is subtracted from V[X], and the results stored in V[X].*/
void Interpreter::_8XY5(void)
{
    if (HW->V[X] > HW->V[Y])
        HW->V[0xF] = 1;
    else
        HW->V[0xF] = 0;

    HW->V[X] -= HW->V[Y];
}

// If LSB of V[X] is 1, V[F] = 1, then divide V[X] by 2
void Interpreter::_8XY6(void)
{
    if ((HW->V[X] & 0x1) == 1)
        HW->V[0xF] = 1;
    else
        HW->V[0xF] = 0;

    HW->V[X] >>= 1; // Neat bitwise way of dividing by two
}

// V[X] = V[Y] - V[X], V[F] = NOT Borrow
void Interpreter::_8XY7(void)
{
    if (HW->V[Y] > HW->V[X])
        HW->V[0xF] = 1;
    else
        HW->V[0xF] = 0;

    HW->V[X] = HW->V[Y] - HW->V[X];
}

// If MSB of V[X} = 1, V[F] = 1, then multiply V[X] by 2
void Interpreter::_8XYE(void)
{
    //maybe take a look at this again and see if desired value is acquired!

    if ((HW->V[X] >> 7) == 1)
        HW->V[0xF] = 1;
    else
        HW->V[0xF] = 0;

    HW->V[X] <<= 1;  // Bitwise *2
}

// If V[X] != V[Y], skip next inst.
void Interpreter::_9XY0(void)
{
    if (HW->V[X] != HW->V[Y])
        HW->programCounter += 2;
}

// Self-explanatory
void Interpreter::_ANNN(void)
{
    HW->indexRegister = NNN;
}

void Interpreter::_BNNN(void)
{
    HW->programCounter = NNN + HW->V[0];
}

/* Generate random num between 0 and 255, bitwise AND that
   Value with KK, result in V[X]*/
void Interpreter::_CXKK(void)
{
    uint8_t randomNum = genRandomNum();

    HW->V[X] = (randomNum & KK);
}

/*Display n-byte sprite starting at memory location I at (V[X], V[Y]), V[F] = collision.
The interpreter reads n bytes from memory, starting at the address stored in I.
These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
Sprites are XORed onto the existing screen.
If this causes any pixels to be erased, V[F] is set to 1, otherwise it is set to 0.
If the sprite is positioned so part of it is outside the coordinates of the display,
it wraps around to the opposite side of the screen.*/
void Interpreter::_DXYN(void)
{
    uint16_t x = HW->V[X];
    uint16_t y = HW->V[Y];
    uint16_t height = N;
    uint16_t pixel;

    HW->V[0xF] = 0;

    for (int yline = 0; yline < height; yline++)
    {
        pixel = HW->memory[HW->indexRegister + yline];

        for (int xline = 0; xline < 8; xline++)
        {
            if ((pixel & (0x80 >> xline)) != 0)
            {
                if (HW->screen[(x + xline + ((y + yline) * 64))] == 1)
                {
                    HW->V[0xF] = 1;
                }

                HW->screen[x + xline + ((y + yline) * 64)] ^= 1;
            }
        }
    }

    HW->drawFlag = 1;
}

// Skip next instruction if key with the value of V[X] is pressed
void Interpreter::_EX9E(void)
{
    if (keystates[HW->V[X]] == 1)
        HW->programCounter += 2;
}

// Same as above if not pressed
void Interpreter::_EXA1(void)
{
    if (keystates[HW->V[X]] == 0)
        HW->programCounter += 2;
}

// Self-explanatory
void Interpreter::_FX07(void)
{
    HW->V[X] = HW->delayTimer;
}

//Wait for a key press, store the value of the key in V[X]
void Interpreter::_FX0A(void)
{
    //if buggy, take a look at this again and see if a delay/exec stop can
    //be written using some SDL function instead of rolling PC back while
    //no key has been pressed

    int temp = -1;

    for (int idx = 0; idx < 16; ++idx)
    {
        if (keystates[idx])
        {
            temp = idx;
            break;
        }
    }

    if (temp != -1)
        HW->V[X] = temp;
    else
        HW->programCounter -= 2;
}

// Self-explanatory
void Interpreter::_FX15(void)
{
    HW->delayTimer = HW->V[X];
}

void Interpreter::_FX18(void)
{
    HW->soundTimer = HW->V[X];
}

// I = I + V[X]
void Interpreter::_FX1E(void)
{
    HW->indexRegister += HW->V[X];
}

// I = location of sprite for digit V[X]
void Interpreter::_FX29(void)
{
    HW->indexRegister = HW->V[X] * 0x5;
}

// Store BCD representation of V[X] in memory locations I, I+1, and I+2
void Interpreter::_FX33(void)
{
    //Refer wikipedia/BCD 8421
    HW->memory[HW->indexRegister]     =  HW->V[X] / 100;
    HW->memory[HW->indexRegister + 1] = (HW->V[X] / 10) % 10;
    HW->memory[HW->indexRegister + 2] = (HW->V[X] % 100) % 10;
    HW->programCounter += 2;
}

// Store register contents V[0] through V[X] in memory starting at location I
void Interpreter::_FX55(void)
{
    for (int idx = 0; idx <= X; ++idx)
        HW->memory[HW->indexRegister + idx] = HW->V[idx];
}

// Read register contents V[0] through V[X] from memory starting at location I
void Interpreter::_FX65(void)
{
    for (int idx = 0; idx <= X; ++idx)
        HW->V[idx] = HW->memory[HW->indexRegister + idx];
}
