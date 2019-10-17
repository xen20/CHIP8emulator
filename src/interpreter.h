#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "hardware.h"

class Interpreter{
public:

    Interpreter(Hardware *mainHardware);
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

private:
    Hardware *HW;
};

#endif // INTERPRETER_H
