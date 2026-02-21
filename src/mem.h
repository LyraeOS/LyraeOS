#ifndef MEM_H
#define MEM_H
#include "liblyr.h"

void *memcpy(void *restrict dest, const void *restrict src, st n);
void *memset(void *s, int c, st n);
void *memmove(void *dest, const void *src, st n);
int memcmp(const void *s1, const void *s2, st n);

#endif