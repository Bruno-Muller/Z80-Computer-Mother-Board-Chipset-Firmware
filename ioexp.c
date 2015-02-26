#include "ioexp.h"

void ioexp_init() {
    ioexp_data_write(IOEXP8_IOCON, IOEXP_CONF);
    ioexp_data_write(IOEXP8_IODIR, IOEXP_INPUTS);

    ioexp_address_write(IOEXP16_IOCON, IOEXP_CONF);
    ioexp_address_write(IOEXP16_IODIRA, IOEXP_INPUTS);
    ioexp_address_write(IOEXP16_IODIRB, IOEXP_INPUTS);

    ioexp_interrupt_write(IOEXP8_IOCON, IOEXP_CONF);
    ioexp_interrupt_write(IOEXP8_IODIR, IOEXP_INTERRUPT_IODIR);
    ioexp_interrupt_write(IOEXP8_INTCON, IOEXP_INTERRUPT_INTCON);
}

unsigned char ioexp_address_read(unsigned char address) {
    ioexp_address_select();
    SSPBUF = IOEXP_READ;
    while (!BF);
    SSPBUF = address;
    while (!BF);
    SSPBUF = 0x00;
    while (!BF);
    ioexp_unselect();
    return SSPBUF;
}

void ioexp_address_write(unsigned char address, unsigned char data) {
    ioexp_address_select();
    ioexp_write(address, data);
    ioexp_unselect();
}

unsigned char ioexp_data_read(unsigned char address) {
    ioexp_data_select();
    SSPBUF = IOEXP_READ;
    while (!BF);
    SSPBUF = address;
    while (!BF);
    SSPBUF = 0x00;
    while (!BF);
    ioexp_unselect();
    return SSPBUF;
}

void ioexp_data_write(unsigned char address, unsigned char data) {
    ioexp_data_select();
    ioexp_write(address, data);
    ioexp_unselect();
}

void ioexp_interrupt_write(unsigned char address, unsigned char data) {
    ioexp_interrupt_select();
    ioexp_write(address, data);
    ioexp_unselect();
}

unsigned char ioexp_interrupt_read(unsigned char address) {
    ioexp_interrupt_select();
    SSPBUF = IOEXP_READ;
    while (!BF);
    SSPBUF = address;
    while (!BF);
    SSPBUF = 0x00;
    while (!BF);
    ioexp_unselect();
    return SSPBUF;
}

unsigned char ioexp_read(unsigned char address) {
    SSPBUF = IOEXP_READ;
    while (!BF);
    SSPBUF = address;
    while (!BF);
    SSPBUF = 0x00;
    while (!BF);
    return SSPBUF;
}

void ioexp_write(unsigned char address, unsigned char data) {
    SSPBUF = IOEXP_WRITE;
    while (!BF);
    SSPBUF = address;
    while (!BF);
    SSPBUF = data;
    while (!BF);
}
