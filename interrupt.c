#include "interrupt.h"

void interrupt interrupt_handler() {
    if (TMR1IE && TMR1IF) {
        TMR1ON = 0;
        TMR1IF = 0;
        TMR1H = ((15535 + 80) & 0xFF00) >> 8;
        TMR1L = ((15535 + 80) & 0x00FF);
        TMR1ON = 1;
        z80_interrupt_vector = INTERRUPT_VECTOR_TIMER;
        z80_int_assert(); // throw interrupt to z80
    } else if (RCIE && RCIF) {
        RCIF = 0;
        z80_char_buffer = RCREG;
        z80_interrupt_vector = INTERRUPT_VECTOR_USART;
        z80_int_assert(); // throw interrupt to z80
    } else if (INTE && INTF) {
        INTF = 0;
        interrupt_occured = INTERRUPT_OCCURED;
    }
}