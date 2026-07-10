#ifndef LYRAEOS_PMM_H
#define LYRAEOS_PMM_H
#include <stdint.h>
#include <stdbool.h>
#include <lib/limine.h>
#include <stddef.h>
#include <mm/bitmap.h>

extern uint64_t hhdm_base;

void pmm_init(const struct limine_memmap_response *memmap);

void* pmm_alloc(size_t page_count);
void* pmm_allocz(size_t page_count);
void pmm_free(void* addr, size_t page_count);

void* pmm_find_free_run(size_t page_count);

size_t pmm_get_free_memory(void);

#endif //LYRAEOS_PMM_H
