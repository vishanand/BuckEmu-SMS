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

// increment last 7 bits of R register
#define R_inc() R = (R & 0x80) | ((R+1) & 0x7F);

#endif