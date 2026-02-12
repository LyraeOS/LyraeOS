#include "util.h"
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