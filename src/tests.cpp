#include "tests.h"

using namespace emutest;

#include <iostream>
#include <cstdio>

int emutest::Executor(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

InterpreterTests::InterpreterTests() : _interpreter(&this->_hw)
{
}

InterpreterTests::~InterpreterTests()
{

}


InterpreterTests SetUpTests(void)
{
    InterpreterTests _interpreterTests;

    _interpreterTests._hw.opcode = 0x1234;

    _interpreterTests._hw.V[2] = 0xFF;
    _interpreterTests._hw.V[3] = 0xF;

    return _interpreterTests;
}


TEST(Interpreter, 00E0)
{
    //TODO: See how to perform multiple assertion
}

TEST(Interpreter, 1NNN)
{
    // Arrange
    InterpreterTests _interpreterTests;

    //Act
    _interpreterTests._hw.opcode = 0xFFFF;
    _interpreterTests._interpreter._1NNN();

    //Assert
    EXPECT_EQ(_interpreterTests._hw.programCounter, 0x0FFF);
}

TEST(Interpreter, 2NNN)
{
    InterpreterTests _interpreterTests;

    _interpreterTests._hw.programCounter = 0xFFF;
    _interpreterTests._hw.opcode = 0xFFFF;
    auto sPtr = _interpreterTests._hw.stackPtr = 0;
    _interpreterTests._interpreter._2NNN();

    ASSERT_EQ(_interpreterTests._hw.stack[sPtr], _interpreterTests._hw.programCounter);
    ASSERT_EQ(++sPtr, _interpreterTests._hw.stackPtr);
}

TEST(Interpreter, 3XKK)
{
    InterpreterTests _interpreterTests;

    _interpreterTests._hw.opcode = 0xFFFF;
    _interpreterTests._hw.programCounter = 0;

    _interpreterTests._hw.V[0xF] = 0xFF;
    _interpreterTests._interpreter._3XKK();

    EXPECT_EQ(_interpreterTests._hw.programCounter, 2);
}

TEST(Interpreter, 6XKK)
{
    InterpreterTests _interpreterTests;

    _interpreterTests._hw.opcode = 0xFFFF;
    _interpreterTests._interpreter._6XKK();

    EXPECT_EQ(_interpreterTests._hw.V[0xF], _interpreterTests._hw.opcode & 0x00FF);
}

TEST(Interpreter, 7XKK)
{
    InterpreterTests _interpreterTests;

    _interpreterTests._hw.opcode = 0xFFFF;
    _interpreterTests._interpreter._7XKK();

    EXPECT_EQ(_interpreterTests._hw.V[0xF], _interpreterTests._hw.opcode & 0x00FF);
}

TEST(Interpreter, 8XY0)
{
    InterpreterTests _interpreterTests;

    _interpreterTests._hw.opcode = 0x1234;
    _interpreterTests._hw.V[3] = 0xF;

    _interpreterTests._interpreter._8XY0();

    EXPECT_EQ(_interpreterTests._hw.V[2], _interpreterTests._hw.V[3]);
}

TEST(Interpreter, 8XY1)
{
    auto _interpreterTests = SetUpTests();

    auto _referenceHWX = _interpreterTests._hw.V[2] | _interpreterTests._hw.V[3];
    _interpreterTests._interpreter._8XY1();

    EXPECT_EQ(_referenceHWX, _interpreterTests._hw.V[2]);
}

TEST(Interpreter, 8XY2)
{
    auto _interpreterTests = SetUpTests();

    auto _referenceHWX = _interpreterTests._hw.V[2] & _interpreterTests._hw.V[3];
    _interpreterTests._interpreter._8XY2();

    EXPECT_EQ(_referenceHWX, _interpreterTests._hw.V[2]);
}

TEST(Interpreter, 8XY3)
{
    auto _interpreterTests = SetUpTests();

    auto _referenceHWX = _interpreterTests._hw.V[2] ^ _interpreterTests._hw.V[3];
    _interpreterTests._interpreter._8XY3();

    EXPECT_EQ(_referenceHWX, _interpreterTests._hw.V[2]);
}

TEST(Interpreter, 8XY4)
{
    // Multiple assert. Assert case 1 for now.

    auto _interpreterTests = SetUpTests();

    _interpreterTests._interpreter._8XY4();

    EXPECT_EQ(_interpreterTests._hw.V[0xF], 1);
}

// Next tests are a significant amount of multiple assert...

TEST(Interpreter, ANNN)
{
    InterpreterTests _interpreterTests;

    _interpreterTests._hw.opcode = 0xFFFF;
    _interpreterTests._interpreter._ANNN();

    EXPECT_EQ(_interpreterTests._hw.indexRegister, _interpreterTests._hw.opcode & 0x0FFF);
}

TEST(Interpreter, BNNN)
{
    InterpreterTests _interpreterTests;

    _interpreterTests._hw.opcode = 0xFFFF;
    _interpreterTests._hw.V[0] = 0x1;
    _interpreterTests._interpreter._BNNN();

    EXPECT_EQ(_interpreterTests._hw.programCounter, _interpreterTests._hw.opcode & 0x0FFF + 0x1);
}

// Keystates being external impedes keyboard tests. Include a keyboard object in interpreter?


TEST(Interpreter, FX07)
{
    InterpreterTests _interpreterTests;

    _interpreterTests._hw.delayTimer = 100;
    _interpreterTests._hw.opcode = 0xFFFF;
    _interpreterTests._interpreter._FX07();

    EXPECT_EQ(_interpreterTests._hw.V[0xF], _interpreterTests._hw.delayTimer);
}

TEST(Interpreter, FX15)
{
    InterpreterTests _interpreterTests;

    _interpreterTests._hw.opcode = 0xFFFF;
    _interpreterTests._hw.V[0xF] = 0xF;
    _interpreterTests._interpreter._FX15();

    EXPECT_EQ(_interpreterTests._hw.delayTimer, _interpreterTests._hw.V[0xF]);
}

TEST(Interpreter, FX18)
{
    InterpreterTests _interpreterTests;

    _interpreterTests._hw.opcode = 0xFFFF;
    _interpreterTests._hw.V[0xF] = 0xF;
    _interpreterTests._interpreter._FX18();

    EXPECT_EQ(_interpreterTests._hw.soundTimer, _interpreterTests._hw.V[0xF]);
}


TEST(Interpreter, FX1E)
{
    InterpreterTests _interpreterTests;

    _interpreterTests._hw.opcode = 0xFFFF;
    _interpreterTests._hw.indexRegister = 0xF;
    _interpreterTests._hw.V[0xF] = 0xF;

    auto reference = _interpreterTests._hw.indexRegister + 0xF;
    _interpreterTests._interpreter._FX1E();

    EXPECT_EQ(reference, _interpreterTests._hw.indexRegister);
}

TEST(Interpreter, FX29)
{
    InterpreterTests _interpreterTests;

    _interpreterTests._hw.opcode = 0xFFFF;
    auto testRegister = _interpreterTests._hw.V[0xF] = 0xF;

    _interpreterTests._interpreter._FX29();

    EXPECT_EQ(_interpreterTests._hw.indexRegister, testRegister * 0x5);
}

TEST(Interpreter, FX33)
{
    // mul assert
}

TEST(Interpreter, FX55)
{
    InterpreterTests _interpreterTests;

    const int numberOfElementsInRegisters = 16;

    memset(&_interpreterTests._hw.V, 1, numberOfElementsInRegisters);
    _interpreterTests._hw.opcode = 0xFFF;
    _interpreterTests._hw.indexRegister = 0xFF;
    _interpreterTests._interpreter._FX55();

    for(int i = 0; i < numberOfElementsInRegisters; ++i)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(_interpreterTests._hw.V[i],
                  _interpreterTests._hw.memory[_interpreterTests._hw.indexRegister + i]);
    }

}

TEST(Interpreter, FX65)
{
    InterpreterTests _interpreterTests;

    const int numberOfElementsInRegisters = 16;

    memset(&_interpreterTests._hw.memory, 1, numberOfElementsInRegisters);
    _interpreterTests._hw.opcode = 0xFFF;
    _interpreterTests._hw.indexRegister = 0xFF;
    _interpreterTests._interpreter._FX65();

    for(int i = 0; i < numberOfElementsInRegisters; ++i)
    {
        SCOPED_TRACE(i);
        ASSERT_EQ(_interpreterTests._hw.V[i],
                  _interpreterTests._hw.memory[_interpreterTests._hw.indexRegister + i]);
    }

}
