#include "state_machine.h"

unsigned char state_machine_get_event(unsigned char port, unsigned char data) {
    
    unsigned char state = ((PORTA & 0b00110000) >> 3) | ((PORTB & 0b00000010) >> 1);

    /* States :
     * STATE_M1_READ_AND_WRITE 0x00,
     * STATE_READ_AND_WRITE    0x01,
     * STATE_M1_READ           0x02,
     * STATE_READ              0x03,
     * STATE_M1_WRITE          0x04,
     * STATE_WRITE             0x05,
     * STATE_INT_ACK           0x06,
     * STATE_READ_OR_WRITE     0x07
     */
    const unsigned char event_from_state[] = {
        EVENT_M1_READ_AND_WRITE,
        EVENT_READ_AND_WRITE,
        EVENT_M1_READ,
        0xFF,
        EVENT_M1_WRITE,
        0xFF,
        EVENT_INT_ACK,
        EVENT_READ_OR_WRITE
    };

    /* Write states
     * PORT_USART      0x00
     * PORT_TIMER      0x01
     * PORT_SDCARD     0x02
     * PORT_EEPROM     0x03
     * PORT_RESTART    0x04
     */
    const unsigned char write_event_from_port[] = {
        EVENT_WRITE_CHAR,
        0xFF,
        0xFF,
        EVENT_FLASH_BIOS,
        EVENT_RESTART,
    };

    /* Read states
     * PORT_USART      0x00
     */
    const unsigned char read_event_from_port[] = {
        EVENT_READ_CHAR
    };

    if (state == STATE_WRITE) {
        if (port == PORT_TIMER) {
            if (data == 0x00) {
                return EVENT_TIMER_ENABLE;
            } else {
                return EVENT_TIMER_DISABLE;
            }
        }
        else if (port == PORT_SDCARD) {
            if (data == 0x00) {
                return EVENT_SDCARD_LOAD_SECTOR;
            } else {
                return EVENT_SDCARD_INIT;
            }
        } else {
            return write_event_from_port[port];
        } 

    } else if (state == STATE_READ) {
        return read_event_from_port[port];
    } else {
        return event_from_state[state];
    }
}