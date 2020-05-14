#include <cstdio>
#include <string>

#include "sms.hpp"

int main(){
    SMS sms;

    std::printf("BuckEmu SMS: a Sega Master System Emulator!\n");

    std::string filename = "ROM/zexdoc.sms";
    if (!sms.cart.openROM(filename)){
        std::printf("Error opening ROM file %s\n", filename.c_str());
        return 1;
    }
    
    // check for TMR SEGA as ROM checksum
    for (int i=0; i<8; i++){
        std::printf("%c", sms.cart.getByte(0, 0x7FF0 + i));
    }
    std::printf("\n");

    return 0;
}