#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <drivers/display/gfx.h>
#include <drivers/display/tty.h>
#include <drivers/input/keyboard.h>
#include <drivers/pci/pci.h>
#include <mm/mem.h>
#include <mm/kheap.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <lib/limine.h>
#include <kernel/logging.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <kernel/serial.h>
#include <shell/shell.h>

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
    fpu_init();
    if (!init_gfx(framebuffer_request)) {
        hlt_loop();
    }
    LOG_INFO("Booting LyraeOS!");
    LOG_INFO("Serial init");
    init_serial();
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
    LOG_INFO("firmware type is {s}", friendly_name);
    pmm_init(mem_resp);
    vmm_init();
    kheap_init(0xFFFF900000000000, 4);

    LOG_INFO("Init GDT");
    gdt_install();
    LOG_INFO("Init keyboard");
    /* keyboard_init(&keypress_queue); */
    LOG_INFO("Init IDT");
    idt_install();
    LOG_DEBUG("Probing pci bus...");
    pci_init();

    shell_loop();
    LOG_WARNING("OS Functions Complete, Halting...\n");
    hlt_loop();
}
