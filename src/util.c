#include "util.h"
#include "intr/idt.h"
size_t kstrlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

void hlt_loop(void) {
  for (;;) {
    asm("hlt");
  }
}
int abs(int value) {
  if (value < 0) {
    return -value;
  }
  return value;
} 
void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}
uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}
void wait_ms(uint64_t ms) {
  uint64_t end_time = timer_ticks + ms;
  while (timer_ticks < end_time) {
  }
}
void charcat(char *s, char c) {
  while (*s != '\0') {
      s++;
  }
  *s = c;
  *(s + 1) = '\0';
}
bool strcmp(char *s1, char *s2) {
  while (*s1 && *s2) {
    if (*s1++ != *s2++)
      return false;
  }
  return *s1 == *s2;
}
int atoi(const char *s) {
  int num = 0;
  for (int i = 0; s[i] != '\0'; i++) {
    if (s[i] >= '0' && s[i] <= '9') {
      num = num * 10 + (s[i] - '0');
    }
    else {
      break;
    }
  }
  return num;
}