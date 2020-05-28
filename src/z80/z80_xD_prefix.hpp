#ifndef Z80_xD_PREFIX_H
#define Z80_xD_PREFIX_H

#include "z80.hpp"
#include "z80_macro.hpp"
#include "../sms.hpp"
#include <cstdio>

// execute DD and FD prefix instructions
// IZ is a reference to either IX or IY
inline int Z80::prefix_xD(uint16_t &IZ, uint8_t &IZ_H, uint8_t &IZ_L){
    int cycles = 8;
    R_inc();

    // fetch rest of instruction
    uint8_t opcode = sms.mem.getByte(PC++);

    // decode and execute rest of instruction
    switch (opcode){

        // ADD iz,bc
        case 0x09:
            cycles = 15;
            ADD_R16(IZ,BC);
            break;

        // ADD iz,de
        case 0x19:
            cycles = 15;
            ADD_R16(IZ,DE);
            break;

        // LD iz,**
        case 0x21:
            cycles = 14;
            IZ = fetch16();
            break;

        // LD (**),iz
        case 0x22:
            cycles = 20;
            LD_mem_r16(IZ_H, IZ_L);

        // INC iz
        case 0x23:
            cycles = 10;
            IZ++;
            break;

        // INC izh
        case 0x24:
            cycles = 8;
            INC_R8(IZ_H);
            break;

        // DEC izh
        case 0x25:
            cycles = 8;
            DEC_R8(IZ_H);
            break;

        // LD izh,*
        case 0x26:
            cycles = 11;
            IZ_H = sms.mem.getByte(PC++);
            break;

        // ADD iz,iz
        case 0x29:
            cycles = 15;
            ADD_R16(IZ,IZ);
            break;

        // LD iz,(**)
        case 0x2A:
            cycles = 20;
            LD_r16_mem(IZ_H, IZ_L);

        // DEC iz
        case 0x2B:
            cycles = 10;
            IZ--;
            break;

        // INC izl
        case 0x2C:
            INC_R8(IZ_L);
            break;

        // DEC izl
        case 0x2D:
            DEC_R8(IZ_L);
            break;

        // LD izl,*
        case 0x2E:
            cycles = 11;
            IZ_L = sms.mem.getByte(PC++);
            break;

        // INC (iz+*)
        case 0x34:{
            cycles = 23;
            GET_OFFSET();
            uint8_t tmp = sms.mem.getByte(IZ + offset);
            INC_R8(tmp);
            sms.mem.setByte(IZ + offset, tmp);
            break;
        }

        // DEC (iz+*)
        case 0x35:{
            cycles = 23;
            GET_OFFSET();
            uint8_t tmp = sms.mem.getByte(IZ + offset);
            DEC_R8(tmp);
            sms.mem.setByte(IZ + offset, tmp);
            break;
        }

        // LD (iz+*),*
        case 0x36: {
            cycles = 19;
            GET_OFFSET();
            uint8_t tmpByte = sms.mem.getByte(PC++);
            sms.mem.setByte(IZ + offset, tmpByte);
            break;
        }

        // ADD iz,sp
        case 0x39:
            cycles = 15;
            ADD_R16(IZ,SP);
            break;

        // LD b,izh
        case 0x44:
            LD_r8_r8(B, IZ_H);

        // LD b,izl
        case 0x45:
            LD_r8_r8(B, IZ_L);

        // LD b,(ix+*)
        case 0x46:
            LD_r8_IZ_offset(B);

        // LD c,izh
        case 0x4C:
            LD_r8_r8(C, IZ_H);

        // LD c,izl
        case 0x4D:
            LD_r8_r8(C, IZ_L);

        // LD c,(ix+*)
        case 0x4E:
            LD_r8_IZ_offset(C);

        // LD d,izh
        case 0x54:
            LD_r8_r8(D, IZ_H);

        // LD d,izl
        case 0x55:
            LD_r8_r8(D, IZ_L);

        // LD d,(ix+*)
        case 0x56:
            LD_r8_IZ_offset(D);

        // LD e,izh
        case 0x5C:
            LD_r8_r8(E, IZ_H);

        // LD e,izl
        case 0x5D:
            LD_r8_r8(E, IZ_L);

        // LD e,(ix+*)
        case 0x5E:
            LD_r8_IZ_offset(E);

        // LD izh,b
        case 0x60:
            LD_r8_r8(IZ_H, B);

        // LD izh,c
        case 0x61:
            LD_r8_r8(IZ_H, C);

        // LD izh,d
        case 0x62:
            LD_r8_r8(IZ_H, D);

        // LD izh,e
        case 0x63:
            LD_r8_r8(IZ_H, E);

        // LD izh,izh
        case 0x64:
            LD_r8_r8(IZ_H, IZ_H);

        // LD izh,izl
        case 0x65:
            LD_r8_r8(IZ_H, IZ_L);

        // LD h,(ix+*)
        case 0x66:
            LD_r8_IZ_offset(H);

        // LD izh,a
        case 0x67:
            LD_r8_r8(IZ_H, A);

        // LD izl,b
        case 0x68:
            LD_r8_r8(IZ_L, B);

        // LD izl,c
        case 0x69:
            LD_r8_r8(IZ_L, C);

        // LD izl,d
        case 0x6A:
            LD_r8_r8(IZ_L, D);

        // LD izl,e
        case 0x6B:
            LD_r8_r8(IZ_L, E);

        // LD izl,izh
        case 0x6C:
            LD_r8_r8(IZ_L, IZ_H);

        // LD izl,izl
        case 0x6D:
            LD_r8_r8(IZ_L, IZ_L);

        // LD l,(ix+*)
        case 0x6E:
            LD_r8_IZ_offset(L);

        // LD izl,a
        case 0x6F:
            LD_r8_r8(IZ_L, A);

        // LD (iz+*),b
        case 0x70:
            LD_IZ_offset_r8(B);

        // LD (iz+*),c
        case 0x71:
            LD_IZ_offset_r8(C);

        // LD (iz+*),d
        case 0x72:
            LD_IZ_offset_r8(D);

        // LD (iz+*),e
        case 0x73:
            LD_IZ_offset_r8(E);

        // LD (iz+*),h
        case 0x74:
            LD_IZ_offset_r8(H);

        // LD (iz+*),l
        case 0x75:
            LD_IZ_offset_r8(L);

        // LD (iz+*),a
        case 0x77:
            LD_IZ_offset_r8(A);

        // LD a,izh
        case 0x7C:
            LD_r8_r8(A, IZ_H);

        // LD a,izl
        case 0x7D:
            LD_r8_r8(A, IZ_L);

        // LD a,(ix+*)
        case 0x7E:
            LD_r8_IZ_offset(A);

        // ADD a,izh
        case 0x84:
            ADD_A_R8(IZ_H);
            break;

        // ADD a,izl
        case 0x85:
            ADD_A_R8(IZ_L);
            break;

        // ADD (iz+*)
        case 0x86:
            DO_INDX_OP(ADD_A_R8);

        // ADC a,izh
        case 0x8C:
            ADC_A_R8(IZ_H);
            break;

        // ADC a,izl
        case 0x8D:
            ADC_A_R8(IZ_L);
            break;

        // ADC a,(iz+*)
        case 0x8E:
            DO_INDX_OP(ADC_A_R8);
            break;

        // SUB izh
        case 0x94:
            SUB_INST(IZ_H);
            break;

        // SUB izl
        case 0x95:
            SUB_INST(IZ_L);
            break;

        // SUB (iz+*)
        case 0x96:
            DO_INDX_OP(SUB_INST);

        // SBC a,izh
        case 0x9C:
            SBC_A_R8(IZ_H);
            break;

        // SBC a,izl
        case 0x9D:
            SBC_A_R8(IZ_L);
            break;

        // SBC a,(iz+*)
        case 0x9E:
            DO_INDX_OP(SBC_A_R8);
            break;

        // AND izh
        case 0xA4:
            AND_R8(IZ_H);

        // AND izl
        case 0xA5:
            AND_R8(IZ_L);

        // AND (iz+*)
        case 0xA6:
            DO_INDX_OP(AND_R8);

        // XOR izh
        case 0xAC:
            XOR_R8(IZ_H);

        // XOR izl
        case 0xAD:
            XOR_R8(IZ_L);

        // XOR (iz+*)
        case 0xAE:
            DO_INDX_OP(XOR_R8);

        // OR izh
        case 0xB4:
            OR_R8(IZ_H);
            break;

        // OR izl
        case 0xB5:
            OR_R8(IZ_L);
            break;

        // OR (iz+*)
        case 0xB6:
            DO_INDX_OP(OR_R8);

        // CP izh
        case 0xBC:
            CP_INST(IZ_H);

        // CP izl
        case 0xBD:
            CP_INST(IZ_L);

        // CP (iz+*)
        case 0xBE:
            DO_INDX_OP(CP_INST);

        // DDCB and FDCB prefix instructions
        case 0xCB:
            cycles = prefix_xDCB(IZ, IZ_H, IZ_L);
            break;

        // POP iz
        case 0xE1:
            cycles = 14;
            IZ = pop();
            break;

        // EX (sp),iz
        case 0xE3:{
            cycles = 23;
            uint8_t tmpSwap = sms.mem.getByte(SP);
            sms.mem.setByte(SP, IZ_L);
            IZ_L = tmpSwap;
            tmpSwap = sms.mem.getByte(SP+1);
            sms.mem.setByte(SP+1, IZ_H);
            IZ_H = tmpSwap;
            break;
        }

        // PUSH iz
        case 0xE5:
            cycles = 15;
            push(IZ);
            break;

        // JP (iz)
        case 0xE9:
            PC = sms.mem.getByte(IZ);
            break;

        // ld sp,iz
        case 0xF9:
            cycles = 10;
            SP = IZ;
            break;

        // If another prefix, do a NOP
        case 0xED:
        case 0xDD:
        case 0xFD:
        default:
            PC--;
            break;
    }

    return cycles;
}

#endif