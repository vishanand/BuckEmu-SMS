#include "z80.hpp"
#include "sms.hpp"

/* Here lie the macro helpers */

// sets interrupt mode to M
#define IM(M){  \
    cycles = 8; \
    IM = M;    \
    break;  \
}

// loads intermediate value into 16-bit register
#define LD_r16_imm(reg){ \
    cycles = 10;    \
    reg = fetchWord();    \
    break;  \
}

// loads intermediate value into 8-bit register
#define LD_r8_imm(reg){ \
    cycles = 7; \
    reg = sms.mem.getByte(PC++);    \
    break;  \
}

// pushes register to stack
#define pushReg(reg){   \
    cycles = 11;    \
    push(reg);  \
    break;  \
}


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

        // LD b,*
        case 0x06:
            LD_r8_imm(B);
        
        // LD c,*
        case 0x0E:
            LD_r8_imm(C);

        // LD de,**
        case 0x11:
            LD_r16_imm(DE);

        // LD d,*
        case 0x16:
            LD_r8_imm(D);

        // LD e,*
        case 0x1E:
            LD_r8_imm(E);

        // LD hl,**
        case 0x21:
            LD_r16_imm(HL);

        // LD h,*
        case 0x26:
            LD_r8_imm(H);

        // LD l,*
        case 0x2E:
            LD_r8_imm(L);

        // LD sp,**
        case 0x31:
            LD_r16_imm(SP);

        // LD a,*
        case 0x3E:
            LD_r8_imm(A);

        // JP **
        case 0xC3:
            cycles = 10;
            PC = fetchWord();
            break;

        // PUSH bc
        case 0xC5:
            pushReg(BC);

        // CALL **
        case 0xCD:
            cycles = 17;
            push(PC+2);
            PC = fetchWord();
            break;

        // OUT (*),a
        case 0xD3:
            cycles = 11;
            sms.ports.write(sms.mem.getByte(PC++), A);
            break;

        // PUSH de
        case 0xD5:
            pushReg(DE);

        // PUSH hl
        case 0xE5:
            pushReg(HL);

        // ED prefix: Extended Instructions
        case 0xED:
            return prefixED();
            break;

        // DI: disable interrupts
        case 0xF3:
            IFF1 = 0;
            IFF2 = 0;
            break;

        // PUSH af
        case 0xF5:
            pushReg(AF);

        // EI: enable interrupts
        case 0xFB:
            IFF1 = 1;
            IFF2 = 1;
            break;

        default:
            cycles = 69;
            break;

    }

    return cycles;
}

// execute ED prefix instructions
inline int Z80::prefixED(){
    int cycles = 8;

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
inline uint16_t Z80::fetchWord(){
    uint16_t w = sms.mem.getByte(PC+1);
    w <<= 8;
    w |= sms.mem.getByte(PC);
    PC += 2;
    return w;
}

inline void Z80::push(uint16_t w){
    SP -= 2;
    sms.mem.setByte(SP + 1, (uint8_t) w & 0xFF);
    sms.mem.setByte(SP, (uint8_t) w >> 8);
}

inline uint16_t Z80::pop(){
    uint16_t w = sms.mem.getByte(SP);
    w <<= 8;
    w |= sms.mem.getByte(SP + 1);
    SP += 2;
    return w;
}