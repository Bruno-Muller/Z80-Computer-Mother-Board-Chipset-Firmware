/* 
 * File:   event.h
 * Author: Bruno
 *
 * Created on 26 décembre 2014, 16:07
 */

#ifndef EVENT_H
#define	EVENT_H

#include <xc.h>

#include "bus.h"
#include "computer.h"
#include "eeprom.h"
#include "sdcard.h"
#include "usart.h"
#include "z80.h"

union uInteger {
    unsigned int value;
    unsigned char bytes[2];
};
typedef union uInteger Integer;

union uLong {
    unsigned long value;
    unsigned char bytes[4];
};
typedef union uLong Long;

// Normal events
#define EVENT_INT_ACK               0x00
#define EVENT_TIMER_ENABLE          0x01
#define EVENT_TIMER_DISABLE         0x02
#define EVENT_WRITE_CHAR            0x03
#define EVENT_READ_CHAR             0x04
#define EVENT_RESTART               0x05
#define EVENT_FLASH_BIOS            0x06
#define EVENT_SDCARD_LOAD_SECTOR    0x07
#define EVENT_SDCARD_INIT           0x08
// Error events
#define EVENT_READ_AND_WRITE        0x88
#define EVENT_M1_READ_AND_WRITE     0x89
#define EVENT_M1_READ               0x8A
#define EVENT_M1_WRITE              0x8B
#define EVENT_UNEXPECTED_READ_PORT  0x8C
#define EVENT_UNEXPECTED_WRITE_PORT 0x8D
#define EVENT_READ_OR_WRITE         0x8E
#define EVENT_UNEXPECTED_STATE      0x8F

struct sEventParam {
    unsigned char port;
    unsigned char data;
};
typedef struct sEventParam EventParam;

void event_int_ack();
void event_timer_enable();
void event_timer_disable();
void event_write_char();
void event_read_char();
void event_restart();
void event_sdcard_load_sector();
void event_flash_bios();
void event_sdcard_init();

void event_error(unsigned char event);

void event_handler(unsigned char event, unsigned char port, unsigned char data);

#endif	/* EVENT_H */

