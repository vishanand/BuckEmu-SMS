#include <cstdio>
#include "ports.hpp"
#include "sms.hpp"

Ports::Ports(SMS& smsP) : sms(smsP) {
    
}

uint8_t Ports::read(uint8_t port){
    // TODO: read from ports
    // https://www.smspower.org/uploads/Development/smstech-20021112.txt

    
    // VDP Input
    if (port <= 0xBF && port >= 0x80){
        if (port % 2 == 0){
            return sms.vdp.read_data();
        } else {
            return sms.vdp.read_control();
        }
    }

    // VDP H and V counters
    if (port <= 0x7F && port >= 0x40){
        if (port % 2 == 0){
            return sms.vdp.v_counter;
        } else {
            return sms.vdp.h_counter;
        }
    }

    return 0xFF;
}

void Ports::write(uint8_t port, uint8_t val){
    // TODO: write from ports

    // SDSC control port
    if (port == 0xFC){
        std::printf("SDSC Control Port: %d\n\n", val);
    }

    // SDSC data output
    else if (port == 0xFD){
        //std::printf("SDSC Data Port: %c\n", val);
        std::printf("%c", val);
    }

    // VDP Output
    if (port <= 0xBF && port >= 0x80){
        if (port % 2 == 0){
            sms.vdp.write_data(val);
        } else {
            sms.vdp.write_control(val);
        }
    }
}