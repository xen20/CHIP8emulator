This is a simple CHIP8 emulator written in a mix of C++ (structure, some "advanced" functions e.g maps) and C. SDL2 is required for drawing, and googletest (installed automatically when building project) is used for unit testing.

Features:

- 99% (likely 100%) compatibility with games - not sure just because I have not tested them all :) 
- Live debug features, e.g printing current opcode, program counter, register contents.

Demo:

![Alt Text](https://i.imgur.com/PtIYX8L.gif)


To-do:

- Add proper sound output (the current SDL2 implementation is blocking).
- Cover all opcodes with tests, make building test suite optional.
- A little bit of refactoring to better conform to OOP design principles.
