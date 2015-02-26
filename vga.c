#include "vga.h"

void vga_write_char(unsigned char data) {
    vga_select();
    spi_write(data);
    vga_unselect();

    ioexp_interrupt_select();
    spi_write(IOEXP_READ);
    spi_write(IOEXP8_GPIO);
    while ((spi_read() & VGA_BUSY_MASK) == 0);
    ioexp_unselect();
}
