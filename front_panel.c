/* 
 * File:   front_panel.c
 * Author: Bruno
 *
 * Created on 24 janvier 2015, 20:40
 */

#include "front_panel.h"

void front_panel_handler() {
    static unsigned int address = 0;
    static unsigned char mode = FP_RUN;
    static unsigned char intmask;

    unsigned char status, low, high;

    decoder_select(DECODER_SELECT_FRONT_PANEL);
    status = spi_read();
    decoder_unselect();

    __delay_ms(1);

    decoder_select(DECODER_SELECT_FRONT_PANEL);
    low = spi_read();
    decoder_unselect();

    __delay_ms(1);

    decoder_select(DECODER_SELECT_FRONT_PANEL);
    high = spi_read();
    decoder_unselect();

    switch (status) {
        case FP_RUN:
            ioexp_interrupt_write(IOEXP8_GPINTEN, intmask);
            mode = FP_RUN;
            memory_operation_postlude();
            computer_memory_idle();
            z80_bus_release();
            break;
        case FP_PROGRAM:
            if (mode == FP_RUN) {
                intmask = ioexp_interrupt_read(IOEXP8_GPINTEN);
                ioexp_interrupt_write(IOEXP8_GPINTEN, INTERRUPT_FP);
                mode = FP_PROGRAM;
                z80_bus_request_from_wait_state();
                computer_memory_active();
            }
            break;
        case FP_EXAMINE:
            memory_unselect();
            memory_read_prelude();
            address = high<<8 | low;
            ioexp_address_write(IOEXP16_GPIOA, address);
            ioexp_address_write(IOEXP16_GPIOB, address>>8);
            memory_select();
            break;
        case FP_EXAMINE_NEXT:
            address++;
            memory_unselect();
            memory_read_prelude();
            ioexp_address_write(IOEXP16_GPIOA, address);
            ioexp_address_write(IOEXP16_GPIOB, address>>8);
            memory_select();
            break;
        case FP_DEPOSIT:
            memory_unselect();
            memory_write_prelude();
            ioexp_address_write(IOEXP16_GPIOA, address);
            ioexp_address_write(IOEXP16_GPIOB, address>>8);
            ioexp_data_write(IOEXP8_GPIO, low);
            memory_select();
            break;
        case FP_DEPOSIT_NEXT:
            address++;
            memory_unselect();
            memory_write_prelude();
            ioexp_address_write(IOEXP16_GPIOA, address);
            ioexp_address_write(IOEXP16_GPIOB, address>>8);
            ioexp_data_write(IOEXP8_GPIO, low);
            memory_select();
            break;
        case FP_CLEAR:
            memory_unselect();
            memory_write_prelude();
            ioexp_address_write(IOEXP16_GPIOA, address);
            ioexp_address_write(IOEXP16_GPIOB, address>>8);
            ioexp_data_write(IOEXP8_GPIO, 0);
            memory_select();
            break;
        case FP_RESET:

            z80_reset_assert();
            __delay_ms(1);
            z80_reset_disassert();
            break;
        case FP_HALT:
            break;
        //default:
            //usart_write_string("\r\nFRONT PANEL BAD STATUS: ");
            //usart_write_hex(status);
    }

}

