#include "z80.hpp"

Z80::Z80(){
    // default register values
    A = 0xFF;
    F = 0xFF;
    PC = 0;
    SP = 0xFFFF;
    IFF1 = false;
    IFF2 = false;
}

