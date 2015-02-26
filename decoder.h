/* 
 * File:   decoder.h
 * Author: Bruno
 *
 * Created on 19 décembre 2014, 20:50
 */

#ifndef DECODER_H
#define	DECODER_H

#include <xc.h>

#include "conf.h"

#define DECODER_SELECT_0 0
#define DECODER_SELECT_1 1
#define DECODER_SELECT_2 2
#define DECODER_SELECT_3 3
#define DECODER_SELECT_4 4
#define DECODER_SELECT_5 5
#define DECODER_SELECT_6 6
#define DECODER_SELECT_7 7

#define DECODER_SELECT_DATA         DECODER_SELECT_0
#define DECODER_SELECT_ADDRESS      DECODER_SELECT_1
#define DECODER_SELECT_EEPROM       DECODER_SELECT_2
#define DECODER_SELECT_CLOCK        DECODER_SELECT_3
#define DECODER_SELECT_INTERRUPT    DECODER_SELECT_4
#define DECODER_SELECT_FRONT_PANEL  DECODER_SELECT_5
#define DECODER_SELECT_KEYBOARD     DECODER_SELECT_6
#define DECODER_SELECT_VGA          DECODER_SELECT_7

void decoder_select(unsigned char address);
void decoder_unselect();

#endif	/* DECODER_H */

