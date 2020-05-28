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

        // IN b,(c)
        case 0x40:
            IN_r8_C(B);

        // OUT (c),b
        case 0x41:
            OUT_C_r8(B);

        // SBC hl,bc
        case 0x42:
            cycles = 15;
            SBC_R16(HL, BC);
            break;

        // LD (**),bc
        case 0x43:
            cycles = 20;
            LD_mem_r16(B,C);

        // LD i,a
        case 0x47:
            cycles = 9;
            I = A;
            break;

        // IN c,(c)
        case 0x48:
            IN_r8_C(C);

        // OUT (c),h
        case 0x49:
            OUT_C_r8(C);

        // ADC hl,bc
        case 0x4A:
            cycles = 15;
            ADC_R16(HL, BC);
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

        // IN d,(c)
        case 0x50:
            IN_r8_C(D);

        // OUT (c),d
        case 0x51:
            OUT_C_r8(D);
            
        // SBC hl,de
        case 0x52:
            cycles = 15;
            SBC_R16(HL, DE);
            break;
            
        // LD (**),de
        case 0x53:
            cycles = 20;
            LD_mem_r16(D,E);

        // LD a,i
        case 0x57:
            cycles = 9;
            A = I;
            if (IFF2)   SetBit(F, PVF);
            else    ClearBit(F, PVF);
            break;

        // IN e,(c)
        case 0x58:
            IN_r8_C(E);

        // OUT (c),e
        case 0x59:
            OUT_C_r8(E);

        // ADC hl,de
        case 0x5A:
            cycles = 15;
            ADC_R16(HL, DE);
            break;

        // LD de,(**)
        case 0x5B:
            cycles = 20;
            LD_r16_mem(D,E);

        // LD a,r
        case 0x5F:
            cycles = 9;
            A = R;
            if (IFF2)   SetBit(F, PVF);
            else    ClearBit(F, PVF);
            break;

        // IN h,(c)
        case 0x60:
            IN_r8_C(H);

        // OUT (c),h
        case 0x61:
            OUT_C_r8(H);

        // SBC hl,hl
        case 0x62:
            cycles = 15;
            SBC_R16(HL, HL);
            break;
            
        // LD (**),hl
        case 0x63:
            cycles = 20;
            LD_mem_r16(H,L);

        // RRD
        case 0x67:{
            cycles = 18;
            uint8_t tempA = A;
            uint8_t tempHL = sms.mem.getByte(HL);

            A = (A & 0xF0) | (tempHL & 0x0F);
            tempHL = (tempHL >> 4) | (tempA << 4);
            sms.mem.setByte(HL, tempHL);

            ClearBit(F, HF);
            ClearBit(F, NF);
            PARITY_FLAG(A);
            SZ_FLAGS(A);
            break;
        }

        // IN l,(c)
        case 0x68:
            IN_r8_C(L);

        // OUT (c),l
        case 0x69:
            OUT_C_r8(L);

        // ADC hl,hl
        case 0x6A:
            cycles = 15;
            ADC_R16(HL, HL);
            break;

        // LD hl,(**)
        case 0x6B:
            cycles = 20;
            LD_r16_mem(H,L);

        // RLD
        case 0x6F:{
            cycles = 18;
            uint8_t tempA = A;
            uint8_t tempHL = sms.mem.getByte(HL);

            A = (A & 0xF0) | (tempHL >> 4); 
            tempHL = (tempHL << 4) | (tempA & 0x0F);
            sms.mem.setByte(HL, tempHL);

            ClearBit(F, HF);
            ClearBit(F, NF);
            PARITY_FLAG(A);
            SZ_FLAGS(A);
            break;
        }

        // IN (c)
        case 0x70:{
            uint8_t tmpByte = 0;
            IN_r8_C(tmpByte);
        }

        // OUT (c),0
        case 0x71:{
            uint8_t tmpByte = 0;
            OUT_C_r8(tmpByte);
        }

        // SBC hl,sp
        case 0x72:
            cycles = 15;
            SBC_R16(HL, SP);
            break;

        // LD (**),sp
        case 0x73:
            cycles = 20;
            LD_mem_r16(SP_H, SP_L);

        // IN a,(c)
        case 0x78:
            IN_r8_C(A);

        // OUT (c),a
        case 0x79:
            OUT_C_r8(A);

        // ADC hl,sp
        case 0x7A:
            cycles = 15;
            ADC_R16(HL, SP);
            break;

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

        // RETI/RETN
        case 0x45: case 0x55: case 0x65: case 0x75:
        case 0x4D: case 0x5D: case 0x6D: case 0x7D:
            cycles = 14;
            IFF1 = IFF2;
            PC = pop();
            break;

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

        // INI
        case 0xA2:
            INI_IND(1);
            break;

        // OUTI
        case 0xA3:
            OUTI_OUTD(1);
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

        // IND
        case 0xAA:
            INI_IND(-1);
            break;


        // OUTD
        case 0xAB:
            OUTI_OUTD(-1);
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

        // INIR
        case 0xB2:
            INI_IND(1);
            // repeat until B == 0
            if (B != 0){
                PC -= 2;
                cycles = 21;
            }
            break;

        // OTIR
        case 0xB3:
            OUTI_OUTD(1);
            // repeat until B == 0
            if (B != 0){
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

        // INDR
        case 0xBA:
            INI_IND(-1);
            // repeat until B == 0
            if (B != 0){
                PC -= 2;
                cycles = 21;
            }
            break;

        // OTDR
        case 0xBB:
            OUTI_OUTD(-1);
            // repeat until B == 0
            if (B != 0){
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