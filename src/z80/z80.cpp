#include "z80.hpp"
#include "../sms.hpp"

// include rest of Z80 class code here
#include "z80_macro.hpp"
#include "z80_no_prefix.hpp"
#include "z80_ED_prefix.hpp"
#include "z80_CB_prefix.hpp"
#include "z80_xD_prefix.hpp"
#include "z80_xDCB_prefix.hpp"

Z80::Z80(SMS& smsP) : sms(smsP){
    // default register values
    AF = 0;
    PC = 0;
    SP = 0xFFFF;
    IFF1 = false;
    IFF2 = false;
    IM = 0;
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
    sms.mem.setByte(SP, (uint8_t) (w & 0xFF));
    sms.mem.setByte(SP + 1, (uint8_t) (w >> 8));
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

// set subtraction flags
inline void Z80::SUB_FLAGS(uint8_t Reg, uint8_t N){
    // Zero flag
    if (Reg == N)
        SetBit(F, ZF);
    else
        ClearBit(F, ZF);

    // Carry flag
    if (Reg < N){
        SetBit(F, CF);
    } else{
        ClearBit(F, CF);
    }

    // Sign flag
    if ((Reg - N) & 0x80){
        SetBit(F, SF);
    } else {
        ClearBit(F, SF);
    }

    // Half-carry flag
    if (((Reg & 0xF) - (N & 0xF)) & 0x10)
        SetBit(F, HF);
    else
        ClearBit(F, HF);

    // Overflow flag
    if ((int8_t) Reg < (int8_t) N){
        if (CheckBit(F, SF))    ClearBit(F, PVF);
        else    SetBit(F, PVF);
    } else{
        if (CheckBit(F, SF))    SetBit(F, PVF);
        else    ClearBit(F, PVF);
    }

    // Subtract flag
    SetBit(F, NF);
}

// Get Parity of Byte
inline bool Z80::getParity(uint8_t N){
    uint8_t x = N;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (~x) & 1;
}
