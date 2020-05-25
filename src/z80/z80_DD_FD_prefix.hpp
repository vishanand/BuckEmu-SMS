#ifndef Z80_xD_PREFIX_H
#define Z80_xD_PREFIX_H

#include "z80.hpp"
#include "z80_macro.hpp"
#include "../sms.hpp"

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
            GET_OFFSET()
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

        // LD b,(ix+*)
        case 0x46:
            LD_r8_IZ_offset(B);

        // LD c,(ix+*)
        case 0x4E:
            LD_r8_IZ_offset(C);

        // LD d,(ix+*)
        case 0x56:
            LD_r8_IZ_offset(D);

        // LD e,(ix+*)
        case 0x5E:
            LD_r8_IZ_offset(E);

        // LD h,(ix+*)
        case 0x66:
            LD_r8_IZ_offset(H);

        // LD l,(ix+*)
        case 0x6E:
            LD_r8_IZ_offset(L);

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

        // LD a,(ix+*)
        case 0x7E:
            LD_r8_IZ_offset(A);

        // DDCB and FDCB prefix instructions
        case 0xCB:
            cycles = prefix_xDCB(IZ, IZ_H, IZ_L);
            break;

        // POP iz
        case 0xE1:
            cycles = 14;
            IZ = pop();
            break;

        // PUSH iz
        case 0xE5:
            cycles = 15;
            push(IZ);
            break;

        // If another prefix, do a NOP
        case 0xED:
        case 0xDD:
        case 0xFD:
            PC--;
            break;

        default:
            cycles = 69;
            break;
    }

    return cycles;
}

#endif