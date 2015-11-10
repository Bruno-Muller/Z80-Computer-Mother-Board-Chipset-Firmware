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
#include "interrupt.h"
#include "memory.h"
#include "z80.h"

#define COMPUTER_TIMER_ENABLE       0x00
#define COMPUTER_TIMER_DISABLE      0x01
#define COMPUTER_SDCARD_READ        0x00
#define COMPUTER_SDCARD_INIT        0x01
#define COMPUTER_CLOCK_GET_DATETIME 0x00
#define COMPUTER_CLOCK_SET_DATETIME 0x01
#define COMPUTER_CLOCK_INT_ENABLE   0x02
#define COMPUTER_CLOCK_INT_DISABLE  0x03

#define PARAM_ADDRESS_INT       0x50
#define PARAM_ADDRESS_LONG      0x52
#define PARAM_ADDRESS_RETURN    0x56
#define PARAM_ADDRESS_CLOCK     0x57
#define PARAM_SIZE_CLOCK        7

typedef union {
    struct {
        unsigned Z80    :1;
        unsigned FP     :1;
        unsigned CLK    :1;
        unsigned KBD    :1;
        unsigned HZ     :1;
        unsigned USART  :1;
        unsigned TMR    :1;
    };
    unsigned char value;
} InterruptFlags;

typedef union {
    struct {
        unsigned RESUME         :1;
        unsigned BUS_RELEASE    :1;
        unsigned THROW_INT      :1;
        unsigned WAIT_ACK       :1;
    };
    unsigned char value;
} InterruptResponse;

typedef struct {
    unsigned char state;
    unsigned char port;
    unsigned char data;
    unsigned char intf;
    //unsigned char intcap;
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

void computer_clock_int_enable();
void computer_clock_int_disable();
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
extern void (*read_function_pointer[])();

#define WRITE_TABLE_SIZE    0x08
extern void (*write_function_pointer[])();

#define STATE_TABLE_SIZE    0x08
extern void (*state_function_pointer[])();

#define CLOCK_TABLE_SIZE    0x04
extern void (*clock_function_pointer[])();

extern ComputerParameters computer_parameters;
extern volatile unsigned char computer_char_buffer;
extern unsigned char buffer[32];

extern volatile InterruptFlags interrupt_flags;
extern volatile InterruptResponse interrupt_response;

#endif	/* COMPUTER_H */

