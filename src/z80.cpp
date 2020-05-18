#include "z80.hpp"
#include "sms.hpp"

/* Here lie the macro helpers */

// macros to manipulate bit n of byte x
#define SetBit(x,n)     ((x) |= (1<<(n)))
#define ClearBit(x,n)   ((x) &= ~(1<<(n)))
#define FlipBit(x,n)    ((x) ^= (1<<(n)))
#define CheckBit(x,n)   ((x) & (1<<(n)))

// define flag constants 
#define CF  0   // Carry flag
#define NF  1   // Subtract (negative) flag
#define PVF 2   // Parity/Overflow flag
#define HF  4   // Half carry flag
#define ZF  6   // Zero flag
#define SF  7   // Sign flag

// define condition codes
#define z_CC   (CheckBit(F, ZF))  // ZF set
#define c_CC   (CheckBit(F, CF))  // CF set
#define pe_CC   (CheckBit(F, PVF))  // PVF set
#define m_CC   (CheckBit(F, SF))  // SF set
#define nz_CC   (!z_CC)  // ZF reset
#define nc_CC   (!c_CC)  // CF reset
#define po_CC   (!pe_CC)  // PVF reset
#define p_CC   (!m_CC)  // SF reset

// sets interrupt mode to M
#define IM(M){  \
    cycles = 8; \
    IM = M;    \
    break;  \
}

// loads immediate value into 16-bit register
#define LD_r16_imm(reg){ \
    cycles = 10;    \
    reg = fetch16();    \
    break;  \
}

// loads immediate value into 8-bit register
#define LD_r8_imm(reg){ \
    cycles = 7; \
    reg = sms.mem.getByte(PC++);    \
    break;  \
}

// loads 8-bit register into 8-bit register
#define LD_r8_r8(dest, src){ \
    dest = src; \
    break;  \
}

// loads contents of (hl) into reg
#define LD_r8_HL(reg){  \
    cycles = 7; \
    reg = sms.mem.getByte(HL);    \
    break;  \
}

// loads reg into (hl)
#define LD_HL_r8(reg){  \
    cycles = 7; \
    sms.mem.setByte(HL, reg);   \
    break;  \
}

// increment a 16-bit register (no flags)
#define INC_R16(reg){   \
    cycles = 6; \
    reg++;  \
    break;  \
}

// decrement a 16-bit register (no flags)
#define DEC_R16(reg){   \
    cycles = 6; \
    reg--;  \
    break;  \
}

// return if condition is true
#define RET_CC(cc){ \
    cycles = 5; \
    if (cc){    \
        cycles = 11;    \
        PC = pop(); \
    }   \
    break;  \
}

// jump if condition is true
#define JP_CC(cc){  \
    cycles = 10;    \
    uint16_t addr = fetch16();  \
    if (cc){    \
        PC = addr;  \
    }    \
    break; \
}

// pushes register to stack
#define pushReg(reg){   \
    cycles = 11;    \
    push(reg);  \
    break;  \
}

// pops from stack to register
#define popReg(reg){    \
    cycles = 10;    \
    reg = pop();    \
    break;  \
}

// swaps two 16 bit registers
#define swap16(a, b){   \
    uint16_t tmpSwp = a;  \
    a = b;  \
    b = tmpSwp;   \
}

// increment last 7 bits of R register
#define R_inc() R = (R & 0x80) | ((R+1) & 0x7F);


/* Here lie the class method implementations */

Z80::Z80(SMS& smsP) : sms(smsP){
    // default register values
    AF = 0;
    PC = 0;
    SP = 0xFFFF;
    IFF1 = false;
    IFF2 = false;
    IM = 0;
}

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

        // DEC bc
        case 0x0B:
            DEC_R16(BC);
        
        // LD c,*
        case 0x0E:
            LD_r8_imm(C);

        // LD de,**
        case 0x11:
            LD_r16_imm(DE);

        // INC de
        case 0x13:
            INC_R16(DE);

        // LD d,*
        case 0x16:
            LD_r8_imm(D);

        // DEC de
        case 0x1B:
            DEC_R16(DE);

        // LD e,*
        case 0x1E:
            LD_r8_imm(E);

        // LD hl,**
        case 0x21:
            LD_r16_imm(HL);

        // INC hl
        case 0x23:
            INC_R16(HL);

        // LD h,*
        case 0x26:
            LD_r8_imm(H);

        // DEC hl
        case 0x2B:
            DEC_R16(HL);

        // LD l,*
        case 0x2E:
            LD_r8_imm(L);

        // LD sp,**
        case 0x31:
            LD_r16_imm(SP);

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

        // CALL **
        case 0xCD:
            cycles = 17;
            push(PC+2);
            PC = fetch16();
            break;

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

        // PUSH de
        case 0xD5:
            pushReg(DE);

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

        // RET po
        case 0xE0:
            RET_CC(po_CC);

        // POP hl
        case 0xE1:
            popReg(HL);

        // JP po,**
        case 0xE2:
            JP_CC(po_CC);

        // PUSH hl
        case 0xE5:
            pushReg(HL);

        // RET pe
        case 0xE8:
            RET_CC(pe_CC);

        // JP pe,**
        case 0xEA:
            JP_CC(pe_CC);

        // ED prefix: Extended Instructions
        case 0xED:
            return prefixED();
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

        // Unimplemented instruction
        default:
            cycles = 69;
            break;
    }

    return cycles;
}

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

// return PC
int Z80::getPC(){
    return PC;
}

// fetch 16 bits and increment PC
inline uint16_t Z80::fetch16(){
    uint16_t w = sms.mem.getByte(PC+1);
    w <<= 8;
    w |= sms.mem.getByte(PC);
    PC += 2;
    return w;
}

inline void Z80::push(uint16_t w){
    SP -= 2;
    sms.mem.setByte(SP, (uint8_t) w & 0xFF);
    sms.mem.setByte(SP + 1, (uint8_t) w >> 8);
}

inline uint16_t Z80::pop(){
    uint16_t w = sms.mem.getByte(SP + 1);
    w <<= 8;
    w |= sms.mem.getByte(SP);
    SP += 2;
    return w;
}

// LDI and LDD instructions
// INC=1 to increment HL,DE     (LDI mode)
// INC=-1 to decrement HL,DE    (LDD mode)
// Copies a byte from (HL) to (DE)
inline void Z80::LDI_LDD(int8_t INC){
    uint8_t tmpByte = sms.mem.getByte(HL);
    sms.mem.setByte(DE, tmpByte);
    
    HL += INC;
    DE += INC;
    BC--;
    if (BC > 0){
        SetBit(F, PVF);
    } else {
        ClearBit(F, PVF);
    }

    ClearBit(F, HF);
    ClearBit(F, NF);
}
