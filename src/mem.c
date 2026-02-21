#include "mem.h"
void *memcpy(void *restrict dest, const void *restrict src, st n) {
  u8 *restrict pdest = (u8 *restrict)dest;
  const u8 *restrict psrc = (const u8 *restrict)src;

  for (st i = 0; i < n; i++) {
    pdest[i] = psrc[i];
  }

  return dest;
}

void *memset(void *s, int c, st n) {
  u8 *p = (u8 *)s;

  for (st i = 0; i < n; i++) {
    p[i] = (u8)c;
  }

  return s;
}

void *memmove(void *dest, const void *src, st n) {
  u8 *pdest = (u8 *)dest;
  const u8 *psrc = (const u8 *)src;

  if (src > dest) {
    for (st i = 0; i < n; i++) {
      pdest[i] = psrc[i];
    }
  } else if (src < dest) {
    for (st i = n; i > 0; i--) {
      pdest[i - 1] = psrc[i - 1];
    }
  }

  return dest;
}

int memcmp(const void *s1, const void *s2, st n) {
  const u8 *p1 = (const u8 *)s1;
  const u8 *p2 = (const u8 *)s2;

  for (st i = 0; i < n; i++) {
    if (p1[i] != p2[i]) {
      return p1[i] < p2[i] ? -1 : 1;
    }
  }

  return 0;
}