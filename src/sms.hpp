#ifndef SMS_H
#define SMS_H

#include "z80.hpp"
#include "memory.hpp"
#include "cartridge.hpp"

// The SMS class contains all the various hardware components of the Sega Master System
class SMS {
    public:
        SMS() : mem(*this) {};

        Z80 z80;
        Cartridge cart;
        Memory mem;
};

#endif