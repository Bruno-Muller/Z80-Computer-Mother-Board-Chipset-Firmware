/* 
 * File:   newmain.c
 * Author: Bruno
 *
 * Created on 18 décembre 2014, 21:00
 */

#include <xc.h>

#include "bool.h"
#include "bus.h"
#include "clock.h"
#include "computer.h"
#include "conf.h"
#include "decoder.h"
#include "eeprom.h"
#include "front_panel.h"
#include "init.h"
#include "interrupt.h"
#include "ioexp.h"
#include "keyboard.h"
#include "sdcard.h"
#include "spi.h"
#include "usart.h"
#include "z80.h"

void rescue_bios() {
    unsigned char ret = sdcard_init();
    if (ret != SDCARD_SUCCESS) {
        usart_write_string("SDCARD MISSING OR FAILED!");
        while(1);
    }

    const unsigned int addresses[] = {512*0,512*1,512*2,512*3,512*4,512*5,512*6,512*7};

    memory_write_prelude();

    unsigned char i;
    for (i = 0; i < 8; i++) {
        sdcard_read(addresses[i], i+1);
    }

    memory_write_postlude();
}

void load_bios() {
    unsigned int address = 0;

    while (address < BIOS_MAX_SIZE) {
        eeprom_read(address, buffer); // read 32 bytes

        memory_write_prelude();

        unsigned char i;
        for (i = 0; i < EEPROM_PAGE_SIZE; i++, address++) {
            memory_write(address, buffer[i]);
        }
        
        memory_write_postlude();
    }
}

void main(void)
{
    init();
    
    z80_bus_request_from_wait_state();
    computer_memory_active();

    //rescue_bios();
    load_bios();
        
    z80_reset_assert();

    computer_memory_idle();
    z80_bus_release();

    usart_interrupt_enable();

    ioexp_interrupt_read(IOEXP8_INTCAP);
    ioexp_interrupt_write(IOEXP8_GPINTEN, IOEXP_INTERRUPT_MASK);

    z80_interrupt_enable();
    
    z80_reset_disassert();

    z80_resume();

    unsigned char throw_interrupt = FALSE;

    while(1) {
        interrupt_wait();
        di();

        unsigned char int_ctrl_intf = ioexp_interrupt_read(IOEXP8_INTF);
        unsigned char int_ctrl_intcap = ioexp_interrupt_read(IOEXP8_INTCAP);

        if ((int_ctrl_intcap & INTERRUPT_Z80) == 0) {
            computer_parameters.state = ((PORTA & 0b00110000) >> 3) | ((PORTB & 0b00000010) >> 1);
            computer_parameters.port = bus_address_low_read();
            computer_parameters.data = bus_data_read();
            computer_parameters.handler = NULL;
            computer_handler();
            if (computer_parameters.handler != NULL) (*computer_parameters.handler)();
        }
        if ((int_ctrl_intcap & INTERRUPT_FRONT_PANEL) == 0) {
            front_panel_handler();
        }
        if ((int_ctrl_intcap & INTERRUPT_KEYBOARD) == 0) {
            computer_char_buffer = keyboard_read();
            if (computer_char_buffer != 0x00) {
                z80_interrupt_vector = INTERRUPT_VECTOR_USART;
                throw_interrupt = TRUE;
            }
        }
        if (((int_ctrl_intf & INTERRUPT_1HZ) != 0) && ((int_ctrl_intcap & INTERRUPT_1HZ) == 0)) {
            clock_handler();
        }

        
        interrupt_acknowledge();
        z80_resume();
        ei();

        if (((int_ctrl_intcap & INTERRUPT_KEYBOARD) != 0) && (throw_interrupt != FALSE)) {
            throw_interrupt = FALSE;
            z80_int_assert();
        }
    }

}