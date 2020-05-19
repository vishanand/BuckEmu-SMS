#ifndef SMS_H
#define SMS_H

#include "z80/z80.hpp"
#include "memory.hpp"
#include "cartridge.hpp"
#include "ports.hpp"

// The SMS class contains all the various hardware components of the Sega Master System
class SMS {
    public:
        SMS();

        Z80 z80;
        Cartridge cart;
        Memory mem;
        Ports ports;
};

#endif