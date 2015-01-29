/* 
 * File:   bus.h
 * Author: Bruno
 *
 * Created on 22 décembre 2014, 22:21
 */

#ifndef BUS_H
#define	BUS_H

#include <xc.h>

#include "conf.h"
#include "ioexp.h"


#define bus_data_read() ioexp_data_read(IOEXP8_GPIO)
#define bus_address_low_read() ioexp_address_read(IOEXP16_GPIOA)

/*
unsigned char bus_data_read();
unsigned char bus_address_low_read();
*/
#endif	/* BUS_H */

