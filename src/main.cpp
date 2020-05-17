#include <cstdio>
#include <string>

#include "sms.hpp"

int main(){
    SMS sms;

    std::printf("BuckEmu SMS: a Sega Master System Emulator!\n");

    // load ROM into memory
    std::string filename = "ROM/zexdoc_sdsc.sms";
    if (!sms.cart.openROM(filename)){
        std::printf("Error opening ROM file '%s'\n", filename.c_str());
        return 1;
    }

    // run instructions until we hit an unimplemented instruction
    int cycles = 0;
    std::printf("\nStarting emulation at 0x00!\n");
    while (cycles < 69){
        uint16_t pc = sms.z80.getPC();
        uint16_t opcode = sms.mem.getByte(pc);
        if (opcode == 0xED){
            opcode = (opcode << 8) | sms.mem.getByte(pc+1);
        }

        cycles = sms.z80.runInstruction();

        std::printf("PC: 0x%X \tOpcode: 0x%X \tCycles: %d\n", pc, opcode, cycles);
    }
    
    return 0;
}
