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
void outl(uint16_t port, uint32_t val);
uint32_t inl(uint16_t port);

void charcat(char *s, char c);
bool strcmp(const char *s1, const char *s2);
void wait_ms(uint64_t ms);
int atoi(const char *s);
void panic(const char* message);
void assert(bool condition, const char* message);
int sum(int* array, int size);
float fsum(float* array, int size);
int min(int a, int b);
int max(int a, int b);
float fmin(float a, float b);
float fmax(float a, float b);
int amin(int* array, int size);
int amax(int* array, int size);
float afmin(float* array, int size);
float afmax(float* array, int sise);

#endif // UTIL_H
