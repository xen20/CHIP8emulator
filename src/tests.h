#ifndef TESTS_H
#define TESTS_H

#include <gtest/gtest.h>

#include "hardware.h"
#include "interpreter.h"

namespace emutest
{
    int Executor(int argc, char **argv);
    void Initialize();

    class InterpreterTests
    {
        public:
            InterpreterTests();
            virtual ~InterpreterTests();
            Hardware _hw;
            Interpreter _interpreter;
    };

    class HardwareTests
    {
        public:
            HardwareTests();
            virtual ~HardwareTests();
        private:

    };
}

#endif // TESTS_H
