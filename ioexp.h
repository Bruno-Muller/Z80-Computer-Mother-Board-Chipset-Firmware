/* 
 * File:   ioexp.h
 * Author: Bruno
 *
 * Created on 19 décembre 2014, 20:38
 */

#ifndef IOEXP_H
#define	IOEXP_H

#include <xc.h>

#include "decoder.h"
#include "spi.h"

#define IOEXP8_IODIR   0x00
#define IOEXP8_IPOL    0x01
#define IOEXP8_GPINTEN 0x02
#define IOEXP8_DEFVAL  0x03
#define IOEXP8_INTCON  0x04
#define IOEXP8_IOCON   0x05
#define IOEXP8_GPPU    0x06
#define IOEXP8_INTF    0x07
#define IOEXP8_INTCAP  0x08
#define IOEXP8_GPIO    0x09
#define IOEXP8_OLAT    0x0A

#define IOEXP16_IODIRA      0x00
#define IOEXP16_IODIRB      0x01
#define IOEXP16_IPOLA       0x02
#define IOEXP16_IPOLB       0x03
#define IOEXP16_GPINTENA    0x04
#define IOEXP16_GPINTENB    0x05
#define IOEXP16_DEFVALA     0x06
#define IOEXP16_DEFVALB     0x07
#define IOEXP16_INTCONA     0x08
#define IOEXP16_INTCONB     0x09
#define IOEXP16_IOCON       0x0A
#define IOEXP16_IOCONA      0x0A
#define IOEXP16_IOCONB      0x0B
#define IOEXP16_GPPUA       0x0C
#define IOEXP16_GPPUB       0x0D
#define IOEXP16_INTFA       0x0E
#define IOEXP16_INTFB       0x0F
#define IOEXP16_INTCAPA     0x10
#define IOEXP16_INTCAPB     0x11
#define IOEXP16_GPIOA       0x12
#define IOEXP16_GPIOB       0x13
#define IOEXP16_OLATA       0x14
#define IOEXP16_OLTAB       0x15

#define IOEXP_OUTPUTS 0x00
#define IOEXP_INPUTS  0xFF
#define IOEXP_CONF    0x20 // SEQOP disable; HAEN disable; ODR disable; INTPOL Active-low
#define IOEXP_INTERRUPT_IODIR   0b10011111
#define IOEXP_INTERRUPT_MASK    0b00001011
#define IOEXP_INTERRUPT_INTCON  0x00

#define IOEXP_WRITE   0x40
#define IOEXP_READ    0x41

#define ioexp_data_select()     decoder_select(DECODER_SELECT_DATA)
#define ioexp_address_select()  decoder_select(DECODER_SELECT_ADDRESS)
#define ioexp_interrupt_select() decoder_select(DECODER_SELECT_INTERRUPT)
#define ioexp_unselect()        decoder_unselect()

void ioexp_init();

unsigned char ioexp_address_read(unsigned char address);
void ioexp_address_write(unsigned char address, unsigned char data);

unsigned char ioexp_data_read(unsigned char address);
void ioexp_data_write(unsigned char address, unsigned char data);

void ioexp_interrupt_write(unsigned char address, unsigned char data);
unsigned char ioexp_interrupt_read(unsigned char address);

unsigned char ioexp_read(unsigned char address);
void ioexp_write(unsigned char address, unsigned char data);



#endif	/* IOEXP_H */

