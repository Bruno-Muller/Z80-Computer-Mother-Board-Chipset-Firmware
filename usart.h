/* 
 * File:   usart.h
 * Author: Bruno
 *
 * Created on 18 décembre 2014, 23:33
 */

#ifndef USART_H
#define	USART_H

#include <xc.h>

#define usart_interrupt_enable() while (RCIF) { unsigned char temp = RCREG; }; RCIE = 1
#define usart_interrupt_disable() RCIE = 0

void usart_init();
unsigned char usart_read();
void usart_write_char(unsigned char data);
void usart_write_hex(unsigned char hex);
void usart_write_string(const char* string);
void usart_write_unsigned_char(unsigned char data);
void usart_write_unsigned_int(unsigned int data);

#endif	/* USART_H */

