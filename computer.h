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

#define computer_memory_active()    PORTA |= MEMORY_CONTROL_BITS; TRISA &= ~MEMORY_CONTROL_BITS
#define computer_memory_idle()      TRISA |= MEMORY_CONTROL_BITS
/*
void computer_memory_write(unsigned int address, unsigned char data);
unsigned char computer_memory_read(unsigned int address);
*/
#endif	/* COMPUTER_H */

