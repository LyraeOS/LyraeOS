#include "screen/gfx.h"
#include "limine.h"
#include "mem.h"
#include "util.h"
#include "screen/tty.h"
#include "gdt.h"
#include "shell/shell.h"
#include "intr/idt.h"
#include "intr/keyboard.h"

__attribute__((used, section(".limine_requests"))) static volatile u64
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
               section(".limine_requests_start"))) static volatile u64
    limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) static volatile u64
    limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;
extern void fpu_init();
void kmain(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        hlt_loop();
    }
    fpu_init();
    if (!init_gfx(framebuffer_request)) {
        hlt_loop();
    }
    kprintf("Booting LyraeOS!\n");

    kprintf("\x7F Lyrae\n");
    kprintf("[GDT] => Init GDT\n");
    gdt_install();
    keyboard_init(&keypress_queue);
    kprintf("[IDT] => Init IDT\n");
    idt_install();
    const struct limine_memmap_response *mem_resp = memmap_request.response;
    if (mem_resp == NULL) {
        panic("No memory map :(");
    }
    kprintf("[MEM] => Getting largest memory page...\n");
    u64 largest_page_size = 0, index = 0;
    for (u64 i = 0; i < mem_resp->entry_count; i++) {
        struct limine_memmap_entry *entry = mem_resp->entries[i];
        if (entry->length > largest_page_size && entry->type == LIMINE_MEMMAP_USABLE) {
            kprintf("[MEM] => {o}new largest page{r} -> {d} MiB {d} KiB\n", 0x00FF00, entry->length/1024/1024, (entry->length/1024)%1024);
            largest_page_size = entry->length;
            index = i;
        }
    }
    kprintf("[MEM] => largest page id: {d}\n", index);
    shell_loop();
    kprintf("OS Functions Complete, Halting...\n");
    hlt_loop();
}
