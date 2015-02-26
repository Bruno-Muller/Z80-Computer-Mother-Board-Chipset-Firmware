/* 
 * File:   computer.h
 * Author: Bruno
 *
 * Created on 22 décembre 2014, 21:49
 */

#ifndef COMPUTER_H
#define	COMPUTER_H

#include <xc.h>

#include "conf.h"
#include "memory.h"
#include "z80.h"

#define COMPUTER_TIMER_ENABLE       0x00
#define COMPUTER_TIMER_DISABLE      ~COMPUTER_TIMER_ENABLE
#define COMPUTER_SDCARD_READ        0x00
#define COMPUTER_SDCARD_INIT        ~COMPUTER_SDCARD_READ
#define COMPUTER_CLOCK_GET_DATETIME 0x00
#define COMPUTER_CLOCK_SET_DATETIME ~COMPUTER_CLOCK_GET_DATETIME

#define PARAM_ADDRESS_INT       0x50
#define PARAM_ADDRESS_LONG      0x52
#define PARAM_ADDRESS_RETURN    0x56
#define PARAM_ADDRESS_CLOCK     0x57
#define PARAM_SIZE_CLOCK        7

typedef struct {
    unsigned char state;
    unsigned char port;
    unsigned char data;
    void (*handler)();
} ComputerParameters;

typedef union {
    unsigned int value;
    unsigned char bytes[2];
} Integer;

typedef union {
    unsigned long value;
    unsigned char bytes[4];
} Long;

void computer_clock_handler();
void computer_handler();
void computer_read_handler();
void computer_sdcard_handler();
void computer_timer_handler();
void computer_write_handler();

void computer_clock_set_datetime();
void computer_clock_get_datetime();
void computer_error();
void computer_flash_bios();
void computer_keyboard_read();
void computer_restart();
void computer_sdcard_init();
void computer_sdcard_read_sector();
void computer_usart_read();
void computer_usart_write();
void computer_vga_write();

#define computer_memory_active()    PORTA |= MEMORY_CONTROL_BITS; TRISA &= ~MEMORY_CONTROL_BITS
#define computer_memory_idle()      TRISA |= MEMORY_CONTROL_BITS

#define READ_TABLE_SIZE     0x08
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

#define WRITE_TABLE_SIZE    0x08
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

#define STATE_TABLE_SIZE    0x08
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


#endif	/* COMPUTER_H */

