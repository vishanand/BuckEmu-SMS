#include <cstdio>
#include <string>

#include "sms.hpp"

int main(){
    SMS sms;

    std::printf("BuckEmu SMS: a Sega Master System Emulator!\n");

    std::string filename = "ROM/zexdoc.sms";
    if (!sms.cart.openROM(filename)){
        std::printf("Error opening ROM file '%s'\n", filename.c_str());
        return 1;
    }
    
    return 0;
}
