#ifndef PORTS_H
#define PORTS_H
#include <cstdint>

class Ports {
    public:
        void write(uint8_t port, uint8_t val);
        uint8_t read(uint8_t port);
    private:
        uint8_t memControl;
};

#endif