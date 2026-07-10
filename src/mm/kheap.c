#include <mm/kheap.h>
#include <mm/vmm.h>
#include <mm/pmm.h>
#include <mm/mem.h>
#include <kernel/logging.h>

static heap_block_t* heap_start = NULL;
static uintptr_t heap_end = 0;
static uintptr_t heap_max = 0;

static bool heap_expand(size_t pages) {
    LOG_DEBUG("expanding heap by {u} pages", pages);
    page_directory_t* active_pml4 = kernel_pml4;

    uintptr_t new_block_addr = heap_end;

    for (size_t i = 0; i < pages; i++) {
        if (heap_end >= heap_max) return false;

        void* phys = pmm_allocz(1);
        if (!phys) return false;

        if (!vmm_map(active_pml4, heap_end, (uintptr_t)phys, VMM_WRITABLE)) {
            pmm_free(phys, 1);
            return false;
        }
        volatile uint8_t* canary = (volatile uint8_t*)new_block_addr;
        if (i == 0) {
            *canary = 0xAA; // write canary right after page 0 is mapped
        } else {
            if (*canary != 0xAA) {
                LOG_DEBUG("canary at {X} corrupted after iteration {d}! value={x}",
                        (uint64_t)new_block_addr, i, *canary);
            }
        }
        /* LOG_DEBUG("allocated page {d} / {d}", i, pages); */
        heap_end += PAGE_SIZE;
        /* LOG_DEBUG("new heap end: {X}", (uint64_t) heap_end); */
        print_memory_status();
    }

    heap_block_t* new_block = (heap_block_t*)new_block_addr;
    new_block->size = (pages * PAGE_SIZE) - sizeof(heap_block_t);

    new_block->is_free = true;
    new_block->next = NULL;

    heap_block_t* current = heap_start;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_block;

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
size_t kheap_get_free_bytes(void) {
    size_t free_bytes = 0;
    heap_block_t* current = heap_start;

    while (current != NULL) {
        if (current->is_free) {
            free_bytes += current->size;
        }
        current = current->next;
    }

    return free_bytes;
}

size_t kheap_get_unallocated_bytes(void) {
    if (heap_max >= heap_end) {
        return heap_max - heap_end;
    }
    return 0;
}
