#ifndef Z80_MACRO_H
#define Z80_MACRO_H

/* Here lie the macro helpers for the Z80 class */

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

// CPI and CPD helper
#define CPI_CPD(){  \
    cycles = 16;    \
    SUB_FLAGS(sms.mem.getByte(HL)); \
    if (BC != 0)    \
        SetBit(F, PVF); \
    else    \
        ClearBit(F, PVF);   \
    BC--;   \
}

// JR cc,*  - Jump relative if condition met
#define JR_CC(cc){  \
    int8_t rAddr = (int8_t) sms.mem.getByte(PC++);   \
    cycles = 7; \
    if (cc){    \
        PC = PC + rAddr;  \
        cycles = 12;    \
    }   \
}

// CALL on condition code
#define CALL_CC(cc){    \
    if (cc){    \
        cycles = 17;    \
        push(PC+2); \
        PC = fetch16(); \
    } else {    \
        cycles = 10;    \
        PC += 2;    \
    }   \
    break;  \
}

// set Zero and Sign flags based on register
#define SZ_FLAGS(reg){  \
    if ((reg) == 0)   SetBit(F, ZF);  \
    else    ClearBit(F, ZF);    \
    if (CheckBit((reg), SF))  SetBit(F, SF);  \
    else    ClearBit(F, SF);    \
}

// Set Parity flag based on register
#define PARITY_FLAG(reg){   \
    if (getParity(reg)) SetBit(F, PVF);    \
    else    ClearBit(F, PVF);   \
}

// AND operation
#define AND_R8(reg){    \
    ClearBit(F, CF);    \
    ClearBit(F, NF);    \
    SetBit(F, HF);  \
    A = A & (reg);  \
    PARITY_FLAG(A); \
    SZ_FLAGS(A);    \
    break;  \
}

// OR operation
#define OR_R8(reg){    \
    ClearBit(F, CF);    \
    ClearBit(F, NF);    \
    ClearBit(F, HF);  \
    A = A | (reg);  \
    PARITY_FLAG(A); \
    SZ_FLAGS(A);    \
    break;  \
}

// XOR operation
#define XOR_R8(reg){    \
    ClearBit(F, CF);    \
    ClearBit(F, NF);    \
    ClearBit(F, HF);  \
    A = A ^ (reg);  \
    PARITY_FLAG(A); \
    SZ_FLAGS(A);    \
    break;  \
}

// Increment 8-bit register
#define INC_R8(reg){    \
    if ((( (reg) & 0xF) + (1 & 0xF)) & 0x10)    SetBit(F, HF);  \
    else    ClearBit(F, HF);    \
    ClearBit(F, NF);    \
    reg++;  \
    if (reg == 0)   SetBit(F, PVF);  \
    else    ClearBit(F, PVF);    \
    SZ_FLAGS(reg);  \
}

// Decrement 8-bit register
#define DEC_R8(reg){    \
    if ((( (reg) & 0xF) - (1 & 0xF)) & 0x10)    SetBit(F, HF);  \
    else    ClearBit(F, HF);    \
    SetBit(F, NF);    \
    if (reg == 0)   SetBit(F, PVF);  \
    else    ClearBit(F, PVF);    \
    reg--;  \
    SZ_FLAGS(reg);  \
}

// add two 16-bit registers
#define ADD_R16(reg1, reg2){ \
    ClearBit(F, NF);    \
    if ((( (reg1) & 0xFFF) + ((reg2) & 0xFFF)) & 0x1000)    SetBit(F, HF);  \
    else    ClearBit(F, HF);    \
    uint32_t tmp = reg1 + reg2; \
    if (tmp & 0x10000)  SetBit(F, CF);  \
    else    ClearBit(F, CF);    \
    reg1 = (uint16_t) tmp;  \
}

// add 8 bit register to A
#define ADD_A_R8(reg){  \
    ClearBit(F, NF);    \
    if ((( (A) & 0xF) + ((reg) & 0xF)) & 0x10)  SetBit(F, HF);  \
    else    ClearBit(F, HF);     \
    \
    uint8_t oldA = A;   \
    A = A + reg;    \
    if (A < oldA){  \
        SetBit(F, CF);  \
        SetBit(F, PVF); \
    } else {    \
        ClearBit(F, CF);    \
        ClearBit(F, PVF);   \
    }   \
    \
    SZ_FLAGS(A);    \
}

// Load from 16 bit register to memory
#define LD_mem_r16(regH, regL){ \
    uint16_t addr = fetch16();  \
    sms.mem.setByte(addr, regL);    \
    sms.mem.setByte(addr + 1, regH);    \
    break;  \
}

// Load from memory to 16 bit register
#define LD_r16_mem(regH, regL){ \
    uint16_t addr = fetch16();  \
    regL = sms.mem.getByte(addr);   \
    regH = sms.mem.getByte(addr + 1);   \
    break;  \
}

// get offset
#define GET_OFFSET()    int8_t offset = (int8_t) sms.mem.getByte(PC++);

// loads reg into (iz+*)
#define LD_IZ_offset_r8(reg){  \
    cycles = 19; \
    GET_OFFSET(); \
    sms.mem.setByte(IZ + offset, reg);   \
    break;  \
}

// loads contents of (iz+*) into reg
#define LD_r8_IZ_offset(reg){  \
    cycles = 19; \
    GET_OFFSET();   \
    reg = sms.mem.getByte(IZ + offset);    \
    break;  \
}

// increment last 7 bits of R register
#define R_inc() R = (R & 0x80) | ((R+1) & 0x7F);

#endif