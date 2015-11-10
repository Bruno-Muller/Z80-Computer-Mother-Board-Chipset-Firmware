/* 
 * File:   interrupt.h
 * Author: Bruno
 *
 * Created on 26 décembre 2014, 16:50
 */

#ifndef INTERRUPT_H
#define	INTERRUPT_H

#include <xc.h>

#include "computer.h"
#include "z80.h"

#define INTERRUPT_VECTOR_USART  0x00
#define INTERRUPT_VECTOR_TIMER  0x02
#define INTERRUPT_VECTOR_CLOCK  0x04

#define INTERRUPT_Z80           0x01
#define INTERRUPT_FP            0x02
#define INTERRUPT_CLK           0x04
#define INTERRUPT_KBD           0x08
#define INTERRUPT_1HZ           0x10
#define INTERRUPT_USART         0x20
#define INTERRUPT_TMR           0x40

#endif	/* INTERRUPT_H */

