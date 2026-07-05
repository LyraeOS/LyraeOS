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
void kheap_visualize(int start_x, int start_y, int max_width, int line_height);

#endif // LYRAEOS_KHEAP_H