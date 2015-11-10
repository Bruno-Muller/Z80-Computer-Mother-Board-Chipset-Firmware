#include "interrupt.h"

void interrupt interrupt_handler() {
    if (TMR1IE && TMR1IF) {
        TMR1ON = 0;
        TMR1IF = 0;
        TMR1H = ((15535 + 80) & 0xFF00) >> 8;
        TMR1L = ((15535 + 80) & 0x00FF);
        TMR1ON = 1;
        interrupt_flags.TMR = 1;
    } else if (RCIE && RCIF) {
        RCIF = 0;
        computer_char_buffer = RCREG;
        interrupt_flags.USART = 1;
    }/* else if (INTE && INTF) {
        INTF = 0;
    }*/
}