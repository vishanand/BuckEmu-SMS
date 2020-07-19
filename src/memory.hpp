// Main Memory (RAM)
#ifndef MEMORY_H
#define MEMORY_H
#include <cstdint>

class SMS;

class Memory {
    public:
        Memory(SMS& smsP);
        uint8_t getByte(uint16_t addr);
        void setByte(uint16_t addr, uint8_t val);
        
    private:
        uint8_t RAM[0x2000]; // 8 KB of RAM
        uint8_t mapReg; // mapping register
        uint8_t page0; // bytes that reference pages in cartridge
        uint8_t page1;
        uint8_t page2;
        SMS& sms; // reference to SMS object
};

#endif