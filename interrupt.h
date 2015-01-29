/* 
 * File:   interrupt.h
 * Author: Bruno
 *
 * Created on 26 décembre 2014, 16:50
 */

#ifndef INTERRUPT_H
#define	INTERRUPT_H

#include <xc.h>

#include "z80.h"

#define INTERRUPT_VECTOR_USART  0x00
#define INTERRUPT_VECTOR_TIMER  0x02

#define INTERRUPT_NONE          0x00
#define INTERRUPT_OCCURED       0x01

#define INTERRUPT_Z80           0x01
#define INTERRUPT_FRONT_PANEL   0x02

volatile unsigned char interrupt_occured = INTERRUPT_NONE;

#define interrupt_wait()    while (interrupt_occured != INTERRUPT_OCCURED)
#define interrupt_ack()     interrupt_occured = INTERRUPT_NONE

#endif	/* INTERRUPT_H */

