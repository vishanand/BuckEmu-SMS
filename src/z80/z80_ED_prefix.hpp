#ifndef Z80_ED_PREFIX_H
#define Z80_ED_PREFIX_H

#include "z80.hpp"
#include "z80_macro.hpp"
#include "../sms.hpp"

// execute ED prefix instructions
inline int Z80::prefixED(){
    int cycles = 8;
    R_inc();

    // fetch rest of instruction
    uint8_t opcode = sms.mem.getByte(PC++);

    // decode and execute rest of instruction
    switch (opcode){
        // IM 0
        case 0x46:
        case 0x66:
            IM(0);

        // IM 1
        case 0x56:
        case 0x76:
            IM(1);

        // IM 2
        case 0x5E:
        case 0x7E:
            IM(2);

        // LDI
        case 0xA0:
            cycles = 16;
            LDI_LDD(1);
            break;

        // LDD
        case 0xA8:
            cycles = 16;
            LDI_LDD(-1);
            break;

        // LDIR
        case 0xB0:
            cycles = 16;
            LDI_LDD(1);
            if (BC > 0){ // repeat instruction until BC is 0
                PC -= 2;
                cycles = 21;
            }
            break;

        // LDDR
        case 0xB8:
            cycles = 16;
            LDI_LDD(-1);
            if (BC > 0){ // repeat instruction until BC is 0
                PC -= 2;
                cycles = 21;
            }
            break;
        
        default:
            cycles = 69;
            break;
    }

    return cycles;
}

#endif