#ifndef Z80_CB_PREFIX_H
#define Z80_CB_PREFIX_H

#include "z80.hpp"
#include "z80_macro.hpp"
#include "../sms.hpp"

// execute CB prefix instructions
inline int Z80::prefixCB(){
    int cycles = 8;
    R_inc();

    // fetch rest of instruction
    uint8_t opcode = sms.mem.getByte(PC++);

    // Decoding strategy based on: http://z80.info/decoding.htm#cb 

    // get first two bits
    uint8_t x_bits = (opcode & 0xC0) >> 6;
    
    // get next three bits
    uint8_t y_bits = (opcode & 0x38) >> 3;

    // get final three bits
    uint8_t z_bits = opcode & 0x07;

    // load in the register that we're using
    uint8_t currReg;
    switch (z_bits){
        case 0x0:
            currReg = B; break;
        case 0x1:
            currReg = C; break;
        case 0x2:
            currReg = D; break;
        case 0x3:
            currReg = E; break;
        case 0x4:
            currReg = H; break;
        case 0x5:
            currReg = L; break;
        case 0x6:
            currReg = sms.mem.getByte(HL);
            cycles = 15;
            break;
        case 0x7:
            currReg = A; break;
    }

    // shift/rotate operations
    if (x_bits == 0){
        
        // check which rotation to perform
        switch (y_bits){
            
            // RLC
            case 0x0:
                RLC_shift(currReg);
                break;

            // RRC
            case 0x1:
                RRC_shift(currReg);
                break;

            // RL
            case 0x2:
                RL_shift(currReg);
                break;
            
            // RR
            case 0x3:
                RR_shift(currReg);
                break;

            // SLA
            case 0x4:
                SLA_shift(currReg);
                break;
            
            // SRA
            case 0x5:
                SRA_shift(currReg);
                break;

            // SLL
            case 0x6:
                SLL_shift(currReg);
                break;

            // SRL
            case 0x7:
                SRL_shift(currReg);
                break;
        }
    }

    // BIT operations
    else if (x_bits == 1){
        if (z_bits == 0x6){
            cycles = 12;
        }
        BIT_INST(y_bits, currReg);
    }

    // RES operations
    else if (x_bits == 2){
        RES_INST(y_bits, currReg);
    }

    // SET operations
    else if (x_bits == 3){
        SET_INST(y_bits, currReg);
    }

    // save back to the register that we're using
    switch (z_bits){
        case 0x0:
            B = currReg; break;
        case 0x1:
            C = currReg; break;
        case 0x2:
            D = currReg; break;
        case 0x3:
            E = currReg; break;
        case 0x4:
            H = currReg; break;
        case 0x5:
            L = currReg; break;
        case 0x6:
            sms.mem.setByte(HL, currReg);
            break;
        case 0x7:
            A = currReg; break;
    }
    
    return cycles;
}

#endif