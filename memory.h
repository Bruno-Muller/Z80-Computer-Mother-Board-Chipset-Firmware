/* 
 * File:   memory.h
 * Author: Bruno
 *
 * Created on 22 décembre 2014, 19:25
 */

#ifndef MEMORY_H
#define	MEMORY_H

#include "decoder.h"
#include "conf.h"
#include "ioexp.h"

#define memory_select()         MEMORY_SELECT_PORT = 0
#define memory_unselect()       MEMORY_SELECT_PORT = 1

#define memory_state_idle()     MEMORY_READ_PORT = 1; MEMORY_WRITE_PORT = 1
#define memory_state_read()     MEMORY_READ_PORT = 0; MEMORY_WRITE_PORT = 1
#define memory_state_write()    MEMORY_READ_PORT = 1; MEMORY_WRITE_PORT = 0

void memory_quick_read_prelude();
void memory_quick_write_prelude();
unsigned char memory_quick_read(unsigned int address);
void memory_quick_write(unsigned int address, unsigned char data);
void memory_quick_operation_postlude();

#define memory_quick_read_postlude()    memory_quick_operation_postlude()
#define memory_quick_write_postlude()   memory_quick_operation_postlude()
/*
void memory_write(unsigned int address, unsigned char data);
void memory_write_string(unsigned int address, const char* string);
unsigned char memory_read(unsigned int address);
*/
#endif	/* MEMORY_H */

