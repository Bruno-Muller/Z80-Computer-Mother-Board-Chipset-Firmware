#include "sdcard.h"

const unsigned int indexes[] = {0,64,128,192,256,320,384,448,512};

unsigned char sdcard_init() {

    unsigned char response, i;

    // clock pulses
    for (i = 0; i<10; i++) {
        spi_write(0xFF);
    }

    //usart_write_string("GO_IDLE_STATE: ");
    sdcard_select();
    sdcard_send_command(GO_IDLE_STATE, 0, 0x95);
    response = spi_write(0xFF);
    sdcard_unselect();
    if (response != 0x01) goto error;

    //usart_write_string("\r\nSEND_IF_COND: ");
    sdcard_select();
    sdcard_send_command(SEND_IF_COND, 0x01AA, 0x87);
    for (i=0; i<5; i++) {
        response = spi_write(0xFF);
    }
    sdcard_unselect();

    i = 0;
    do {
        //usart_write_string("\r\nAPP_CMD: ");
        __delay_ms(10);
        sdcard_select();
        sdcard_send_command(APP_CMD, 0,0);
        response = spi_write(0xFF);
        sdcard_unselect();
        __delay_ms(10);
        //usart_write_string("\r\nSD_SEND_OP_COND: ");
        sdcard_select();
        sdcard_send_command(SD_SEND_OP_COND, 0x40000000,0);
        response = spi_write(0xFF);
        sdcard_unselect();
    } while ((response != 0) && ++i<254);
    if (response != 0x0) goto error;

    //usart_write_string("\r\nREAD_OCR: ");
    sdcard_select();
    sdcard_send_command(READ_OCR, 0, 0);
    for (i=0; i<5; i++) {
        response = spi_write(0xFF);
    }
    sdcard_unselect();

    return SDCARD_SUCCESS;

error:
    sdcard_unselect();
    return SDCARD_INIT_FAILURE;
}

void sdcard_read(unsigned int memory_address, unsigned long block_address) {

    unsigned char response;
    unsigned int i, j;

    for (i = 0; i < 8; i++) {

        sdcard_select();
        sdcard_send_command(READ_SINGLE_BLOCK, block_address, 0);

        do {
            response = spi_write(0xFF);
        } while (response != 254);

        for (j = 0; j < indexes[i]; j++) {
            SSPBUF = 0xFF;
            while (!BF);
        }
        for (j = 0; j < 64; j++) {
            SSPBUF = 0xFF;
            while (!BF);
            buffer[j] = SSPBUF;
        }

        for (j = indexes[i + 1]; j < 512; j++) {
            SSPBUF = 0xFF;
            while (!BF);
        }
        do {
            SSPBUF = 0xFF;
            while (!BF);
            response = SSPBUF;
        } while (response != 255);

        sdcard_unselect();

        for (j = 0; j < 64; j++) {
            memory_quick_write(memory_address, buffer[j]);
            memory_address++;
        }


    }
}

/*
void sdcard_write(unsigned int memory_address, unsigned long block_address) {

    unsigned char response;
    unsigned int i;

    //usart_write_string("\r\nWRITE_SINGLE_BLOCK: ");
    sdcard_select();
    sdcard_send_command(WRITE_SINGLE_BLOCK, block_address, 0);

    do {
        response = soft_spi_write(0xFF);
    } while (response != 0x00);

    soft_spi_write(0xFE);

    for (i =0; i<512; i++) {
        soft_spi_write(memory_read(memory_address));
        memory_address++;
    }

    soft_spi_write(0xFF);
    soft_spi_write(0xFF);

    do {
        response = soft_spi_write(0xFF);
    } while (response != 255);

    sdcard_unselect();
}
*/

void sdcard_send_command(unsigned char cmd, unsigned long arg, unsigned char crc) {
    SSPBUF = 0xFF;
    while (!BF);

    SSPBUF = cmd | 0x40;
    while (!BF);
    SSPBUF = arg >> 24;
    while (!BF);
    SSPBUF = arg >> 16;
    while (!BF);
    SSPBUF = arg >> 8;
    while (!BF);
    SSPBUF = arg;
    while (!BF);
    SSPBUF = crc;
    while (!BF);

    SSPBUF = 0xFF;
    while (!BF);
}