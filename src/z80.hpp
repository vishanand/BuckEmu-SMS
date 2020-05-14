#ifndef Z80_H
#define Z80_H
#include <cstdint>

class Z80 {
    public:

    private:
        // CPU registers
        uint8_t A;   // accumulator (can be combined into AF)
        uint8_t F;   // flags
        uint8_t B;   // can be combined into BC
        uint8_t C;   
        uint8_t D;   // can be combined into DE
        uint8_t E;
        uint8_t H;   // can be combined into HL
        uint8_t L;
        uint16_t IX; // index X, Y
        uint16_t IY;
        uint16_t PC; // program counter
        uint16_t SP; // stack pointer
        uint8_t I;   // interrupt
        uint8_t R;   // refresh (used as RNG)
        bool IFF1;  // interrupt flip flops 1, 2
        bool IFF2;

        // Shadow registers (can be swapped with normal ones)
        uint8_t A_s;
        uint8_t F_s;
        uint8_t B_s;
        uint8_t C_s;
        uint8_t D_s;
        uint8_t E_s;
        uint8_t H_s;
        uint8_t L_s;

};

#endif