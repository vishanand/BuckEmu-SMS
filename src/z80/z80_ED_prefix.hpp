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

        // LD (**),bc
        case 0x43:
            cycles = 20;
            LD_mem_r16(B,C);

        // LD i,a
        case 0x47:
            cycles = 9;
            I = A;
            break;

        // LD bc,(**)
        case 0x4B:
            cycles = 20;
            LD_r16_mem(B,C);

        // LD r,a
        case 0x4F:
            cycles = 9;
            R = A;
            break;

        // LD (**),de
        case 0x53:
            cycles = 20;
            LD_mem_r16(D,E);

        // LD a,i
        case 0x57:
            cycles = 9;
            A = I;
            break;

        // LD de,(**)
        case 0x5B:
            cycles = 20;
            LD_r16_mem(D,E);

        // LD a,r
        case 0x5F:
            cycles = 9;
            A = R;
            break;

        // LD (**),hl
        case 0x63:
            cycles = 20;
            LD_mem_r16(H,L);

        // LD hl,(**)
        case 0x6B:
            cycles = 20;
            LD_r16_mem(H,L);

        // LD (**),sp
        case 0x73:
            cycles = 20;
            LD_mem_r16(SP_H, SP_L);

        // LD sp,(**)
        case 0x7B:
            cycles = 20;
            LD_r16_mem(SP_H, SP_L);

        // NEG
        case 0x44: case 0x54: case 0x64: case 0x74:
        case 0x4C: case 0x5C: case 0x6C: case 0x7C:{
            uint8_t tmpByte = 0;
            SUB_FLAGS(tmpByte, A);
            A = 0 - A;
            break;
        }

        // IM 0
        case 0x46: case 0x66:
            IM(0);

        // IM 1, IM 0/1
        case 0x56: case 0x76: case 0x4E: case 0x6E:
            IM(1);

        // IM 2
        case 0x5E: case 0x7E:
            IM(2);

        // LDI
        case 0xA0:
            cycles = 16;
            LDI_LDD(1);
            break;

        // CPI
        case 0xA1:
            CPI_CPD();
            HL++;
            break;

        // LDD
        case 0xA8:
            cycles = 16;
            LDI_LDD(-1);
            break;

        // CPD
        case 0xA9:
            CPI_CPD();
            HL--;
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

        // CPIR
        case 0xB1:
            CPI_CPD();
            HL++;
            // repeat unless BC == 0 or (hl) == 0
            if (BC != 0 && !CheckBit(F, ZF)){
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

        // CPDR
        case 0xB9:
            CPI_CPD();
            HL--;
            // repeat unless BC == 0 or (hl) == 0
            if (BC != 0 && !CheckBit(F, ZF)){
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