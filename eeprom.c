#include "eeprom.h"

void eeprom_read(unsigned int address, unsigned char *buffer) {
    eeprom_select();
    spi_write(EEPROM_READ);

    spi_write(address>>8);
    spi_write(address);

    unsigned char i;
    for (i=0; i<EEPROM_PAGE_SIZE; i++) {
        buffer[i] = spi_read();
    }
  
    eeprom_unselect();
}

void eeprom_write_page(unsigned int address, unsigned char *buffer) {
    eeprom_select();
    spi_write(EEPROM_WRITE_ENABLE);
    eeprom_unselect();

    eeprom_select();
    spi_write(EEPROM_WRITE);

    spi_write(address>>8);
    spi_write(address);

    unsigned char i;
    for (i=0; i<EEPROM_PAGE_SIZE; i++) {
        spi_write(buffer[i]);        
    }

    eeprom_unselect();
}

void eeprom_wait_write_cycle() {
    unsigned char status;
    do {
        eeprom_select();
        spi_write(EEPROM_READ_STATUS);
        status = spi_read();
        eeprom_unselect();
    } while (status & 0x3);
}