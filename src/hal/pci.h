#ifndef LYRAEOS_PCI_H
#define LYRAEOS_PCI_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "util.h"

typedef struct {
    uint8_t bus;
    uint8_t slot;
    uint8_t function;
} pci_location_t;

typedef struct {
    uint8_t class;
    uint8_t subclass;
    uint8_t prog_if;
} pci_class_info_t;

typedef struct {
    uint16_t vendor;
    const char* name;
} pci_vendor_lookup_t;

typedef struct {
    uint8_t class;
    uint8_t subclass;
    uint8_t prog_if;
    const char* name;
} pci_class_lookup_t;
    

typedef struct {
    pci_class_info_t class_info;
    pci_location_t location;
    uint16_t vendor;
    uint16_t id;
} pci_device_t;

uint16_t pci_config_read_word(pci_location_t location, uint8_t offset);
uint16_t pci_get_vendor(pci_location_t location);
uint16_t pci_get_device_id(pci_location_t location);
uint16_t pci_get_device_class(pci_location_t location);
uint16_t pci_get_device_subclass(pci_location_t location);
uint8_t pci_get_device_prog_if(pci_location_t location);
pci_class_info_t pci_get_class_info(pci_location_t location);
pci_device_t pci_get_device(pci_location_t location);
void pci_add_device(pci_device_t device);
void pci_print_info();
void pci_probe();
void pci_init();

inline bool pci_is_valid(uint16_t vendor_id) {
    return vendor_id != 0xFFFF;
}

const char* pci_vendor_name(uint16_t vendor);
const char* pci_class_name(pci_class_info_t* class);


#endif // LYRAEOS_PCI_H
