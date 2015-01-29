#include "z80.h"

void z80_resume() {
    Z80_RESUME = 0;
    __delay_us(1);
    Z80_RESUME = 1;    
}

