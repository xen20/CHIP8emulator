#ifndef HARDWARE_H
#define HARDWARE_H

#include <cstdint>

#define  NNN (HW->opcode &  0x0FFF)
#define  N   (HW->opcode &  0x000F)
#define  X   (HW->opcode &  0x0F00) >> 8
#define  Y   (HW->opcode &  0x00F0) >> 4
#define  KK  (HW->opcode &  0x00FF)

class Hardware{
public:

    Hardware();
    ~Hardware();

    uint16_t programCounter;
    uint16_t opcode;
    uint8_t  memory[4096];

    //uint8_t  screen[64][32];
    uint8_t  screen[2048];

    uint8_t  V[16];
    uint16_t indexRegister;

    uint8_t  delayTimer;
    uint8_t  soundTimer;

    uint16_t stack[16];
    uint16_t stackPtr;

    uint8_t drawFlag;

private:

    void loadFontset(void);
};
#endif // HARDWARE_H
