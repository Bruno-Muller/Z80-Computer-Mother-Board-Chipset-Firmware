/* 
 * File:   sdcard.h
 * Author: Bruno
 *
 * Created on 19 décembre 2014, 20:25
 */

#ifndef SDCARD_H
#define	SDCARD_H

#include <xc.h>

#include "decoder.h"
#include "memory.h"
#include "spi.h"
#include "usart.h"

#define sdcard_select()      decoder_select(DECODER_SELECT_SDCARD)
#define sdcard_unselect()    decoder_unselect()

// Commandes
#define GO_IDLE_STATE           0
#define SEND_IF_COND            8
#define SET_BLOCK_LEN           16
#define READ_SINGLE_BLOCK       17
#define WRITE_SINGLE_BLOCK      24
#define ERASE_BLOCK_START_ADDR  32
#define ERASE_BLOCK_END_ADDR    33
#define SD_SEND_OP_COND         41
#define APP_CMD                 55
#define READ_OCR                58
#define CRC_ON_OFF              59

#define SDCARD_SUCCESS          0x00
#define SDCARD_INIT_FAILURE     0x01

#define ON                      1
#define OFF                     0

unsigned char sdcard_init();
void sdcard_send_command(unsigned char command, unsigned long arg, unsigned char crc);
void sdcard_read(unsigned int memory_address, unsigned long block_address);
//void sdcard_write(unsigned int memory_address, unsigned long block_address);

#endif	/* SDCARD_H */

