global load_gdtr
global reload_registers

section .data

; Initialize memory space for setting up gdtr
gdtr dw 0 ; For limit storage
	 dq 0 ; For base storage
 

section .text

; Load the gdtr register
load_gdtr:
	lea rax, [rel gdtr]
	mov   [rax], di
	mov   [rax+2], rsi
	lgdt  [rax]
	ret

; Reload CS register to refresh and use new gdt
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
