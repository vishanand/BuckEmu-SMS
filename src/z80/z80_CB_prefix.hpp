#ifndef Z80_CB_PREFIX_H
#define Z80_CB_PREFIX_H

#include "z80.hpp"
#include "z80_macro.hpp"
#include "../sms.hpp"

// execute CB prefix instructions
inline int Z80::prefixCB(){
    int cycles = 23;
    R_inc();

    // fetch rest of instruction
    uint8_t opcode = sms.mem.getByte(PC++);

    // decode and execute rest of instruction
    switch (opcode){

        default:
            cycles = 69;
            break;
    }

    return cycles;
}

#endif