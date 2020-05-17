#ifndef Z80_H
#define Z80_H
#include <cstdint>

class SMS;

class Z80 {
    public:
        Z80(SMS& smsP);
        int runInstruction();
        int getPC();

    private:
        // CPU registers
        union {     // AF register (can be used separately or combined)
            uint16_t AF;
            struct {
                uint8_t F;  // accumulator
                uint8_t A;  // flags
            };
        };

        union {     // BC register
            uint16_t BC;
            struct {
                uint8_t C;
                uint8_t B;
            };
        };

        union {     // DE register
            uint16_t DE;
            struct {
                uint8_t E;
                uint8_t D;
            };
        };

        union {     // HL register
            uint16_t HL;
            struct {
                uint8_t L;
                uint8_t H;
            };
        };

        uint16_t IX; // index X, Y
        uint16_t IY;
        uint16_t PC; // program counter
        uint16_t SP; // stack pointer
        uint8_t I;   // interrupt
        uint8_t R;   // refresh (used as RNG)
        bool IFF1;  // interrupt flip flops 1, 2
        bool IFF2;
        uint8_t IM; // interrupt mode

        // Shadow registers (can be swapped with normal ones)
        uint8_t AF_s;
        uint8_t BC_s;
        uint8_t DE_s;
        uint8_t HL_s;

        SMS& sms; // reference to SMS object

        inline int prefixED(); // ED prefix instructions
        inline uint16_t fetch16();
        inline void push(uint16_t word);
        inline uint16_t pop();
};

#endif