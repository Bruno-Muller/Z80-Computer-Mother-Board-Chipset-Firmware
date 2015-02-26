/* 
 * File:   front_panel.h
 * Author: Bruno
 *
 * Created on 24 janvier 2015, 20:40
 */

#ifndef FRONT_PANEL_H
#define	FRONT_PANEL_H

#include <xc.h>

#include "computer.h"
#include "decoder.h"
#include "interrupt.h"
#include "memory.h"
#include "spi.h"
#include "usart.h"
#include "z80.h"

#define FP_RUN          0x10
#define FP_PROGRAM      0x00
#define FP_EXAMINE      0x08
#define FP_EXAMINE_NEXT 0x04
#define FP_DEPOSIT      0x02
#define FP_DEPOSIT_NEXT 0x01
#define FP_CLEAR        0x80
#define FP_RESET        0x50
#define FP_HALT         0x30

void front_panel_handler();

#endif	/* FRONT_PANEL_H */

