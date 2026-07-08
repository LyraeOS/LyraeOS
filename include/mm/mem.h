#ifndef MEM_H
#define MEM_H

#include <stddef.h>
#include <stdint.h>
#define ALIGN_DOWN(address, align)  ((address) & ~((align)-1))
#define ALIGN_UP(address, align)    (((address) + (align)-1) & ~((align)-1))
#define PAGE_SIZE 4096ULL
#define KB_TO_PAGES(kb) (((kb) * 1024ULL) / PAGE_SIZE)

#define PHYS_TO_VIRT(p) ((void *)((uint64_t)(p) + hhdm_base))
#define VIRT_TO_PHYS(v) ((uint64_t)(v) - hhdm_base)
#define BIT_TO_PAGE(bit)    ((size_t)bit * PAGE_SIZE)
#define PAGE_TO_BIT(page)   ((size_t)page / PAGE_SIZE)

#define GiB 0x40000000UL

#define HEAP_MAX_SIZE	(4 * GiB)
#define HEAP_START_ADDR	0xFFFF900000000000


void *memcpy(void *restrict dest, const void *restrict src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

#endif