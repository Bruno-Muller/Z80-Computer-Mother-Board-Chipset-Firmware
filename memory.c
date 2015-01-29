#include "memory.h"

void memory_quick_read_prelude() {
    memory_state_read();
    ioexp_data_write(IOEXP8_IODIR, IOEXP_INPUTS);
    ioexp_address_write(IOEXP16_IODIRA, IOEXP_OUTPUTS);
    ioexp_address_write(IOEXP16_IODIRB, IOEXP_OUTPUTS);
}

void memory_quick_write_prelude() {
    memory_state_write();
    ioexp_data_write(IOEXP8_IODIR, IOEXP_OUTPUTS);
    ioexp_address_write(IOEXP16_IODIRA, IOEXP_OUTPUTS);
    ioexp_address_write(IOEXP16_IODIRB, IOEXP_OUTPUTS);
}

unsigned char memory_quick_read(unsigned int address) {
    ioexp_address_write(IOEXP16_GPIOA, address);
    ioexp_address_write(IOEXP16_GPIOB, address>>8);

    memory_select();
    __delay_us(1);
    unsigned char data =  ioexp_data_read(IOEXP8_GPIO);
    memory_unselect();

    return data;
}

void memory_quick_write(unsigned int address, unsigned char data) {
    ioexp_data_write(IOEXP8_GPIO, data);
    ioexp_address_write(IOEXP16_GPIOA, address);
    ioexp_address_write(IOEXP16_GPIOB, address>>8);

    memory_select();
    __delay_us(1);
    memory_unselect();
}

void memory_quick_operation_postlude() {
    ioexp_data_write(IOEXP8_IODIR, IOEXP_INPUTS);
    ioexp_address_write(IOEXP16_IODIRA, IOEXP_INPUTS);
    ioexp_address_write(IOEXP16_IODIRB, IOEXP_INPUTS);

    memory_state_idle();
}
/*
unsigned char memory_read(unsigned int address) {
    memory_state_read();

    ioexp_data_write(IOEXP8_IODIR, IOEXP_INPUTS);

    ioexp_address_write(IOEXP16_IODIRA, IOEXP_OUTPUTS);
    ioexp_address_write(IOEXP16_GPIOA, address);

    ioexp_address_write(IOEXP16_IODIRB, IOEXP_OUTPUTS);
    ioexp_address_write(IOEXP16_GPIOB, address>>8);


    memory_select();
    __delay_us(1);
    unsigned char data =  ioexp_data_read(IOEXP8_GPIO);
    memory_unselect();

    ioexp_address_write(IOEXP16_IODIRA, IOEXP_INPUTS);
    ioexp_address_write(IOEXP16_IODIRB, IOEXP_INPUTS);

    memory_state_idle();
    return data;
}

void memory_write(unsigned int address, unsigned char data) {
    memory_state_write();

    ioexp_data_write(IOEXP8_IODIR, IOEXP_OUTPUTS);
    ioexp_data_write(IOEXP8_GPIO, data);

    ioexp_address_write(IOEXP16_IODIRA, IOEXP_OUTPUTS);
    ioexp_address_write(IOEXP16_GPIOA, address);

    ioexp_address_write(IOEXP16_IODIRB, IOEXP_OUTPUTS);
    ioexp_address_write(IOEXP16_GPIOB, address>>8);

    memory_select();
    __delay_us(1);
    memory_unselect();

    //
    ioexp_data_write(IOEXP8_IODIR, IOEXP_INPUTS);
    ioexp_address_write(IOEXP16_IODIRA, IOEXP_INPUTS);
    ioexp_address_write(IOEXP16_IODIRB, IOEXP_INPUTS);

    memory_state_idle();
}

void memory_write_string(unsigned int address, const char* string) {
    while (1) {
        memory_write(address, *string);
        if (*string == 0) break;

        string++;
        address++;
    }
}
*/