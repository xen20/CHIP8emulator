#ifndef CORE_H
#define CORE_H

#include <cstdint>

#define  NNN (HW.opcode >> 4)
#define  N   (HW.opcode &  0x000F)
#define  X   (HW.opcode &  0x0F00) >> 8
#define  Y   (HW.opcode &  0x00F0) >> 4
#define  KK  (HW.opcode >> 8)

class Hardware{
public:
    Hardware();
    ~Hardware();

    uint16_t programCounter;
    uint16_t opcode;
    uint8_t  memory[4096];

    uint8_t  screen[64][32];

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

class Core{
public:
    Core();
    ~Core();
    void loadROM(char *ROM);
    void initSDL(void);
    void drawSDL(void);
    void closeSDL(void);
    void emulateSystem(void);
    bool isRunning;
private:
    void fetchOpcode(void);
    uint16_t decodeOpcode(void);
    void indexOpcodes(void);
    void interpretOpcode(void);
};

class Interpreter{
public:
    Interpreter();
    ~Interpreter();
    void _00E0(void);
    void _00EE(void);
    void _1NNN(void);
    void _2NNN(void);

    void _3XKK(void);
    void _4XKK(void);
    void _5XY0(void);
    void _6XKK(void);

    void _7XKK(void);
    void _8XY0(void);
    void _8XY1(void);
    void _8XY2(void);

    void _8XY3(void);
    void _8XY4(void);
    void _8XY5(void);
    void _8XY6(void);

    void _8XY7(void);
    void _8XYE(void);
    void _9XY0(void);
    void _ANNN(void);

    void _BNNN(void);
    void _CXKK(void);
    void _DXYN(void);
    void _EX9E(void);

    void _EXA1(void);
    void _FX07(void);
    void _FX0A(void);
    void _FX15(void);

    void _FX18(void);
    void _FX1E(void);
    void _FX29(void);
    void _FX33(void);

    void _FX55(void);
    void _FX65(void);
};

#endif // CORE_H
