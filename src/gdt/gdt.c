#include "gdt.h"
#include "serial/serial.h"
#include <stdint.h>

#define PORT 0x3f8 // COM1
#define DESCRIPTOR_SIZE 8
#define GDT_ENTRIES 5

uint64_t gdt[5];


// setup.asm
void *load_gdt(void);
void reload_registers(void);


static void create_descriptors(){

    gdt[0] = 0x0000000000000000; // Null segment descriptor 
    gdt[1] = 0x00af9b000000ffff; // Kernel code
    gdt[2] = 0x00af93000000ffff; // Kernel data
    gdt[3] = 0x00affb000000ffff; // Usermode code
    gdt[4] = 0x00aff3000000ffff; // Usermode data
}


static void set_gdtr_registers(uint64_t *gdt){
    
    asm("mov %0, %%di\n\t" "mov %1, %%rsi"
        : // no output
        : "r" ((unsigned short)((GDT_ENTRIES * DESCRIPTOR_SIZE) - 1)), "r" ((unsigned long long)(gdt)) // input
        : "%di", "%rsi"); // clobbered registers
}

void gdt_init(){

    asm("cli" ::: "memory"); // Disable interrupts

    create_descriptors();
    set_gdtr_registers(gdt);

    load_gdt();
    reload_registers();

}
