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
    for (size_t i = 0; i < pci_device_count; i++) {
        pci_device_t current_device = pci_devices[i];
        kprintf("[({s} 0x{x})] => {s}\n",
                pci_vendor_name(current_device.vendor),
                current_device.vendor,
                pci_class_name(&current_device.class_info)
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
}

void pci_init() {
    pci_devices = (pci_device_t*) kmalloc(sizeof(pci_device_t) * 64);
    pci_probe();
};
static const pci_vendor_lookup_t pci_vendor_table[] = {
    {0x8086, "Intel"},
    {0x1234, "QEMU"},
    {0x10DE, "Nvidia"},
    {0x1180, "Ricoh Co. Ltd."} 
};

#define PCI_VENDOR_TABLE_COUNT \
    sizeof(pci_vendor_table) / sizeof(pci_vendor_table[0])

const char* pci_vendor_name(uint16_t vendor) {
    for (size_t i = 0; i < PCI_VENDOR_TABLE_COUNT; i++) {
        const pci_vendor_lookup_t* table_vendor = &pci_vendor_table[i];
        if (table_vendor->vendor == vendor) return table_vendor->name;
    }
    return "Unknown";
}

// ill fix this later once we have filessystem support
static const pci_class_lookup_t pci_class_table[] = {
    {0x0, 0x0, 0xFF, "Non-VGA-Compatible Unclassifed Device"},
    {0x0, 0x1, 0xFF, "VGA-Compatible Unclassifed Device"},
    {0x1, 0x0, 0xFF, "SCSI Bus Controller"},
    {0x1, 0x1, 0x00, "IDE Controller / ISA Compatibility mode-only controller"},
    {0x1, 0x1, 0x05, "IDE Controller / PCI native mode-only controller"},
    {0x1, 0x1, 0x0A, "IDE Controller / ISA Compatibility mode controller"},
    {0x1, 0x1, 0x0F, "IDE Controller / PCI native mode controller"},
    {0x1, 0x1, 0x80, "IDE Controller / ISA Compatibility mode-only controller"},
    {0x1, 0x1, 0x85, "IDE Controller / PCI native mode-only controller"},
    {0x1, 0x1, 0x8A, "IDE Controller / ISA Compatibility mode controller"},
    {0x1, 0x1, 0x8F, "IDE Controller / PCI native mode controller"},
    {0x1, 0x2, 0xFF, "Floppy Disk Controller"},
    {0x1, 0x3, 0xFF, "IPI Bus Controller"},
    {0x1, 0x4, 0xFF, "RAID Controller"},
    {0x1, 0x5, 0x20, "ATA Controller / Single DMA"},
    {0x1, 0x5, 0x30, "ATA Controller / Chained DMA"},
    {0x1, 0x6, 0x00, "Serial ATA Controller / Vendor Specific Interface"},
    {0x1, 0x6, 0x01, "Serial ATA Controller / AHCI 1.0"},
    {0x1, 0x6, 0x02, "Serial ATA Controller / Serial Storage Bus"},
    {0x1, 0x7, 0x00, "Serial Attached SCSI Controller / SAS"},
    {0x1, 0x7, 0x01, "Serial Attached SCSI Controller / Serial Storage Bus"},
    {0x1, 0x8, 0x01, "Non-Volatile Memory Controller / NVMHCI"},
    {0x1, 0x8, 0x02, "Non-Volatile Memory Controller / NVM Express"},
    {0x2, 0x0, 0xFF, "Ethernet Controller"},
    {0x2, 0x80, 0xFF, "Other Network Controller"},
    {0x3, 0x0, 0x0, "VGA Compatible Controller / VGA Controller"},
    {0x3, 0x0, 0x1, "VGA Compatible Controller / 8514-Compatible Controller"},
    {0x3, 0x1, 0xFF, "XGA Controller"},
    {0x3, 0x2, 0xFF, "3D Controller"},
    {0x3, 0x80, 0xFF, "Other Display Controller"},
    {0x4, 0x00, 0xFF, "Multimedia Video Controller"},
    {0x4, 0x01, 0xFF, "Multimedia Audio Controller"},
    {0x4, 0x02, 0xFF, "Computer Telephony Device"},
    {0x4, 0x03, 0xFF, "Audio Device"},
    {0x4, 0x80, 0xFF, "Other Multimedia Controller"},
    {0x5, 0x00, 0xFF, "RAM Controller"},
    {0x5, 0x01, 0xFF, "Flash Controller"},
    {0x5, 0x80, 0xFF, "Other Memory Controller"},
    {0x6, 0x00, 0xFF, "Host Bridge"},
    {0x6, 0x01, 0xFF, "ISA Bridge"},
    {0x6, 0x02, 0xFF, "EISA Bridge"},
    {0x6, 0x03, 0xFF, "MCA Bridge"},
    {0x6, 0x04, 0x00, "PCI-to-PCI Bridge / Normal Decode"},
    {0x6, 0x04, 0x01, "PCI-to-PCI Bridge / Subtractive Decode"},
    {0x6, 0x05, 0xFF, "PCMCIA Bridge"},
    {0x6, 0x06, 0xFF, "NuBus Bridge"},
    {0x6, 0x07, 0xFF, "CardBus Bridge"},
    {0x6, 0x08, 0x00, "RACEway Bridge / Transparent Mode"},
    {0x6, 0x08, 0x01, "RACEway Bridge / Endpoint Mode"},
    {0x6, 0x09, 0x40, "PCI-to-PCI Bridge / Semi-Transparent primary bus to host CPU"},
    {0x6, 0x09, 0x80, "PCI-to-PCI Bridge / Semi-Transparent secondary bus to host CPU"},
    {0x6, 0x0A, 0xFF, "InfiniBand-to-PCI Bridge"},
    {0x6, 0x80, 0xFF, "Other Bridge"},
    {0x7, 0x00, 0xFF, "Serial Controller"},
    {0x7, 0x01, 0xFF, "Parallel Controller"},
    {0x7, 0x02, 0xFF, "Multiport Serial Controller"},
    {0x7, 0x03, 0xFF, "Modem"},
    {0x7, 0x04, 0xFF, "IEEE 488.1/2 (GPIB) Controller"},
    {0x7, 0x05, 0xFF, "Smart Card Controller"},
    {0x7, 0x80, 0xFF, "Other Simple Communication Controller"},
    {0x8, 0x00, 0x00, "PIC / Generic 8259-Compatible"},
    {0x8, 0x00, 0x01, "PIC / ISA-Compatible"},
    {0x8, 0x00, 0x02, "PIC / EISA-Compatible"},
    {0x8, 0x00, 0x10, "PIC / I/O APIC Interrupt Controller"},
    {0x8, 0x00, 0x20, "PIC / I/O(x) APIC Interrupt Controller"},
    {0x8, 0x01, 0xFF, "DMA Controller"},
    {0x8, 0x02, 0xFF, "Timer"},
    {0x8, 0x03, 0xFF, "RTC Controller"},
    {0x8, 0x04, 0xFF, "PCI Hot-Plug Controller"},
    {0x8, 0x05, 0xFF, "SD Host Controller"},
    {0x8, 0x06, 0xFF, "IOMMU"},
    {0x8, 0x80, 0xFF, "Other Base System Peripheral"},
    {0x9, 0x00, 0xFF, "Keyboard Controller"},
    {0x9, 0x01, 0xFF, "Digitizer Pen"},
    {0x9, 0x02, 0xFF, "Mouse Controller"},
    {0x9, 0x03, 0xFF, "Scanner Controller"},
    {0x9, 0x04, 0xFF, "Gameport Controller"},
    {0x9, 0x80, 0xFF, "Other Input Device Controller"},
    {0xA, 0x00, 0xFF, "Generic Docking Station"},
    {0xA, 0x80, 0xFF, "Other Docking Station"},
    {0xB, 0x00, 0xFF, "Processor 386"},
    {0xB, 0x01, 0xFF, "Processor 486"},
    {0xB, 0x02, 0xFF, "Processor Pentium"},
    {0xB, 0x03, 0xFF, "Processor Pentium Pro"},
    {0xB, 0x04, 0xFF, "Processor Alpha"},
    {0xB, 0x05, 0xFF, "Processor PowerPC"},
    {0xB, 0x06, 0xFF, "Processor MIPS"},
    {0xB, 0x07, 0xFF, "Processor Co-Processor"},
    {0xB, 0x80, 0xFF, "Other Processor"},
    {0xC, 0x00, 0xFF, "Firewire (IEEE 1394) Controller"},
    {0xC, 0x01, 0xFF, "ACCESS Bus Controller"},
    {0xC, 0x02, 0xFF, "SSA"},
    {0xC, 0x03, 0x00, "USB Controller / UHCI"},
    {0xC, 0x03, 0x10, "USB Controller / OHCI"},
    {0xC, 0x03, 0x20, "USB Controller / EHCI (USB2)"},
    {0xC, 0x03, 0x30, "USB Controller / XHCI (USB3)"},
    {0xC, 0x03, 0x80, "USB Controller / Unspecified"},
    {0xC, 0x03, 0xFE, "USB Controller / USB Device"},
    {0xC, 0x04, 0xFF, "Fibre Channel"},
    {0xC, 0x05, 0xFF, "SMBus Controller"},
    {0xC, 0x06, 0xFF, "Infiniband Controller"},
    {0xC, 0x07, 0xFF, "IPMI Interface"},
    {0xC, 0x08, 0xFF, "SERCOS Interface"},
    {0xC, 0x09, 0xFF, "CANbus Controller"},
    {0xC, 0x80, 0xFF, "Other Serial Bus Controller"},
    {0xD, 0x00, 0xFF, "iRDA Compatible Controller"},
    {0xD, 0x01, 0xFF, "Consumer IR Controller"},
    {0xD, 0x10, 0xFF, "RF Controller"},
    {0xD, 0x11, 0xFF, "Bluetooth Controller"},
    {0xD, 0x12, 0xFF, "Broadband Controller"},
    {0xD, 0x20, 0xFF, "Ethernet Controller (802.1a)"},
    {0xD, 0x21, 0xFF, "Ethernet Controller (802.1b)"},
    {0xD, 0x80, 0xFF, "Other Wireless Controller"},
    {0xE, 0x00, 0xFF, "I20"},
    {0xF, 0x01, 0xFF, "Satellite TV Controller"},
    {0xF, 0x02, 0xFF, "Satellite Audio Controller"},
    {0xF, 0x03, 0xFF, "Satellite Voice Controller"},
    {0xF, 0x04, 0xFF, "Satellite Data Controller"},
    {0x10, 0x00, 0xFF, "Networking and Computing Encryption/Decryption"},
    {0x10, 0x10, 0xFF, "Entertainment Encryption/Decryption"},
    {0x10, 0x80, 0xFF, "Other Encryption Controller"},
    {0x11, 0x00, 0xFF, "DPIO Modules"},
    {0x11, 0x01, 0xFF, "Performance Counters"},
    {0x11, 0x10, 0xFF, "Communication Synchronizer"},
    {0x11, 0x20, 0xFF, "Signal Processing Management"},
    {0x11, 0x80, 0xFF, "Other Signal Processing Controller"},
};

#define PCI_CLASS_TABLE_COUNT \
    (sizeof(pci_class_table) / sizeof(pci_class_table[0]))

const char* pci_class_name(pci_class_info_t* info) {
    for (size_t i = 0; i < PCI_CLASS_TABLE_COUNT; i++) {
        const pci_class_lookup_t* entry = &pci_class_table[i];
        if (entry->class != info->class) continue;
        if (entry->subclass != info->subclass) continue;
        if (entry->prog_if != 0xFF && entry->prog_if != info->prog_if) continue;
        return entry->name;
    }
    return "Unknown";
}
