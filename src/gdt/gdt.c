#include "gdt.h"
#include "serial/serial.h"
#include <stdint.h>

#define PORT 0x3f8 // COM1
#define DESCRIPTOR_SIZE 8
#define GDT_ENTRIES 5

uint64_t gdt[5];

// gdt_setup.asm
void load_gdtr(void);
void reload_registers(void);


// Fill up the gdt
// TODO: Make a gdt struct and format this better smh
static void create_descriptors(){

    gdt[0] = 0x0000000000000000; // Null segment descriptor 
    gdt[1] = 0x00af9b000000ffff; // Kernel code
    gdt[2] = 0x00af93000000ffff; // Kernel data
    gdt[3] = 0x00affb000000ffff; // Usermode code
    gdt[4] = 0x00aff3000000ffff; // Usermode data
}


// Setup for loading gdtr
// TODO: Move this to asm
static void set_gdtr_registers(uint64_t *gdt){
    
    __asm__ ("mov %0, %%di\n\t" "mov %1, %%rsi"
        : // no output
        : "r" ((uint16_t)((GDT_ENTRIES * DESCRIPTOR_SIZE) - 1)), "r" ((uint64_t)(gdt)) // input
        : "%di", "%rsi"); // clobbered registers
}

void gdt_init(){

    __asm__ ("cli" ::: "memory"); // Disable interrupts

    create_descriptors();
    set_gdtr_registers(gdt);
    load_gdtr();
    reload_registers();
}
