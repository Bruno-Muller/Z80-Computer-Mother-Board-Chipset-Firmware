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
#include "null.h"
#include "sdcard.h"
#include "spi.h"
#include "usart.h" 
#include "z80.h"

#include "vga.h"

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
            memory_write_prelude();
            memory_write(address, buffer[i]);
        }
        
        memory_write_postlude();
    }
}

void main(void)
{
    init();

    interrupt_flags.value = 0;
    interrupt_response.value = 0;

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

    //z80_interrupt_enable();
    
    z80_reset_disassert();
    z80_resume();
    ioexp_interrupt_read(IOEXP8_GPIO);

    while (1) {

        while ((interrupt_flags.value == 0) && (PORTBbits.RB0 == 1));

        di();

        //usart_write_char('a');

        computer_parameters.intf = ioexp_interrupt_read(IOEXP8_INTF);
        //usart_write_hex(computer_parameters.intf);
        //usart_write_hex(ioexp_interrupt_read(IOEXP8_GPIO));
        interrupt_flags.value = (interrupt_flags.value & (INTERRUPT_1HZ | INTERRUPT_Z80 | INTERRUPT_USART | INTERRUPT_TMR | INTERRUPT_KBD)) | computer_parameters.intf;

        if (interrupt_flags.Z80 == 1) {
            //usart_write_char('b');
            computer_parameters.state = ((PORTA & 0b00110000) >> 3) | ((PORTB & 0b00000010) >> 1);
            computer_parameters.port = bus_address_low_read();
            computer_parameters.data = bus_data_read();
            computer_parameters.handler = NULL;

            //usart_write_hex(computer_parameters.state);

            computer_handler();

            interrupt_response.RESUME = 1;
            if (computer_parameters.handler != NULL) (*computer_parameters.handler)();
            interrupt_flags.Z80 = 0;
            //usart_write_char('c');
        }

        else if (interrupt_flags.FP == 1) {
            front_panel_handler();
            interrupt_flags.FP = 0;

        }

        else if (interrupt_flags.KBD == 1) {
            computer_char_buffer = keyboard_read();

            if (computer_char_buffer != 0x00) {
                interrupt_response.THROW_INT = 1;
            } else {
                interrupt_flags.KBD = 0;
            }
            
        }

        else if ((interrupt_flags.value & (INTERRUPT_TMR | INTERRUPT_USART | INTERRUPT_1HZ | INTERRUPT_CLK)) != 0) {
            interrupt_response.THROW_INT = 1;
        }

        
        if (interrupt_response.BUS_RELEASE == 1) {
            interrupt_response.BUS_RELEASE = 0;
            z80_bus_release();
        }
        if (interrupt_response.RESUME == 1) {
            interrupt_response.RESUME = 0;
            z80_resume();
        }

        ioexp_interrupt_read(IOEXP8_GPIO);

        if (interrupt_response.THROW_INT == 1) {
            interrupt_response.THROW_INT = 0;
            interrupt_response.WAIT_ACK = 1;
            z80_int_assert();
        }

        //usart_write_char('z');
        
        ei();
    }
}