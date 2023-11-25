global load_gdt
global reload_registers

section .data

gdtr DW 0 ; For limit storage
	 DQ 0 ; For base storage
 

section .text

load_gdt:
	lea rax, [rel gdtr]
	mov   [rax], DI
	mov   [rax+2], RSI
	lgdt  [rax]
	ret

reload_registers:
	push 0x08
	lea rax, [rel .reload_CS]
	push rax
	retfq

.reload_CS:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret
