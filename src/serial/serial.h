#pragma once
#include <stdint.h>

// Receive byte from serial port
// TODO: Test this?
uint8_t inb(uint16_t port);

// Output byte to serial port
void outb(uint16_t port, uint8_t data);

// Check the DR bit of line status register. This is set if there is data that can be read
int serial_received(uint16_t port);

// Read serial for data, if there is any.
char read_serial(uint16_t port);

// Check THRE of line status register to check if buffer is empty (data can be sent)
int is_transmit_empty(uint16_t port);

// Output string to serial port
void serial_print(uint16_t port, char *data);