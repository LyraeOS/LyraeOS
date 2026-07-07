#include "screen/gfx.h"
#include "limine.h"
#include "memory/mem.h"
#include "util.h"
#include "screen/tty.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "gdt.h"
#include "serial.h"
#include "shell/shell.h"
#include "intr/idt.h"
#include "intr/keyboard.h"
#include "memory/kheap.h"
#include "memory/pmm.h"
#include "memory/vmm.h"
#include "intr/mouse.h"

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

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_firmware_type_request
    fw_type_request = {.id = LIMINE_FIRMWARE_TYPE_REQUEST_ID, .revision = 0};

extern void fpu_init();
void kmain(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        hlt_loop();
    }
    init_serial();
    fpu_init();
    if (!init_gfx(framebuffer_request)) {
        hlt_loop();
    }
    kprintf("Booting LyraeOS!\n");
    const TTYTheme* cur = tty_cur_theme();
    const struct limine_memmap_response *mem_resp = memmap_request.response;
    if (mem_resp == NULL) {
        panic("No memory map :(");
    }
    const struct limine_firmware_type_response *fw_type_resp = fw_type_request.response;
    char* friendly_name;
    switch (fw_type_resp->firmware_type) {
	case LIMINE_FIRMWARE_TYPE_EFI32:
	    friendly_name = "EFI32";
	    break;
	case LIMINE_FIRMWARE_TYPE_EFI64:
	    friendly_name = "EFI64";
	    break;
	case LIMINE_FIRMWARE_TYPE_X86BIOS:
	    friendly_name = "BIOS";
	    break;
	default:
	    friendly_name = "UNKNOWN";
	    break;
    }
    kprintf("[{o}FW{r}] => firmware type is {s}\n", cur->info, friendly_name);
    pmm_init(mem_resp);
    vmm_init();
    kheap_init(0xFFFF900000000000, 4);

    int* b = kmalloc(sizeof(int)*4);
    b[0] = 1;
    b[1] = 2;
    b[2] = 3;
    b[3] = 4;
    kfree(b);

    gdt_install();
    kprintf("[{o}GDT{r}] => Init GDT\n", cur->info);
    keyboard_init(&keypress_queue);
    kprintf("[{o}IDT{r}] => Init IDT\n", cur->info);
    idt_install();
    tty_clear();
    shell_loop();
    kprintf("OS Functions Complete, Halting...\n");
    hlt_loop();
}
