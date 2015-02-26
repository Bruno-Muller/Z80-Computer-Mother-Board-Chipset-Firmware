#include "timer.h"

void timer_enable() {
    TMR1H = ((15535 + 80) & 0xFF00) >> 8;
    TMR1L = ((15535 + 80) & 0x00FF);
    TMR1ON = 1;
    TMR1IE = 1;
}

void timer_disable() {
    TMR1IE = 0;
    TMR1ON = 0;
}