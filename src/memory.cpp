#include "sms.hpp"
#include "memory.hpp"


Memory::Memory(SMS& smsP) : sms(smsP) {
    mapReg = 0;
    page0 = 0;
    page1 = 1;
    page2 = 2;
}

uint8_t Memory::getByte(uint16_t addr){
    if (addr < 0x0400){
        // ROM Bank 0
        return sms.cart.getByte(0, addr);
    } else if (addr < 0x4000){
        // ROM Page 0
        return sms.cart.getByte(page0, addr);
    } else if (addr < 0x8000){
        // ROM Page 1
        return sms.cart.getByte(page1, addr);
    } else if (addr < 0xC000){
        // ROM Page 2 or Cartridge RAM
        // TODO: Enable cartridge RAM functionality (gamesaves)
        return sms.cart.getByte(page2, addr);
    } else if (addr < 0xE000){
        // 8 KB System RAM
        return RAM[addr - 0xC000];
    } else {
        // mirror of System RAM
        return RAM[addr - 0xE000];
    }
}

void Memory::setByte(uint16_t addr, uint8_t val){
    if (addr < 0x8000){
        // ignore ROM writes
        return;
    } else if (addr < 0xC000){
        // ROM Page 2 or Cartridge RAM
    } else if (addr < 0xE000){
        // 8 KB System RAM
        RAM[addr - 0xC000] = val;
    } else {
        // mirror of System RAM
        RAM[addr - 0xE000] = val;
    }

    // ROM paging
    switch (addr){
        case 0xFFFC:
            mapReg = val;
            break;
        case 0xFFFD:
            page0 = val;
            break;
        case 0xFFFE:
            page1 = val;
            break;
        case 0xFFFF:
            page2 = val;
            break;
    }
}
