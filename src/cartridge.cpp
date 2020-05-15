#include <cstdio>
#include "cartridge.hpp"

// read ROM file in 16K chunks
bool Cartridge::openROM(std::string filename){
    FILE *fp = std::fopen(filename.c_str(), "rb");
    if (!fp){
        return false;
    }

    for (int i=0; i<256; i++){
        Banks.push_back(Bank());
        int len = std::fread(Banks.back().mem, 1, 0x4000, fp);
        if (len < 1){
            Banks.pop_back();
            break;
        }
    }

    std::fclose(fp);

    // check for TMR SEGA at 0x7FF0
    std::string data(8,'0');
    std::string chksum = "TMR SEGA";
    for (int i=0; i<8; i++){
        data[i] = (char) Banks[0].mem[0x7FF0 + i];
    }
    
    return data.compare(chksum) == 0;
}

Cartridge::~Cartridge(){
    Banks.clear();
}

uint8_t Cartridge::getByte(uint8_t page, uint16_t addr){
    return Banks[page % Banks.size()].mem[addr];
}

void Cartridge::setByte(uint8_t page, uint16_t addr, uint8_t val){
    Banks[page % Banks.size()].mem[addr] = val;
}
