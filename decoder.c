#include "decoder.h"

void decoder_select(unsigned char address) {
    switch(address) {
        case DECODER_SELECT_DATA:
        case DECODER_SELECT_ADDRESS:
        case DECODER_SELECT_EEPROM:
        case DECODER_SELECT_SDCARD:
        case DECODER_SELECT_INTERRUPT:
        case DECODER_SELECT_FRONT_PANEL:
            PORTA =  address | (PORTA & ~0x07) ;
            DECODER_SELECT = 0;
            break;
        default:
            DECODER_SELECT = 1;
    }
}

void decoder_unselect() {
    DECODER_SELECT = 1;
}