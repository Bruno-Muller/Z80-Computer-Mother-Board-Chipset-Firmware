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

void memory_read_prelude();
void memory_write_prelude();
unsigned char memory_read(unsigned int address);
void memory_write(unsigned int address, unsigned char data);
void memory_operation_postlude();

#define memory_read_postlude()    memory_operation_postlude()
#define memory_write_postlude()   memory_operation_postlude()

#endif	/* MEMORY_H */

