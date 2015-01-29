/* 
 * File:   eeprom.h
 * Author: Bruno
 *
 * Created on 19 décembre 2014, 20:25
 */

#ifndef EEPROM_H
#define	EEPROM_H

#include <xc.h>

#include "conf.h"
#include "decoder.h"
#include "spi.h"

#define EEPROM_READ             0b00000011
#define EEPROM_WRITE            0b00000010
#define EEPROM_WRITE_DISABLE    0b00000100
#define EEPROM_WRITE_ENABLE     0b00000110
#define EEPROM_READ_STATUS      0b00000101
#define EEPROM_WRITE_STATUS     0b00000001

#define eeprom_select()      decoder_select(DECODER_SELECT_EEPROM)
#define eeprom_unselect()    decoder_unselect()

#define EEPROM_PAGE_SIZE    32
#define BIOS_MAX_SIZE       0x1000

void eeprom_read(unsigned int address, unsigned char *buffer);
void eeprom_write_page(unsigned int address, unsigned char *buffer);
void eeprom_wait_write_cycle();

#endif	/* EEPROM_H */

