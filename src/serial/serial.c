#include "serial.h"
#include <stdint.h>



uint8_t inb(uint16_t port){
    uint8_t result;

    __asm__ ("inb %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}


// Output byte to serial port
void outb(uint16_t port, uint8_t data){
    __asm__ ("outb %%al, %%dx" :   : "a" (data), "d" (port));
}


int serial_received(uint16_t port){
    return inb(port + 5) & 1;
}


char serial_read(uint16_t port){
    while(serial_received(port) == 0);

    return inb(port);
}


int is_transmit_empty(uint16_t port){
    return inb(port + 5) & 0x20;
}


void serial_print(uint16_t port, char *data){
    while(is_transmit_empty(port) == 0);
    
    int i = 0;

    while(data[i] != '\0'){
        outb(port, data[i]);
        i++;
    }
}