/* 
 * File:   spi.h
 * Author: Bruno
 *
 * Created on 19 décembre 2014, 20:16
 */

#ifndef SPI_H
#define	SPI_H

#include <xc.h>

void spi_init();

unsigned char spi_read();
unsigned char spi_write(unsigned char data);

#endif	/* SPI_H */

