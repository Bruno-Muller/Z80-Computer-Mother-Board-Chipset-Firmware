#include "z80.h"

void z80_resume() {
    Z80_RESUME = 1;
    __delay_us(1);
    Z80_RESUME = 0;
}

void z80_interrupt_acknowledge() {
    unsigned char interrupt_vector;

    if (interrupt_response.WAIT_ACK == 0) computer_error();

    if (interrupt_flags.USART == 1) {
        interrupt_vector = INTERRUPT_VECTOR_USART;
        interrupt_flags.USART = 0;
    }
    else if (interrupt_flags.KBD == 1) {
        interrupt_vector = INTERRUPT_VECTOR_USART;
        interrupt_flags.KBD = 0;
    }
    else if (interrupt_flags.HZ == 1) {
        interrupt_vector = INTERRUPT_VECTOR_CLOCK;
        interrupt_flags.HZ = 0;
    }
    else if (interrupt_flags.TMR == 1) {
        interrupt_vector = INTERRUPT_VECTOR_TIMER;
        interrupt_flags.TMR = 0;
    }
    /* else if (interrupt_flags.CLK == 1)  {
        interrupt_vector = INTERRUPT_VECTOR_CLOCK;
        interrupt_flags.CLK = 0;
    } */
    else {
        computer_error();
    }

    interrupt_response.WAIT_ACK = 0;
    z80_int_disassert();
    
    ioexp_data_write(IOEXP8_GPIO, interrupt_vector);
    ioexp_data_write(IOEXP8_IODIR, IOEXP_OUTPUTS);

    ioexp_data_write(IOEXP8_IODIR, IOEXP_INPUTS);
}