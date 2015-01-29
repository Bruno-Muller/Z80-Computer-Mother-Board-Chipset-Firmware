/* 
 * File:   z80.h
 * Author: Bruno
 *
 * Created on 22 d�cembre 2014, 20:08
 */

#ifndef Z80_H
#define	Z80_H

#include <xc.h>

#include "conf.h"

#define z80_bus_request()   Z80_BUSREQ = 0; while(Z80_BUSACK == 1)
#define z80_bus_request_from_wait_state()   Z80_BUSREQ = 0;  z80_resume(); while(Z80_BUSACK == 1)
#define z80_bus_release()   Z80_BUSREQ = 1

#define z80_interrupt_enable()  INTF = 0; INTE = 1
#define z80_interrupt_disable() INTE = 0

#define z80_reset_assert() Z80_RESET = 0
#define z80_reset_disassert()   Z80_RESET = 1

#define z80_int_assert()    Z80_INT = 0
#define z80_int_disassert() Z80_INT = 1

volatile unsigned char z80_interrupt_vector, z80_char_buffer;

void z80_resume(); // continue

#endif	/* Z80_H */

