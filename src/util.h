#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdint.h>

void hlt_loop(void);
size_t kstrlen(const char* str);
int abs(int value);
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void wait_ms(uint64_t ms);
#endif
