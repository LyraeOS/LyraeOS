#include <mm/pmm.h>
#include <mm/mem.h>
#include <drivers/display/tty.h>


static size_t highest_page_top = 0;
static size_t used_pages_count = 0;
bitmap_t pmm_bitmap;
__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_hhdm_request
    hhdm_request = {.id = LIMINE_HHDM_REQUEST_ID, .revision = 0};

uint64_t hhdm_base;
void pmm_init(const struct limine_memmap_response *memmap) {
    hhdm_base = hhdm_request.response->offset;
    size_t current_page_top = 0;
    struct limine_memmap_entry *current_entry;
    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        current_entry = memmap->entries[i];

        if (current_entry->type != LIMINE_MEMMAP_USABLE) continue;
        current_page_top = current_entry->base + current_entry->length;
        if (current_page_top > highest_page_top)
            highest_page_top = current_page_top;
    }
    used_pages_count = KB_TO_PAGES(highest_page_top);

    pmm_bitmap.size = ALIGN_UP(ALIGN_DOWN(highest_page_top, PAGE_SIZE) / PAGE_SIZE / 8, PAGE_SIZE);
    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        current_entry = memmap->entries[i];
        if (current_entry->type != LIMINE_MEMMAP_USABLE) continue;
        if (current_entry->base == 0) continue;
        if (current_entry->length >= pmm_bitmap.size) {
            pmm_bitmap.map = (uint64_t *)PHYS_TO_VIRT(current_entry->base);
            current_entry->base += pmm_bitmap.size;
            current_entry->length -= pmm_bitmap.size;

            break;
        }
    }
    memset((void*)pmm_bitmap.map, 0xFF, pmm_bitmap.size);

    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        current_entry = memmap->entries[i];
        if (current_entry->type != LIMINE_MEMMAP_USABLE) continue;

        uint64_t aligned_base = ALIGN_UP(current_entry->base, PAGE_SIZE);
        uint64_t aligned_end = ALIGN_DOWN(current_entry->base + current_entry->length, PAGE_SIZE);

        if (aligned_end > aligned_base) {
            size_t pages = (aligned_end - aligned_base) / PAGE_SIZE;
            pmm_free((void*)aligned_base, pages);
        }
    }
    uint64_t low_mem_pages = 0x100000 / PAGE_SIZE;
    for (size_t i = 0; i < low_mem_pages; i++) {
        bitmap_set_bit(&pmm_bitmap, i);
    }


    uintptr_t map_virtual_addr = (uintptr_t)pmm_bitmap.map;

    uint64_t bitmap_phys_base = map_virtual_addr - hhdm_base;

    size_t bitmap_page_count = pmm_bitmap.size / PAGE_SIZE;
    size_t bitmap_start_bit = bitmap_phys_base / PAGE_SIZE;

    for (size_t i = 0; i < bitmap_page_count; i++) {
        bitmap_set_bit(&pmm_bitmap, bitmap_start_bit + i);
    }

    bitmap_set_bit(&pmm_bitmap, 0); // Null pointer guard
}
void* pmm_alloc(size_t page_count) {
    if (used_pages_count <= 0) return NULL;

    void* ptr = pmm_find_free_run(page_count);
    if (ptr == NULL) return NULL;

    uint64_t idx = PAGE_TO_BIT(ptr);
    for (size_t i = 0; i < page_count; i++)
        bitmap_set_bit(&pmm_bitmap, idx + i);

    used_pages_count += page_count;

    return (void*)BIT_TO_PAGE(idx);
}
void* pmm_allocz(size_t page_count) {
    void* ptr = pmm_alloc(page_count);
    memset((void* )PHYS_TO_VIRT(ptr), 0, PAGE_SIZE * page_count);
    return ptr;
}
void pmm_free(void* addr, size_t page_count) {
    uint64_t idx = PAGE_TO_BIT(addr);
    for (size_t i = 0; i < page_count; i++)
        bitmap_unset_bit(&pmm_bitmap, idx + i);

    used_pages_count -= page_count;
}

void* pmm_find_free_run(size_t page_count) {
    size_t max_bits = PAGE_TO_BIT(highest_page_top);
    for (size_t all_bits = 0; all_bits < max_bits; all_bits++) {
        size_t page_counter;
        for (page_counter = 0; page_counter < page_count; page_counter++) {
            if (bitmap_check_bit(&pmm_bitmap, all_bits + page_counter)) {
                break;
            }
        }
        if (page_counter == page_count) {
            return (void*)BIT_TO_PAGE(all_bits);
        }
    }
    return NULL;
}
size_t pmm_get_free_memory(void) {
    size_t total_pages = KB_TO_PAGES(highest_page_top);
    
    if (used_pages_count >= total_pages) {
        return 0;
    }
    
    size_t free_pages = total_pages - used_pages_count;
    return free_pages * PAGE_SIZE; // Returns free memory in bytes
}
