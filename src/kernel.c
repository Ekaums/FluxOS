#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

#include "serial/serial.h"
#include "mem/mem.h"
#include "gdt/gdt.h"

 
LIMINE_BASE_REVISION(1)

#define PORT 0x3f8 // COM1
 
// Requests allow kernel to interface with bootloader
// This request allows the kernel to maniplate the screen (framebuffer)
struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};
    


// Halt and catch fire function.
static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

// Kernel's entry point.
// If renaming _start(), make sure to change the linker script accordingly.
void _start(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    serial_print(PORT, "Initializing GDT...\n\r");
    gdt_init();
    serial_print(PORT, "Success.\n\n\r");

    serial_print(PORT, "Welcome to FluxOS");

    hcf(); // We're done, just hang...
}