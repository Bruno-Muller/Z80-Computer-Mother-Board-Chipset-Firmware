/* 
 * File:   newmain.c
 * Author: Bruno
 *
 * Created on 18 décembre 2014, 21:00
 */

#include <xc.h>

#include "bus.h"
#include "computer.h"
#include "conf.h"
#include "decoder.h"
#include "eeprom.h"
#include "event.h"
#include "front_panel.h"
#include "init.h"
#include "interrupt.h"
#include "ioexp.h"
#include "sdcard.h"
#include "spi.h"
#include "state_machine.h"
#include "usart.h"
#include "z80.h"

void rescue_bios() {
    unsigned char ret = sdcard_init();
    if (ret != SDCARD_SUCCESS) {
        usart_write_string("SDCARD MISSING OR FAILED!");
        while(1);
    }

    const unsigned int addresses[] = {512*0,512*1,512*2,512*3,512*4,512*5,512*6,512*7};

    memory_quick_write_prelude();

    unsigned char i;
    for (i = 0; i < 8; i++) {
        sdcard_read(addresses[i], i+1);
    }

    memory_quick_write_postlude();
}

void load_bios() {
    unsigned int address = 0;

    while (address < BIOS_MAX_SIZE) {
        eeprom_read(address, buffer); // read 64 bytes

        memory_quick_write_prelude();

        unsigned char i;
        for (i = 0; i < BUFFER_SIZE; i++, address++) {
            memory_quick_write(address, buffer[i]);
        }
        
        memory_quick_write_postlude();
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

    // event loop
    while(1) {
        interrupt_wait();
        di();

        unsigned char val = ioexp_interrupt_read(IOEXP8_INTCAP);
        unsigned char event;

        if ((val & INTERRUPT_Z80) == 0) {
            unsigned char port = bus_address_low_read();
            unsigned char data = bus_data_read();
            event = state_machine_get_event(port, data);
            event_handler(event, port, data);
        }
        if ((val & INTERRUPT_FRONT_PANEL) == 0) {
            front_panel();
        }
        

        interrupt_ack();
        z80_resume();
        ei();
    }

}