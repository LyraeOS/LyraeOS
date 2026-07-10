#include <mm/vmm.h>
#include <mm/pmm.h>
#include <mm/mem.h>

page_directory_t* kernel_pml4 = NULL;
uintptr_t kernel_pml4_phys = 0;

static page_directory_t* get_next_level(page_directory_t* current_level, size_t index, bool allocate) {
    uint64_t entry = current_level[index];

    if (entry & VMM_PRESENT) {
        return (page_directory_t*)PHYS_TO_VIRT(entry & ~0xFull);
    }

    if (!allocate) return NULL;

    void* new_table_phys = pmm_allocz(1);
    if (!new_table_phys) return NULL;

    current_level[index] = (uintptr_t)new_table_phys | VMM_PRESENT | VMM_WRITABLE | VMM_USER;

    return (page_directory_t*)PHYS_TO_VIRT(new_table_phys);
}

bool vmm_map(page_directory_t* pml4, uintptr_t virt, uintptr_t phys, uint64_t flags) {
    page_directory_t* pdpt = get_next_level(pml4, PML4_INDEX(virt), true);
    if (!pdpt) return false;

    page_directory_t* pd = get_next_level(pdpt, PDPT_INDEX(virt), true);
    if (!pd) return false;

    page_directory_t* pt = get_next_level(pd, PD_INDEX(virt), true);
    if (!pt) return false;

    size_t pt_idx = PT_INDEX(virt);

    pt[pt_idx] = (phys & ~0xFFF) | flags | VMM_PRESENT;

    asm volatile("invlpg (%0)" ::"r"(virt) : "memory");

    return true;
}

void vmm_unmap(page_directory_t* pml4, uintptr_t virt) {
    page_directory_t* pdpt = get_next_level(pml4, PML4_INDEX(virt), false);
    if (!pdpt) return;

    page_directory_t* pd = get_next_level(pdpt, PDPT_INDEX(virt), false);
    if (!pd) return;

    page_directory_t* pt = get_next_level(pd, PD_INDEX(virt), false);
    if (!pt) return;

    pt[PT_INDEX(virt)] = 0;
    asm volatile("invlpg (%0)" ::"r"(virt) : "memory");
}

page_directory_t* vmm_create_address_space(void) {
    void* new_pml4_phys = pmm_allocz(1);
    if (!new_pml4_phys) return NULL;

    page_directory_t* new_pml4_virt = (page_directory_t*)PHYS_TO_VIRT(new_pml4_phys);

    uint64_t* current_pml4;
    asm volatile("mov %%cr3, %0" : "=r"(current_pml4));
    current_pml4 = (uint64_t*)PHYS_TO_VIRT((uintptr_t)current_pml4 & ~0xFFF);

    for (int i = 256; i < 512; i++) {
        new_pml4_virt[i] = current_pml4[i];
    }

    return new_pml4_virt;
}

void vmm_switch_address_space(page_directory_t* pml4_virt) {
    uintptr_t phys = VIRT_TO_PHYS((uintptr_t)pml4_virt);
    asm volatile("mov %0, %%cr3" :: "r"(phys) : "memory");
}

void vmm_init(void) {
    kernel_pml4 = vmm_create_address_space();
    vmm_switch_address_space(kernel_pml4);
}
