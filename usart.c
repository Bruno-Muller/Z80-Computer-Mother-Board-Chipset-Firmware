#include "usart.h"

void usart_init() {

    BRGH = 1;  // High Baud Rate : High speed
    SPBRG = 7; // Baud rate : 125k

    SYNC = 0; // Asynchronous mode
    SPEN = 1; // Serial port enabled
    
    TXEN = 1; // Transmit enabled
    CREN = 1; // Enables receiver
}

unsigned char usart_read() {
    while (RCIF == 0);
    return RCREG;
}

void usart_write_char(unsigned char data) {
    while (TXIF == 0);
    TXREG = data;
}

void usart_write_hex(unsigned char hex)
{
    usart_write_string("0x");
    unsigned char tmp = (hex>>4) & 0x0F;
    usart_write_char(tmp<0x0A?tmp+'0':tmp-0x0A+'A');
    tmp = hex & 0x0F;
    usart_write_char(tmp<0x0A?tmp+'0':tmp-0x0A+'A');
}

void usart_write_string(const char* string) {
    while (*string != 0) {
        usart_write_char(*string);
        string++;
    }
}

void usart_write_unsigned_char(unsigned char data) {
    if (data>99) usart_write_char(data/100 + '0');
    if (data>9) usart_write_char((data/10)%10 + '0');
    usart_write_char(data%10 + '0');
}

void usart_write_unsigned_int(unsigned int data) {
    //if (data>9999)
    usart_write_char(data/10000 + '0');
    //if (data>999)
    usart_write_char((data/1000)%10 + '0');
    //if (data>99)
    usart_write_char((data/100)%10 + '0');
    //if (data>9)
    usart_write_char((data/10)%10 + '0');
    usart_write_char(data%10 + '0');
}