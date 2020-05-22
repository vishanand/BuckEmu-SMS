#ifndef Z80_NO_PREFIX_H
#define Z80_NO_PREFIX_H

#include "z80.hpp"
#include "z80_macro.hpp"
#include "../sms.hpp"

// execute no prefix instructions
int Z80::runInstruction(){
    int cycles = 4; // default amount of cycles
    R_inc(); // increment refresh register each fetch

    // fetch instruction
    uint8_t opcode = sms.mem.getByte(PC++);

    // decode and execute instruction
    switch (opcode){
        
        // NOP: no operation
        case 0x00:
            break;

        // LD bc,**
        case 0x01:
            LD_r16_imm(BC);

        // LD (bc),a
        case 0x02:
            cycles = 7;
            sms.mem.setByte(BC, A);
            break;

        // INC bc
        case 0x03:
            INC_R16(BC);

        // LD b,*
        case 0x06:
            LD_r8_imm(B);

        // EX af,af'
        case 0x08:
            swap16(AF, AF_s);
            break;

        // LD a,(bc)
        case 0x0A:
            cycles = 7;
            A = sms.mem.getByte(BC);
            break;

        // DEC bc
        case 0x0B:
            DEC_R16(BC);
        
        // LD c,*
        case 0x0E:
            LD_r8_imm(C);

        // DJNZ
        case 0x10:
            B--;
            JR_CC(B != 0);
            cycles++;
            break;

        // LD de,**
        case 0x11:
            LD_r16_imm(DE);

        // LD (de),a
        case 0x12:
            cycles = 7;
            sms.mem.setByte(DE, A);
            break;

        // INC de
        case 0x13:
            INC_R16(DE);

        // LD d,*
        case 0x16:
            LD_r8_imm(D);

        // JR *
        case 0x18:
            JR_CC(true);
            break;

        // LD a,(de)
        case 0x1A:
            cycles = 7;
            A = sms.mem.getByte(DE);
            break;

        // DEC de
        case 0x1B:
            DEC_R16(DE);

        // LD e,*
        case 0x1E:
            LD_r8_imm(E);

        // JR nz,*
        case 0x20:
            JR_CC(nz_CC);
            break;

        // LD hl,**
        case 0x21:
            LD_r16_imm(HL);

        // LD (**),hl
        case 0x22:{
            cycles = 16;
            uint16_t addr = fetch16();
            sms.mem.setByte(addr, L);
            sms.mem.setByte(addr + 1, H);
            break;
        }

        // INC hl
        case 0x23:
            INC_R16(HL);

        // LD h,*
        case 0x26:
            LD_r8_imm(H);

        // JR z,*
        case 0x28:
            JR_CC(z_CC);
            break;

        // LD hl,(**)
        case 0x2A:{
            cycles = 16;
            uint16_t addr = fetch16();
            L = sms.mem.getByte(addr);
            H = sms.mem.getByte(addr + 1);
            break; 
        }

        // DEC hl
        case 0x2B:
            DEC_R16(HL);

        // LD l,*
        case 0x2E:
            LD_r8_imm(L);

        // JR nc,*
        case 0x30:
            JR_CC(nc_CC);
            break;

        // LD sp,**
        case 0x31:
            LD_r16_imm(SP);

        // LD (**),a
        case 0x32:
            cycles = 13;
            sms.mem.setByte(fetch16(), A);
            break;

        // INC sp
        case 0x33:
            INC_R16(SP);

        // LD (hl),*
        case 0x36: {
            cycles = 10;
            uint8_t tmpByte = sms.mem.getByte(PC++);
            sms.mem.setByte(HL, tmpByte);
            break;
        }

        // JR c,*
        case 0x38:
            JR_CC(c_CC);
            break;

        // LD a,(**)
        case 0x3A:
            cycles = 13;
            A = sms.mem.getByte(fetch16());   
            break;

        // DEC sp
        case 0x3B:
            DEC_R16(SP);

        // LD a,*
        case 0x3E:
            LD_r8_imm(A);

        // LD b,b
        case 0x40:
            LD_r8_r8(B,B);

        // LD b,c
        case 0x41:
            LD_r8_r8(B,C);

        // LD b,d
        case 0x42:
            LD_r8_r8(B,D);

        // LD b,e
        case 0x43:
            LD_r8_r8(B,E);

        // LD b,h
        case 0x44:
            LD_r8_r8(B,H);

        // LD b,l
        case 0x45:
            LD_r8_r8(B,L);

        // LD b,(hl)
        case 0x46:
            LD_r8_HL(B);

        // LD b,a
        case 0x47:
            LD_r8_r8(B,A);

        // LD c,b
        case 0x48:
            LD_r8_r8(C,B);

        // LD c,c
        case 0x49:
            LD_r8_r8(C,C);

        // LD c,d
        case 0x4A:
            LD_r8_r8(C,D);

        // LD c,e
        case 0x4B:
            LD_r8_r8(C,E);

        // LD c,h
        case 0x4C:
            LD_r8_r8(C,H);

        // LD c,l
        case 0x4D:
            LD_r8_r8(C,L);

        // LD c,(hl)
        case 0x4E:
            LD_r8_HL(C);

        // LD c,a
        case 0x4F:
            LD_r8_r8(C,A);

        // LD d,b
        case 0x50:
            LD_r8_r8(D,B);

        // LD d,c
        case 0x51:
            LD_r8_r8(D,C);

        // LD d,d
        case 0x52:
            LD_r8_r8(D,D);

        // LD d,e
        case 0x53:
            LD_r8_r8(D,E);

        // LD d,h
        case 0x54:
            LD_r8_r8(D,H);

        // LD d,l
        case 0x55:
            LD_r8_r8(D,L);

        // LD d,(hl)
        case 0x56:
            LD_r8_HL(D);

        // LD d,a
        case 0x57:
            LD_r8_r8(D,A);

        // LD e,b
        case 0x58:
            LD_r8_r8(E,B);

        // LD e,c
        case 0x59:
            LD_r8_r8(E,C);

        // LD e,d
        case 0x5A:
            LD_r8_r8(E,D);

        // LD e,e
        case 0x5B:
            LD_r8_r8(E,E);

        // LD e,h
        case 0x5C:
            LD_r8_r8(E,H);

        // LD e,l
        case 0x5D:
            LD_r8_r8(E,L);

        // LD e,(hl)
        case 0x5E:
            LD_r8_HL(E);

        // LD e,a
        case 0x5F:
            LD_r8_r8(E,A);

        // LD h,b
        case 0x60:
            LD_r8_r8(H,B);

        // LD h,c
        case 0x61:
            LD_r8_r8(H,C);

        // LD h,d
        case 0x62:
            LD_r8_r8(H,D);

        // LD h,e
        case 0x63:
            LD_r8_r8(H,E);

        // LD h,h
        case 0x64:
            LD_r8_r8(H,H);

        // LD h,l
        case 0x65:
            LD_r8_r8(H,L);

        // LD h,(hl)
        case 0x66:
            LD_r8_HL(H);

        // LD h,a
        case 0x67:
            LD_r8_r8(H,A);

        // LD l,b
        case 0x68:
            LD_r8_r8(L,B);

        // LD l,c
        case 0x69:
            LD_r8_r8(L,C);

        // LD l,d
        case 0x6A:
            LD_r8_r8(L,D);

        // LD l,e
        case 0x6B:
            LD_r8_r8(L,E);

        // LD l,h
        case 0x6C:
            LD_r8_r8(L,H);

        // LD l,l
        case 0x6D:
            LD_r8_r8(L,L);

        // LD l,(hl)
        case 0x6E:
            LD_r8_HL(L);

        // LD l,a
        case 0x6F:
            LD_r8_r8(L,A);

        // LD (hl),b
        case 0x70:
            LD_HL_r8(B);

        // LD (hl),c
        case 0x71:
            LD_HL_r8(C);

        // LD (hl),d
        case 0x72:
            LD_HL_r8(D);

        // LD (hl),e
        case 0x73:
            LD_HL_r8(E);

        // LD (hl),h
        case 0x74:
            LD_HL_r8(H);

        // LD (hl),l
        case 0x75:
            LD_HL_r8(L);

        // LD (hl),a
        case 0x77:
            LD_HL_r8(A);

        // LD a,b
        case 0x78:
            LD_r8_r8(A,B);

        // LD a,c
        case 0x79:
            LD_r8_r8(A,C);

        // LD a,d
        case 0x7A:
            LD_r8_r8(A,D);

        // LD a,e
        case 0x7B:
            LD_r8_r8(A,E);

        // LD a,h
        case 0x7C:
            LD_r8_r8(A,H);

        // LD a,l
        case 0x7D:
            LD_r8_r8(A,L);

        // LD a,(hl)
        case 0x7E:
            LD_r8_HL(A);

        // LD a,a
        case 0x7F:
            LD_r8_r8(A,A);

        // SUB b
        case 0x90:
            SUB_FLAGS(B);
            A -= B;
            break;

        // SUB c
        case 0x91:
            SUB_FLAGS(C);
            A -= C;
            break;

        // SUB d
        case 0x92:
            SUB_FLAGS(D);
            A -= D;
            break;

        // SUB e
        case 0x93:
            SUB_FLAGS(E);
            A -= E;
            break;

        // SUB h
        case 0x94:
            SUB_FLAGS(H);
            A -= H;
            break;

        // SUB l
        case 0x95:
            SUB_FLAGS(L);
            A -= L;
            break;

        // SUB (hl)
        case 0x96:{
            cycles = 7;
            uint8_t tmpByte = sms.mem.getByte(HL);
            SUB_FLAGS(tmpByte);
            A -= tmpByte;
            break;
        }

        // SUB a
        case 0x97:
            SUB_FLAGS(A);
            A -= A;
            break;

        // CP b
        case 0xB8:
            SUB_FLAGS(B);
            break;

        // CP c
        case 0xB9:
            SUB_FLAGS(C);
            break;

        // CP d
        case 0xBA:
            SUB_FLAGS(D);
            break;

        // CP e
        case 0xBB:
            SUB_FLAGS(E);
            break;

        // CP h
        case 0xBC:
            SUB_FLAGS(H);
            break;

        // CP l
        case 0xBD:
            SUB_FLAGS(L);
            break;

        // CP (hl)
        case 0xBE:
            cycles = 7;
            SUB_FLAGS(sms.mem.getByte(HL));
            break;

        // CP a
        case 0xBF:
            SUB_FLAGS(A);
            break;

        // RET nz
        case 0xC0:
            RET_CC(nz_CC);
        
        // POP bc
        case 0xC1:
            popReg(BC);

        // JP nz,**
        case 0xC2:
            JP_CC(nz_CC);

        // JP **
        case 0xC3:
            cycles = 10;
            PC = fetch16();
            break;

        // CALL nz,**
        case 0xC4:
            CALL_CC(nz_CC);

        // PUSH bc
        case 0xC5:
            pushReg(BC);

        // RET z
        case 0xC8:
            RET_CC(z_CC);

        // RET
        case 0xC9:
            cycles = 10;
            PC = pop();
            break;

        // JP z,**
        case 0xCA:
            JP_CC(z_CC);

        // CALL z,**
        case 0xCC:
            CALL_CC(z_CC);

        // CALL **
        case 0xCD:
            CALL_CC(true);

        // RET nc
        case 0xD0:
            RET_CC(nc_CC);

        // POP de
        case 0xD1:
            popReg(DE);

        // JP nc,**
        case 0xD2:
            JP_CC(nc_CC);

        // OUT (*),a
        case 0xD3:
            cycles = 11;
            sms.ports.write(sms.mem.getByte(PC++), A);
            break;

        // CALL nc,**
        case 0xD4:
            CALL_CC(nc_CC);

        // PUSH de
        case 0xD5:
            pushReg(DE);

        // SUB *
        case 0xD6: {
            cycles = 7;
            uint8_t tmpByte = sms.mem.getByte(PC++);
            SUB_FLAGS(tmpByte);
            A -= tmpByte;
            break;            
        }

        // RET c
        case 0xD8:
            RET_CC(c_CC);

        // EXX
        case 0xD9:
            swap16(BC, BC_s);
            swap16(DE, DE_s);
            swap16(HL, HL_s);
            break;

        // JP c,**
        case 0xDA:
            JP_CC(c_CC);

        // CALL c,**
        case 0xDC:
            CALL_CC(c_CC);

        // RET po
        case 0xE0:
            RET_CC(po_CC);

        // POP hl
        case 0xE1:
            popReg(HL);

        // JP po,**
        case 0xE2:
            JP_CC(po_CC);

        // CALL po,**
        case 0xE4:
            CALL_CC(po_CC);

        // PUSH hl
        case 0xE5:
            pushReg(HL);

        // RET pe
        case 0xE8:
            RET_CC(pe_CC);

        // JP pe,**
        case 0xEA:
            JP_CC(pe_CC);

        // CALL pe,**
        case 0xEC:
            CALL_CC(pe_CC);

        // ED prefix: Extended Instructions
        case 0xED:
            cycles = prefixED();
            break;

        // RET p
        case 0xF0:
            RET_CC(p_CC);

        // POP af
        case 0xF1:
            popReg(AF);

        // JP p,**
        case 0xF2:
            JP_CC(p_CC);

        // DI: disable interrupts
        case 0xF3:
            IFF1 = 0;
            IFF2 = 0;
            break;

        // CALL p,**
        case 0xF4:
            CALL_CC(p_CC);

        // PUSH af
        case 0xF5:
            pushReg(AF);

        // RET m
        case 0xF8:
            RET_CC(m_CC);

        // JP m,**
        case 0xFA:
            JP_CC(m_CC);

        // EI: enable interrupts
        case 0xFB:
            IFF1 = 1;
            IFF2 = 1;
            break;

        // CALL m,**
        case 0xFC:
            CALL_CC(m_CC);

        // CP *
        case 0xFE:
            cycles = 7;
            SUB_FLAGS(sms.mem.getByte(PC++));
            break;

        // Unimplemented instruction
        default:
            cycles = 69;
            break;
    }

    return cycles;
}

#endif