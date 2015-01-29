#include "event.h"

EventParam event_param;

void event_handler(unsigned char event, unsigned char port, unsigned char data) {

    event_param.port = port;
    event_param.data = data;

    /* Normal events :
     * EVENT_INT_ACK               0x00
     * EVENT_TIMER_ENABLE          0x01
     * EVENT_TIMER_DISABLE         0x02
     * EVENT_WRITE_CHAR            0x03
     * EVENT_READ_CHAR             0x04
     * EVENT_RESTART               0x05
     * EVENT_FLASH_BIOS            0x06
     * EVENT_SDCARD_LOAD_SECTOR    0x07
     * EVENT_SDCARD_INIT           0x08
     * Error events :
     * EVENT_READ_AND_WRITE        0x80
     * EVENT_M1_READ_AND_WRITE     0x81
     * EVENT_M1_READ               0x82
     * EVENT_M1_WRITE              0x83
     * EVENT_UNEXPECTED_READ_PORT  0x84
     * EVENT_UNEXPECTED_WRITE_PORT 0x85
     * EVENT_READ_OR_WRITE         0x86
     * EVENT_UNEXPECTED_STATE      0x87
     */
    void (*event_function_pointer[])() = {
        &event_int_ack,
        &event_timer_enable,
        &event_timer_disable,
        &event_write_char,
        &event_read_char,
        &event_restart,
        &event_flash_bios,
        &event_sdcard_load_sector,
        &event_sdcard_init
    };

    if ((event & 0x80) == 0x00) {
        (*event_function_pointer[event])();
    }
    else {
        event_error(event);
    }

}

void event_sdcard_init() {
    unsigned char ret = sdcard_init();

    z80_bus_request_from_wait_state();
    computer_memory_active();

    memory_quick_write_prelude();

    memory_quick_write(0x56, ret);

    memory_quick_write_postlude();

    computer_memory_idle();
    z80_bus_release();
}

void event_int_ack() {
    z80_int_disassert();
    ioexp_data_write(IOEXP8_GPIO, z80_interrupt_vector);
    ioexp_data_write(IOEXP8_IODIR, IOEXP_OUTPUTS);

    ioexp_data_write(IOEXP8_IODIR, IOEXP_INPUTS);
}

void event_timer_enable() {
    TMR1H = ((15535 + 80) & 0xFF00) >> 8;
    TMR1L = ((15535 + 80) & 0x00FF);
    TMR1ON = 1;
    TMR1IE = 1;
}

void event_timer_disable() {
    TMR1IE = 0;
    TMR1ON = 0;
}

void event_write_char() {
    usart_write_char(event_param.data);
}

void event_read_char() {
    ioexp_data_write(IOEXP8_GPIO, z80_char_buffer);
    ioexp_data_write(IOEXP8_IODIR, IOEXP_OUTPUTS);

    ioexp_data_write(IOEXP8_IODIR, IOEXP_INPUTS);
}

void event_restart() {
    usart_write_string("\r\nPress restart button");
    while (1);
}

void event_sdcard_load_sector() {
    Integer memory_address;
    Long sector_address;

    z80_bus_request_from_wait_state();
    computer_memory_active();

    memory_quick_read_prelude();

    memory_address.bytes[0] = memory_quick_read(0x50);
    memory_address.bytes[1] = memory_quick_read(0x51);
    sector_address.bytes[0] = memory_quick_read(0x52);
    sector_address.bytes[1] = memory_quick_read(0x53);
    sector_address.bytes[2] = memory_quick_read(0x54);
    sector_address.bytes[3] = memory_quick_read(0x55);

    memory_quick_write_prelude();

    sdcard_read(memory_address.value, sector_address.value);

    memory_quick_write_postlude();

    computer_memory_idle();
    z80_bus_release();
}

void event_flash_bios() {
    Integer memory_address;

    z80_bus_request_from_wait_state();
    computer_memory_active();

    unsigned int eeprom_address = 0;
    
    memory_quick_read_prelude();

    memory_address.bytes[0] = memory_quick_read(0x50);
    memory_address.bytes[1] = memory_quick_read(0x51);

    while (eeprom_address < BIOS_MAX_SIZE) {
        // read page from memory
        unsigned char i;
        for (i = 0; i < EEPROM_PAGE_SIZE; i++) {
            buffer[i] = memory_quick_read(memory_address.value);
            memory_address.value++;
        }

        //write page to eeprom
        eeprom_write_page(eeprom_address, buffer);
        eeprom_wait_write_cycle();
        eeprom_address += EEPROM_PAGE_SIZE;
    }

    memory_quick_read_postlude();

    computer_memory_idle();
    z80_bus_release();
}

void event_error(unsigned char event) {
     //usart_write_string("\r\nCHIPSET ERROR: ");
     //usart_write_hex(event);
    switch (event) {
        case (EVENT_READ_AND_WRITE):
            usart_write_string("\r\nHARDWARE ERROR : IO READ AND WRITE");
            break;
        case (EVENT_M1_READ_AND_WRITE):
            usart_write_string("\r\nHARDWARE ERROR : M1 READ AND WRITE");
            break;
        case (EVENT_M1_READ):
            usart_write_string("\r\nHARDWARE ERROR : M1 READ");
            break;
        case (EVENT_M1_WRITE):
            usart_write_string("\r\nHARDWARE ERROR : M1 READ");
            break;
        case (EVENT_UNEXPECTED_READ_PORT):
            usart_write_string("\r\nBIOS ERROR : Unexpected Read Port: ");
            usart_write_unsigned_char(event_param.port);
            break;
        case (EVENT_UNEXPECTED_WRITE_PORT):
            usart_write_string("\r\nBIOS ERROR : Unexpected Write Port: ");
            usart_write_unsigned_char(event_param.port);
            break;
        case (EVENT_READ_OR_WRITE):
            usart_write_string("\r\nnBIOS ERROR : IO READ OR WRITE");
            break;
        case (EVENT_UNEXPECTED_STATE):
            usart_write_string("\r\nBIOS ERROR : Unknown State");
            break;
        default:
            usart_write_string("\r\nBIOS ERROR : Unknown Error :P");
    }
}