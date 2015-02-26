#include "keyboard.h"

unsigned char keyboard_read() {
    unsigned char data;
    keyboard_select();
    data = spi_read();
    keyboard_unselect();
    return data;
}
