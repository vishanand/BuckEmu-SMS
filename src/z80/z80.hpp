// Z80 CPU
#ifndef Z80_H
#define Z80_H
#include <cstdint>

class SMS;

class Z80 {
    public:
        Z80(SMS& smsP);
        int runInstruction();
        int getPC();
        void interrupt(uint8_t data);
        void NMI();

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

        union {     // IX register
            uint16_t IX;
            struct {
                uint8_t IX_L;
                uint8_t IX_H;
            };
        };

        union {     // IY register
            uint16_t IY;
            struct {
                uint8_t IY_L;
                uint8_t IY_H;
            };
        };

        union {     // SP stack pointer
            uint16_t SP;
            struct {
                uint8_t SP_L;
                uint8_t SP_H;
            };
        };

        uint16_t PC; // program counter
        uint8_t I;   // interrupt
        uint8_t R;   // refresh (used as RNG)
        bool IFF1;  // interrupt flip flops 1, 2
        bool IFF2;
        uint8_t EI_f;  // enable interrupts counter (0=do nothing, 1=enable)
        uint8_t IM; // interrupt mode
        bool halt; // is halted
        bool NMI_f;

        // Shadow registers (can be swapped with normal ones)
        uint8_t AF_s;
        uint8_t BC_s;
        uint8_t DE_s;
        uint8_t HL_s;

        SMS& sms; // reference to SMS object

        // helper functions
        inline int prefixED();
        inline int prefixCB();
        inline int prefix_xD(uint16_t &IZ, uint8_t &IZ_H, uint8_t &IZ_L);
        inline int prefix_xDCB(uint16_t &IZ, uint8_t &IZ_H, uint8_t &IZ_L);
        inline uint16_t fetch16();
        inline void push(uint16_t word);
        inline uint16_t pop();
        inline void LDI_LDD(int8_t INC);
        inline void SUB_FLAGS(uint8_t Reg, uint8_t N);
        inline bool getParity(uint8_t N);
        inline void DAA_inst();
};

#endif