#include "decoder.h"

void decoder_select(unsigned char address) {
    PORTA = address | (PORTA & ~0x07) ;
    DECODER_SELECT = 0;
}

void decoder_unselect() {
    DECODER_SELECT = 1;
}