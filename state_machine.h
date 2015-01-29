/* 
 * File:   state_machine.h
 * Author: Bruno
 *
 * Created on 26 décembre 2014, 16:31
 */

#ifndef STATE_MACHINE_H
#define	STATE_MACHINE_H

#include <xc.h>

#include "event.h"

#define PORT_USART      0x00
#define PORT_TIMER      0x01
#define PORT_SDCARD     0x02
#define PORT_EEPROM     0x03
#define PORT_RESTART    0x04

// Finite State Machine
// Allowed states
#define STATE_INT_ACK           0x06
#define STATE_WRITE             0x05
#define STATE_READ              0x03
//Forbidden states
#define STATE_M1_READ_AND_WRITE 0x00
#define STATE_M1_WRITE          0x04
#define STATE_M1_READ           0x02
#define STATE_READ_AND_WRITE    0x01
#define STATE_READ_OR_WRITE     0x07

unsigned char state_machine_get_event(unsigned char port, unsigned char data);

#endif	/* STATE_MACHINE_H */

