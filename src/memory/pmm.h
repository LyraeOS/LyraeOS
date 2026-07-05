#ifndef LYRAEOS_PMM_H
#define LYRAEOS_PMM_H
#include <stdint.h>
#include <stdbool.h>
#include <limine.h>
#include <stddef.h>
#include "datastructures/bitmap.h"

extern uint64_t hhdm_base;

void pmm_init(const struct limine_memmap_response *memmap);

void* pmm_alloc(size_t page_count);
void* pmm_allocz(size_t page_count);
void pmm_free(void* addr, size_t page_count);

void* pmm_find_free_run(size_t page_count);

#endif //LYRAEOS_PMM_H
