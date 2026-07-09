#include <lib/util.h>
#include <cpu/idt.h>
#include <kernel/command.h>
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
void outl(uint16_t port, uint32_t val) {
    __asm__ volatile ( "outl %0, %w1" : : "a"(val), "Nd"(port) );
}
uint32_t inl(uint16_t port) {
    uint32_t ret;
    __asm__ volatile ( "inl %w1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

void wait_ms(uint64_t ms) {
  uint64_t end_time = timer_ticks + ms;
  while (timer_ticks < end_time) {
    asm("hlt");
  }
}
void charcat(char *s, char c) {
  while (*s != '\0') {
      s++;
  }
  *s = c;
  *(s + 1) = '\0';
}
bool strcmp(const char *s1, const char *s2) {
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

void panic(const char* message, ...) {
    va_list args;
    va_start(args, message);
    tty_set_cursor_enabled(false);
    tty_change_theme(LYRAE_PANIC);
    kprintf("[{error}ERROR{reset}] KERNEL PANIC: ");
    vkprintf(message, args);
    va_end(args);
    asm volatile ("cli");
    hlt_loop();
}

int sum(int* array, int size) { //(int)(sizeof(array) / sizeof(array[0]))
    int acc = 0;
    for (int i = 0; i < size; i++)
    acc += array[i];
    return acc;
}

float fsum(float* array, int size) { //(float)(sizeof(array) / sizeof(array[0]))
    float acc = 0;
    for (int i = 0; i < size; i++)
    acc += array[i];
    return acc;
}

int min(int a, int b) {
    if (a < b) return a;
    return b;
}
int max(int a, int b) {
    if (a > b) return a;
    return b;
}
float fmin(float a, float b) {
    if (a < b) return a;
    return b;
}
float fmax(float a, float b) {
    if (a > b) return a;
    return b;
}

int amin(int* array, int size) { //(int)(sizeof(array)/sizeof(array[0])
    int value = array[0];
    for (int i = 0; i < size; i++) {
    	if (value > array[i]) {
		value = array[i];
	}
    }
    return value;
}

int amax(int* array, int size) { //(int)(sizeof(array)/sizeof(array[0])
    int value = array[0];
    for (int i = 0; i < size; i++) {
    	if (value < array[i]) {
		value = array[i];
	}
    }
    return value;
}

float afmin(float* array, int size) { //(float)(sizeof(array)/sizeof(array[0]))
    float value = array[0];
    for (int i = 0; i < size; i++) {
    	if (value > array[i]) {
		value = array[i];
	}
    }
    return value;
}

float afmax(float* array, int size) { //(float)(sizeof(array)/sizeof(array[0]))
    float value = array[0];
    for (int i = 0; i < size; i++) {
    	if (value < array[i]) {
		value = array[i];
	}
    }
    return value;
}

COMMAND(panic, "causes a kernel panic with a page fault") {
    *(volatile int *)0xDEADBEEF = 1;
    return -1; // can't return
}
