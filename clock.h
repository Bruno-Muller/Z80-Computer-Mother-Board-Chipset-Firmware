/* 
 * File:   clock.h
 * Author: Bruno
 *
 * Created on 21 février 2015, 18:37
 */

#ifndef CLOCK_H
#define	CLOCK_H

#include "decoder.h"
#include "spi.h"
#include "usart.h"

#define clock_select()      decoder_select(DECODER_SELECT_CLOCK)
#define clock_unselect()    decoder_unselect()

typedef struct {
	unsigned char sec;
	unsigned char min;
	unsigned char hours;
	unsigned char day;
	unsigned char date;
	unsigned char month;
	unsigned char year;
} DateTime;

void clock_handler();

#endif	/* CLOCK_H */

