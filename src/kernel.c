#include "gfx.h"
#include "mem.h"
#include "util.h"
#include "tty.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

__attribute__((used, section(".limine_requests"))) static volatile uint64_t
    limine_base_revision[] = LIMINE_BASE_REVISION(4);
__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST_ID, .revision = 0};
__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_memmap_request
    memmap_request = {.id = LIMINE_MEMMAP_REQUEST_ID, .revision = 0};

__attribute__((used,
               section(".limine_requests_start"))) static volatile uint64_t
    limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t
    limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

void kmain(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        hlt_loop();
    }
    if (!init_gfx(framebuffer_request)) {
        hlt_loop();
    }
    kprintf("Booting LyraeOS!\n");

    kprintf("\x7F Lyrae\n");
    kprintf("Available Memory Map Entries:\n");
    const struct limine_memmap_response *mem_resp = memmap_request.response;
    if (mem_resp == NULL) {
        kprintf("No memory map :(\n");
        hlt_loop();
    }
    kprintf("Count: {d}\n", mem_resp->entry_count);
    for (uint64_t i = 0; i < mem_resp->entry_count; i++) {
        struct limine_memmap_entry *entry = mem_resp->entries[i];
        if (entry->type != LIMINE_MEMMAP_USABLE)
            continue;
        kprintf("[{o}Usable Entry{r}] : {o}{d}{r}\n", 0x00FF00, 0x5555ff, i);
        kprintf("    length: {d} KiB\n", entry->length/1024);
    }
    
    kprintf("OS Functions Complete, Halting...\n");
    hlt_loop();
}