// Game Cartridge ROMs
#ifndef CARTRIDGE_H
#define CARTRIDGE_H
#include <cstdint>
#include <string>
#include <vector>

struct Bank {
    uint8_t mem[0x4000];
};

class Cartridge {
    public:
        bool openROM(std::string filename); // return true if successful
        ~Cartridge();

        uint8_t getByte(uint8_t page, uint16_t addr);
        void setByte(uint8_t page, uint16_t addr, uint8_t val);

    private:
        std::vector<Bank> Banks; // 16K banks of ROM
        int numBanks;
};

#endif