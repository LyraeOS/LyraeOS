#ifndef LYRAEOS_KHEAP_H
#define LYRAEOS_KHEAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct heap_block {
    size_t size;
    bool is_free;
    struct heap_block* next;
} heap_block_t;

void kheap_init(uintptr_t start_addr, size_t initial_pages);
void* kmalloc(size_t size);
void kfree(void* ptr);

size_t kheap_get_free_bytes(void);

size_t kheap_get_unallocated_bytes(void);

#endif // LYRAEOS_KHEAP_H
