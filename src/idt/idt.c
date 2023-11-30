#include "idt.h"

#define GDT_OFFSET_KERNEL_CODE 0x08
#define IDT_MAX_DESCRIPTORS 256

static idtr_t idtr;
__attribute__((aligned(0x10))) static idt_entry_t idt[256]; // IDT

__attribute__((noreturn)) void exception_handler(void);
static void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
extern void *isr_stub_table[];


void exception_handler() {
        __asm__ ("cli");
    for (;;) {
        __asm__ ("hlt");
    } // Completely hangs the computer
}

static void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags){
    idt_entry_t *descriptor = &idt[vector];

    descriptor -> isr_low = (uint64_t)isr & 0xFFFF; // Lower 16 bits of ISR. 64 bit address
    descriptor -> isr_mid = ((uint64_t)isr >> 16) & 0xFFFF; // Middle 16 bits
    descriptor -> isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF; // Other half

    descriptor -> kernel_cs = GDT_OFFSET_KERNEL_CODE; // Segment selector, offset into gdt for kernel code segment

    descriptor -> attributes = flags;
    descriptor -> ist = 0;
    descriptor -> reserved = 0;

}

void idt_init(void){
    idtr.base = (uintptr_t)&idt[0]; // uintptr_t allows a pointer to be converted to an int safely
    idtr.limit = (uint16_t)(sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1);

    for (uint8_t vector = 0; vector < 32; vector++){
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        // vectors[vector] = true
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag

}