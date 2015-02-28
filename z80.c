#include "z80.h"

volatile unsigned char z80_interrupt_vector;

void z80_resume() {
    Z80_RESUME = 0;
    __delay_us(1);
    Z80_RESUME = 1;    
}

void z80_interrupt_acknowledge() {
    z80_int_disassert();
    ioexp_data_write(IOEXP8_GPIO, z80_interrupt_vector);
    ioexp_data_write(IOEXP8_IODIR, IOEXP_OUTPUTS);

    ioexp_data_write(IOEXP8_IODIR, IOEXP_INPUTS);
}