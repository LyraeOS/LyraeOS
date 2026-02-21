#ifndef UTIL_H
#define UTIL_H
#include "intr/idt.h"
#include "liblyr.h"
void hlt_loop(void);
st kstrlen(const char* str);
int abs(int value);
void outb(u16 port, u8 val);
u8 inb(u16 port);
void charcat(char *s, char c);
bool strcmp(char *s1, char *s2);
void wait_ms(u64 ms);
int atoi(const char *s);
void panic(const char* message);
void assert(bool c, const char* message);
#endif
