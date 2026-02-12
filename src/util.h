#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdint.h>

void hlt_loop(void);
size_t kstrlen(const char* str);
int abs(int value);
#endif