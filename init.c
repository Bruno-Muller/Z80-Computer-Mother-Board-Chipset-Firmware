#include "init.h"

void init() {

    /*  Oscillator :
     *  16 MHz
     */
    OSCCON = 0x30;

    /* Option :
     * PORTB pull-ups are disabled
     * Interrupt on falling edge of INT pin
     * Internal instruction cycle clock (FOSC/4)
     * Increment on low-to-high transition on T0CKI pin
     * Prescaler is assigned to the Timer0 module
     * Prescaler Rate Select bits Timer0 1:2
     */
    OPTION_REG = 0x80;

    // Init ports
    PORTA = DEFAULT_PORTA;
    ANSELA = DEFAULT_ANSELA;
    TRISA  = DEFAULT_TRISA;

    PORTB = DEFAULT_PORTB;
    ANSELB = DEFAULT_ANSELB;
    TRISB  = DEFAULT_TRISB;

    Z80_INT = 1;
    TRISC0 = 0; // Z80_INT

    // Internal configuration
    usart_init();
    spi_init();

    // External configuration
    ioexp_init();

    // Timer
    T1CON = 0b00100000; // Timer1 clock source is instruction clock (FOSC/4) ; 1:4 Prescale value
    T1GCON = 0x00;

    // Enable interrupt
    PEIE = 1;
    ei();
    
}