#include <cstdio>
#include "ports.hpp"

uint8_t Ports::read(uint8_t port){
    // TODO: read from ports
    // https://www.smspower.org/uploads/Development/smstech-20021112.txt
    return 0;
}

void Ports::write(uint8_t port, uint8_t val){
    // TODO: write from ports

    // SDSC control port
    if (port == 0xFC){
        std::printf("SDSC Control Port: %d\n", val);
    }

    // SDSC data output
    else if (port == 0xFD){
        std::printf("SDSC Data Port: %c\n", val);
    }
}