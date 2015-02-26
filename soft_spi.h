/* 
 * File:   soft_spi.h
 * Author: Bruno
 *
 * Created on 3 février 2015, 14:12
 */

#ifndef SOFT_SPI_H
#define	SOFT_SPI_H

#include <xc.h>
#include "conf.h"

void soft_spi_init();
unsigned char soft_spi_write(unsigned char data);
unsigned char soft_spi_read();

#endif	/* SOFT_SPI_H */

