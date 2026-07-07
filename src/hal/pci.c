#include "pci.h"
#include "memory/kheap.h"
#include "screen/tty.h"

pci_device_t* pci_devices = 0;
size_t pci_device_count = 0;

uint16_t pci_config_read_word(pci_location_t location, uint8_t offset) {
    uint32_t address;
    uint16_t tmp = 0;
  
    address = (uint32_t)((location.bus << 16) | (location.slot << 11) |
              (location.function << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
  
    outl(0xCF8, address);
    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

uint16_t pci_get_vendor(pci_location_t location) {
    uint16_t vendor = pci_config_read_word(location, 0);
    return vendor;
}

uint16_t pci_get_device_id(pci_location_t location) {
    uint16_t id = pci_config_read_word(location, 2);
    return id;
}

uint16_t pci_get_device_class(pci_location_t location) {
    uint16_t class = pci_config_read_word(location, 0xA);
    return (class & ~0x00FF) >> 8;
}

uint16_t pci_get_device_subclass(pci_location_t location) {
    uint16_t class = pci_config_read_word(location, 0xA);
    return (class & ~0xFF00);
}

uint8_t pci_get_device_prog_if(pci_location_t location) {
    return (pci_config_read_word(location, 0x08) >> 8) & 0xFF;
}

pci_class_info_t pci_get_class_info(pci_location_t location) {
    pci_class_info_t class_info = {
        pci_get_device_class(location),
        pci_get_device_subclass(location),
        pci_get_device_prog_if(location),
    };
    return class_info;
}

pci_device_t pci_get_device(pci_location_t location) {
    if (!pci_is_valid(pci_get_vendor(location))) return (pci_device_t) {0};
    pci_device_t device = {
        pci_get_class_info(location),
        location,
        pci_get_vendor(location),
        pci_get_device_id(location),
    };
    return device;
}

void pci_add_device(pci_device_t device) {
    pci_devices[pci_device_count] = device;
    pci_device_count++;
}

void pci_print_info() {
    kprintf("[vendor:id:class]\n");
    for (size_t i = 0; i < pci_device_count; i++) {
        pci_device_t current_device = pci_devices[i];
        kprintf("[0x{x}:0x{x}:0x{x}] found pci device\n",
                current_device.vendor,
                current_device.id,
                current_device.class_info.class
        );
    }
}

void pci_probe() {
    for(uint32_t bus = 0; bus < 256; bus++) {
        for(uint32_t slot = 0; slot < 32; slot++) {
            for(uint32_t function = 0; function < 8; function++) {
              pci_location_t location = {bus, slot, function};
              pci_device_t device = pci_get_device(location);
              if (device.vendor != 0) {
                  pci_add_device(device);
              }
            }
        }
    }
    pci_print_info();
}

void pci_init() {
    pci_devices = (pci_device_t*) kmalloc(sizeof(pci_device_t) * 64);
    pci_probe();
    kprintf("PCI finished loading devices\n");
};

char* pci_vendor_name(uint16_t vendor) {
    switch (vendor) {
        case 0x8086:
            return "Intel";
        case 0x1234:
            return "QEMU";
        default:
            return "UNKNOWN";
  }
}
