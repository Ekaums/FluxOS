#pragma once
#include <stdint.h>
#include <stddef.h>

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.
 
// Copy data of size n from src to dest
void *memcpy(void *dest, const void *src, size_t n);
 
// Copy c (a byte) into s, n times. Useful for filling certain amount of bytes.
void *memset(void *s, int c, size_t n);
 
// Copies data of size n from src to dest. Use when dest and src overlap
void *memmove(void *dest, const void *src, size_t n);
 
// Compares the s1 and s2 buffers for n bytes
// Returns 0 if all bytes are same
// Returns (-) if s1 < s2
// Returns (+) if si > s2
int memcmp(const void *s1, const void *s2, size_t n);