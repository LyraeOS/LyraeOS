#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void hlt_loop(void);
size_t kstrlen(const char* str);
int abs(int value);
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void charcat(char *s, char c);
bool strcmp(char *s1, char *s2);
void wait_ms(uint64_t ms);
int atoi(const char *s);
void panic(const char* message);
#endif
