#include "computer.h"

void (*clock_function_pointer[])() = {
    &computer_clock_get_datetime,   // COMPUTER_CLOCK_GET_DATETIME 0x00
    &computer_clock_set_datetime,   // COMPUTER_CLOCK_SET_DATETIME 0x01
    &computer_clock_int_enable,     // COMPUTER_CLOCK_INT_ENABLE   0x02
    &computer_clock_int_disable     // COMPUTER_CLOCK_INT_DISABLE  0x03
};

void (*read_function_pointer[])() = {
    &computer_usart_read,    // PORT_USART      0x00
    &computer_error,         // PORT_TIMER      0x01
    &computer_error,         // PORT_SDCARD     0x02
    &computer_error,         // PORT_EEPROM     0x03
    &computer_error,         // PORT_RESTART    0x04
    &computer_keyboard_read, // PORT_KEYBOARD   0x05
    &computer_error,         // PORT_VGA        0x06
    &computer_error          // PORT_CLOCK      0x07
};

void (*write_function_pointer[])() = {
    &computer_usart_write,      // PORT_USART      0x00
    &computer_timer_handler,    // PORT_TIMER      0x01
    &computer_sdcard_handler,   // PORT_SDCARD     0x02
    &computer_flash_bios,       // PORT_EEPROM     0x03
    &computer_restart,          // PORT_RESTART    0x04
    &computer_error,            // PORT_KEYBOARD   0x05
    &computer_vga_write,        // PORT_VGA        0x06
    &computer_clock_handler     // PORT_CLOCK      0x07
};

void (*state_function_pointer[])() = {
        &computer_error,            // Z80_M1_READ_AND_WRITE 0x00
        &computer_error,            // Z80_READ_AND_WRITE    0x01
        &computer_error,            // Z80_M1_READ           0x02
        &computer_read_handler,     // Z80_READ              0x03
        &computer_error,            // Z80_M1_WRITE          0x04
        &computer_write_handler,    // Z80_WRITE             0x05
        &z80_interrupt_acknowledge, // Z80_INT_ACK           0x06
        &computer_error             // Z80_READ_OR_WRITE     0x07
};

ComputerParameters computer_parameters;
volatile unsigned char computer_char_buffer;
unsigned char buffer[32];

volatile InterruptFlags interrupt_flags;
volatile InterruptResponse interrupt_response;

void computer_handler() {
    if (computer_parameters.state<STATE_TABLE_SIZE)
        (*state_function_pointer[computer_parameters.state])();
    else
        computer_error();
}

void computer_read_handler() {
    if ((computer_parameters.port<READ_TABLE_SIZE)) // && (interrupt_response.WAIT_ACK == 0))
        computer_parameters.handler = read_function_pointer[computer_parameters.port];
    else
        computer_error();
}

void computer_write_handler() {
    if ((computer_parameters.port<WRITE_TABLE_SIZE)) // && (interrupt_response.WAIT_ACK == 0))
        computer_parameters.handler = write_function_pointer[computer_parameters.port];
    else
        computer_error();
}

void computer_timer_handler() {
    if (computer_parameters.data == COMPUTER_TIMER_ENABLE) timer_enable();
    else timer_disable();
}

void computer_sdcard_handler() {
    if (computer_parameters.data == COMPUTER_SDCARD_READ) computer_sdcard_read_sector();
    else computer_sdcard_init();
}

void computer_clock_handler() {
    if (computer_parameters.data<CLOCK_TABLE_SIZE)
        (*clock_function_pointer[computer_parameters.data])();
    else
        computer_error();
}

void computer_clock_int_enable() {
    unsigned temp = ioexp_interrupt_read(IOEXP8_GPINTEN);
    temp |= INTERRUPT_1HZ;
    ioexp_interrupt_write(IOEXP8_GPINTEN, temp);
}

void computer_clock_int_disable() {
    unsigned temp = ioexp_interrupt_read(IOEXP8_GPINTEN);
    temp &= ~INTERRUPT_1HZ;
    ioexp_interrupt_write(IOEXP8_GPINTEN, temp);
}

void computer_error() {
    usart_write_string("\r\nBlack Screen of Death !\r\nstatus:");
    usart_write_hex(interrupt_flags.value);
    usart_write_string(" intf:");
    usart_write_hex(computer_parameters.intf);
    usart_write_string(" response:");
    usart_write_hex(interrupt_response.value);
    usart_write_string(" state:");
    usart_write_hex(computer_parameters.state);
    usart_write_string(" port:");
    usart_write_hex(computer_parameters.port);
    usart_write_string(" data:");
    usart_write_hex(computer_parameters.data);
    while(1);
}

void computer_keyboard_read() {
    ioexp_data_write(IOEXP8_GPIO, computer_char_buffer);
    ioexp_data_write(IOEXP8_IODIR, IOEXP_OUTPUTS);

    ioexp_data_write(IOEXP8_IODIR, IOEXP_INPUTS);
}

