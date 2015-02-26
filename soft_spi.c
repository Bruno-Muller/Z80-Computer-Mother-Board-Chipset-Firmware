#include "soft_spi.h"

void soft_spi_init() {
    SOFT_SPI_MOSI = 1;
    SOFT_SPI_SCK = 0;

    // Init PORTC
    TRISB6 = 1; // MISO
    TRISB7 = 0; // MOSI
    TRISC2 = 0; // SCK
}

unsigned char soft_spi_write(unsigned char data) {
    unsigned char i = 0;
    for (i = 0; i<8; i++) {

        if ((data & 0x80) == 0x80) {
            SOFT_SPI_MOSI = 1;
        }
        else {
            SOFT_SPI_MOSI = 0;
        }

        __delay_us(1);

        SOFT_SPI_SCK = 1;

        __delay_us(1);

        data <<= 1;

        if (SOFT_SPI_MISO == 1) {
            data |= 0x01;
        }

        SOFT_SPI_SCK = 0;
    }

    SOFT_SPI_MOSI = 1;

    return data;
}

unsigned char soft_spi_read() {
    unsigned char data = 0;

    SOFT_SPI_MOSI = 1;

    unsigned char i = 0;
    for (i = 0; i<8; i++) {

        __delay_us(1);

        SOFT_SPI_SCK = 1;

        __delay_us(1);

        data <<= 1;

        if (SOFT_SPI_MISO == 1) {
            data |= 0x01;
        }

        SOFT_SPI_SCK = 0;


    }

    return data;
}