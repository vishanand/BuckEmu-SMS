// System I/O Bus Ports
#ifndef PORTS_H
#define PORTS_H
#include <cstdint>

class SMS;

class Ports {
    public:
        Ports(SMS& smsP);
        void write(uint8_t port, uint8_t val);
        uint8_t read(uint8_t port);
    private:
        uint8_t memControl;
        SMS& sms; // reference to SMS object
};

#endif