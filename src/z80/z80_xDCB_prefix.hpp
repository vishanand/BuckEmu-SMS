#ifndef Z80_xDCB_PREFIX_H
#define Z80_xDCB_PREFIX_H

#include "z80.hpp"
#include "z80_macro.hpp"
#include "../sms.hpp"

// execute DDCB and FDCB prefix instructions
// IZ is a reference to either IX or IY
inline int Z80::prefix_xDCB(uint16_t &IZ, uint8_t &IZ_H, uint8_t &IZ_L){
    int cycles = 23;
    R_inc();

    // fetch mandatory displacement byte
    uint8_t displacement = sms.mem.getByte(PC++);

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