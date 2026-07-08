#ifndef LYRAEOS_VMM_H
#define LYRAEOS_VMM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define VMM_PRESENT  (1ULL << 0)
#define VMM_WRITABLE (1ULL << 1)
#define VMM_USER     (1ULL << 2)

#define PML4_INDEX(addr) (((addr) >> 39) & 0x1FF)
#define PDPT_INDEX(addr) (((addr) >> 30) & 0x1FF)
#define PD_INDEX(addr)   (((addr) >> 21) & 0x1FF)
#define PT_INDEX(addr)   (((addr) >> 12) & 0x1FF)

typedef uint64_t page_directory_t;

void vmm_init(void);
page_directory_t* vmm_create_address_space(void);
bool vmm_map(page_directory_t* pml4, uintptr_t virt, uintptr_t phys, uint64_t flags);
void vmm_unmap(page_directory_t* pml4, uintptr_t virt);
void vmm_switch_address_space(page_directory_t* pml4);

#endif // LYRAEOS_VMM_H