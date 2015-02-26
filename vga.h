/* 
 * File:   vga.h
 * Author: Bruno
 *
 * Created on 14 février 2015, 17:55
 */

#ifndef VGA_H
#define	VGA_H

#include <xc.h>

#include "decoder.h"
#include "ioexp.h"
#include "spi.h"

#define VGA_BUSY_MASK   0x80

#define vga_select()     decoder_select(DECODER_SELECT_VGA)
#define vga_unselect()   decoder_unselect()

void vga_write_char(unsigned char data);

#endif	/* VGA_H */

