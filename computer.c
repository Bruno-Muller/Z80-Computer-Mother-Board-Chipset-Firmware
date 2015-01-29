#include "computer.h"
/*
void computer_memory_write(unsigned int address, unsigned char data)
{
    z80_bus_request();
    computer_memory_active();
    memory_write(address, data);
    computer_memory_idle();
    z80_bus_release();
}

unsigned char computer_memory_read(unsigned int address)
{
    z80_bus_request();
    computer_memory_active();
    unsigned char response = memory_read(address);
    computer_memory_idle();
    z80_bus_release();
    return response;
}
*/