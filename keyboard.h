/* 
 * File:   keyboard.h
 * Author: Bruno
 *
 * Created on 15 février 2015, 13:49
 */

#ifndef KEYBOARD_H
#define	KEYBOARD_H

#include <xc.h>

#include "decoder.h"
#include "spi.h"

#define keyboard_select()     decoder_select(DECODER_SELECT_KEYBOARD)
#define keyboard_unselect()   decoder_unselect()

unsigned char keyboard_read();

#endif	/* KEYBOARD_H */

