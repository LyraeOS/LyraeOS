#include "kheap.h"
#include "vmm.h"
#include "pmm.h"
#include "mem.h"
#include "screen/gfx.h"

static heap_block_t* heap_start = NULL;
static uintptr_t heap_end = 0;
static uintptr_t heap_max = 0;

static bool heap_expand(size_t pages) {
    uintptr_t current_pml4_phys;
    asm volatile("mov %%cr3, %0" : "=r"(current_pml4_phys));
    page_directory_t* active_pml4 = (page_directory_t*)PHYS_TO_VIRT(current_pml4_phys);

    for (size_t i = 0; i < pages; i++) {
        if (heap_end >= heap_max) return false;

        void* phys = pmm_allocz(1);
        if (!phys) return false;

        if (!vmm_map(active_pml4, heap_end, (uintptr_t)phys, VMM_WRITABLE)) {
            pmm_free(phys, 1);
            return false;
        }
        heap_end += PAGE_SIZE;
    }
    return true;
}

void kheap_init(uintptr_t start_addr, size_t initial_pages) {
    heap_start = (heap_block_t*)start_addr;
    heap_end = start_addr;
    heap_max = start_addr + (initial_pages * 256 * PAGE_SIZE);

    if (!heap_expand(initial_pages)) {
        return;
    }

    heap_start->size = (initial_pages * PAGE_SIZE) - sizeof(heap_block_t);
    heap_start->is_free = true;
    heap_start->next = NULL;
}

void* kmalloc(size_t size) {
    if (size == 0) return NULL;
    size = (size + 7) & ~7;

    heap_block_t* current = heap_start;

    while (current != NULL) {
        if (current->is_free && current->size >= size) {

            if (current->size >= size + sizeof(heap_block_t) + 8) {
                heap_block_t* new_block = (heap_block_t*)((uintptr_t)current + sizeof(heap_block_t) + size);
                
                new_block->size = current->size - size - sizeof(heap_block_t);
                new_block->is_free = true;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }

            current->is_free = false;
            return (void*)((uintptr_t)current + sizeof(heap_block_t));
        }
        current = current->next;
    }

    // Out of memory space? Expand the heap dynamically
    size_t pages_needed = (size + sizeof(heap_block_t) + PAGE_SIZE - 1) / PAGE_SIZE;
    if (heap_expand(pages_needed)) {
        return kmalloc(size);
    }

    return NULL;
}

void kfree(void* ptr) {
    if (!ptr) return;
    heap_block_t* block = (heap_block_t*)((uintptr_t)ptr - sizeof(heap_block_t));
    block->is_free = true;

    heap_block_t* current = heap_start;
    while (current != NULL) {
        if (current->is_free && current->next && current->next->is_free) {
            current->size += sizeof(heap_block_t) + current->next->size;
            current->next = current->next->next;
            continue;
        }
        current = current->next;
    }
}