void computer_usart_read() {
    ioexp_data_write(IOEXP8_GPIO, computer_char_buffer);
    ioexp_data_write(IOEXP8_IODIR, IOEXP_OUTPUTS);

    ioexp_data_write(IOEXP8_IODIR, IOEXP_INPUTS);
}

void computer_usart_write() {
    usart_write_char(computer_parameters.data);
    vga_write_char(computer_parameters.data);
}

void computer_vga_write() {
    usart_write_char(computer_parameters.data);
    vga_write_char(computer_parameters.data);
}

void computer_restart() {
    usart_write_string("\r\nPress restart button");
    while (1);
}

void computer_sdcard_init() {
    unsigned char ret = sdcard_init();

    z80_bus_request_from_wait_state();
    computer_memory_active();

    memory_write_prelude();

    memory_write(PARAM_ADDRESS_RETURN, ret);

    memory_write_postlude();

    computer_memory_idle();

    interrupt_response.RESUME = 0;
    interrupt_response.BUS_RELEASE = 1;
}

void computer_sdcard_read_sector() {
    Integer memory_address;
    Long sector_address;

    z80_bus_request_from_wait_state();
    computer_memory_active();

    memory_read_prelude();

    memory_address.bytes[0] = memory_read(PARAM_ADDRESS_INT+0);
    memory_address.bytes[1] = memory_read(PARAM_ADDRESS_INT+1);
    sector_address.bytes[0] = memory_read(PARAM_ADDRESS_LONG+0);
    sector_address.bytes[1] = memory_read(PARAM_ADDRESS_LONG+1);
    sector_address.bytes[2] = memory_read(PARAM_ADDRESS_LONG+2);
    sector_address.bytes[3] = memory_read(PARAM_ADDRESS_LONG+3);

    memory_write_prelude();

    sdcard_read(memory_address.value, sector_address.value);

    memory_write_postlude();

    computer_memory_idle();

    interrupt_response.RESUME = 0;
    interrupt_response.BUS_RELEASE = 1;
}

void computer_clock_set_datetime() {
    unsigned char i;
    unsigned char src = PARAM_ADDRESS_CLOCK;

    // Read date/time from RAM
    z80_bus_request_from_wait_state();
    computer_memory_active();

    memory_read_prelude();

    for (i=0; i<PARAM_SIZE_CLOCK; i++, src++) buffer[i] = memory_read(src);

    memory_read_postlude();

    // Write date/time from RTC
    SSPCONbits.SSPEN = 0;
    SSPSTATbits.CKE = 0;
    SSPCONbits.SSPEN = 1;
    clock_select();
    spi_write(0x80);
    for (i=0; i<PARAM_SIZE_CLOCK; i++) spi_write(buffer[i]);
    clock_unselect();
    SSPCONbits.SSPEN = 0;
    SSPSTATbits.CKE = 1;
    SSPCONbits.SSPEN = 1;

    computer_memory_idle();

    interrupt_response.RESUME = 0;
    interrupt_response.BUS_RELEASE = 1;
}

void computer_clock_get_datetime() {
    unsigned char i;
    unsigned char dest = PARAM_ADDRESS_CLOCK;

    // Read date/time from RTC
    SSPCONbits.SSPEN = 0;
    SSPSTATbits.CKE = 0;
    SSPCONbits.SSPEN = 1;
    clock_select();
    spi_write(0x00);
    for (i=0; i<PARAM_SIZE_CLOCK; i++) buffer[i] = spi_read();
    clock_unselect();
    SSPCONbits.SSPEN = 0;
    SSPSTATbits.CKE = 1;
    SSPCONbits.SSPEN = 1;

    // Write date/time into RAM
    z80_bus_request_from_wait_state();
    computer_memory_active();

    memory_write_prelude();

    for (i=0; i<PARAM_SIZE_CLOCK; i++, dest++) memory_write(dest, buffer[i]);

    memory_write_postlude();

    computer_memory_idle();
    
    interrupt_response.RESUME = 0;
    interrupt_response.BUS_RELEASE = 1;
}

void computer_flash_bios() {
    Integer memory_address;

    z80_bus_request_from_wait_state();
    computer_memory_active();

    unsigned int eeprom_address = 0;

    memory_read_prelude();

    memory_address.bytes[0] = memory_read(PARAM_ADDRESS_INT+0);
    memory_address.bytes[1] = memory_read(PARAM_ADDRESS_INT+1);

    while (eeprom_address < BIOS_MAX_SIZE) {
        // read page from memory
        unsigned char i;
        for (i = 0; i < EEPROM_PAGE_SIZE; i++) {
            buffer[i] = memory_read(memory_address.value);
            memory_address.value++;
        }

        //write page to eeprom
        eeprom_write_page(eeprom_address, buffer);
        eeprom_wait_write_cycle();
        eeprom_address += EEPROM_PAGE_SIZE;
    }

    memory_read_postlude();

    computer_memory_idle();

    interrupt_response.RESUME = 0;
    interrupt_response.BUS_RELEASE = 1;
}