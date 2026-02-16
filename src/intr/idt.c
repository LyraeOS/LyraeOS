#include "idt.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags, uint8_t ist) {
    idt[num].offset_low  = base & 0xFFFF;
    idt[num].selector    = sel;
    idt[num].ist         = ist & 0x7;
    idt[num].type_attr   = flags;
    idt[num].offset_mid  = (base >> 16) & 0xFFFF;
    idt[num].offset_high = (base >> 32) & 0xFFFFFFFF;
    idt[num].zero        = 0;
}
void init_pit(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;  // PIT base frequency
    
    outb(0x43, 0x36);  // Command: channel 0, lohi, rate generator
    outb(0x40, divisor & 0xFF);         // Low byte
    outb(0x40, (divisor >> 8) & 0xFF);  // High byte
}
void idt_install()
{
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint64_t)&idt;

    memset(&idt, 0, sizeof(struct idt_entry) * 256);
    
    idt_set_gate(0, (uint64_t)ISR0, 0x08, 0x8E, 0);
    idt_set_gate(1, (uint64_t)ISR1, 0x08, 0x8E, 0);
    idt_set_gate(2, (uint64_t)ISR2, 0x08, 0x8E, 0);
    idt_set_gate(3, (uint64_t)ISR3, 0x08, 0x8E, 0);
    idt_set_gate(4, (uint64_t)ISR4, 0x08, 0x8E, 0);
    idt_set_gate(5, (uint64_t)ISR5, 0x08, 0x8E, 0);
    idt_set_gate(6, (uint64_t)ISR6, 0x08, 0x8E, 0);
    idt_set_gate(7, (uint64_t)ISR7, 0x08, 0x8E, 0);
    idt_set_gate(8, (uint64_t)ISR8, 0x08, 0x8E, 0);
    idt_set_gate(9, (uint64_t)ISR9, 0x08, 0x8E, 0);
    idt_set_gate(10, (uint64_t)ISR10, 0x08, 0x8E, 0);
    idt_set_gate(11, (uint64_t)ISR11, 0x08, 0x8E, 0);
    idt_set_gate(12, (uint64_t)ISR12, 0x08, 0x8E, 0);
    idt_set_gate(13, (uint64_t)ISR13, 0x08, 0x8E, 0);
    idt_set_gate(14, (uint64_t)ISR14, 0x08, 0x8E, 0);
    idt_set_gate(15, (uint64_t)ISR15, 0x08, 0x8E, 0);
    idt_set_gate(16, (uint64_t)ISR16, 0x08, 0x8E, 0);
    idt_set_gate(17, (uint64_t)ISR17, 0x08, 0x8E, 0);
    idt_set_gate(18, (uint64_t)ISR18, 0x08, 0x8E, 0);
    idt_set_gate(19, (uint64_t)ISR19, 0x08, 0x8E, 0);
    idt_set_gate(20, (uint64_t)ISR20, 0x08, 0x8E, 0);
    idt_set_gate(21, (uint64_t)ISR21, 0x08, 0x8E, 0);
    idt_set_gate(22, (uint64_t)ISR22, 0x08, 0x8E, 0);
    idt_set_gate(23, (uint64_t)ISR23, 0x08, 0x8E, 0);
    idt_set_gate(24, (uint64_t)ISR24, 0x08, 0x8E, 0);
    idt_set_gate(25, (uint64_t)ISR25, 0x08, 0x8E, 0);
    idt_set_gate(26, (uint64_t)ISR26, 0x08, 0x8E, 0);
    idt_set_gate(27, (uint64_t)ISR27, 0x08, 0x8E, 0);
    idt_set_gate(28, (uint64_t)ISR28, 0x08, 0x8E, 0);
    idt_set_gate(29, (uint64_t)ISR29, 0x08, 0x8E, 0);
    idt_set_gate(30, (uint64_t)ISR30, 0x08, 0x8E, 0);
    idt_set_gate(31, (uint64_t)ISR31, 0x08, 0x8E, 0);
    idt_set_gate(34, (uint64_t)ISR34, 0x08, 0x8E, 0);
    idt_set_gate(35, (uint64_t)ISR35, 0x08, 0x8E, 0);
    idt_set_gate(36, (uint64_t)ISR36, 0x08, 0x8E, 0);
    idt_set_gate(37, (uint64_t)ISR37, 0x08, 0x8E, 0);
    idt_set_gate(38, (uint64_t)ISR38, 0x08, 0x8E, 0);
    idt_set_gate(39, (uint64_t)ISR39, 0x08, 0x8E, 0);
    idt_set_gate(40, (uint64_t)ISR40, 0x08, 0x8E, 0);
    idt_set_gate(41, (uint64_t)ISR41, 0x08, 0x8E, 0);
    idt_set_gate(42, (uint64_t)ISR42, 0x08, 0x8E, 0);
    idt_set_gate(43, (uint64_t)ISR43, 0x08, 0x8E, 0);
    idt_set_gate(44, (uint64_t)ISR44, 0x08, 0x8E, 0);
    idt_set_gate(45, (uint64_t)ISR45, 0x08, 0x8E, 0);
    idt_set_gate(46, (uint64_t)ISR46, 0x08, 0x8E, 0);
    idt_set_gate(47, (uint64_t)ISR47, 0x08, 0x8E, 0);
    idt_set_gate(48, (uint64_t)ISR48, 0x08, 0x8E, 0);
    idt_set_gate(49, (uint64_t)ISR49, 0x08, 0x8E, 0);
    idt_set_gate(50, (uint64_t)ISR50, 0x08, 0x8E, 0);
    idt_set_gate(51, (uint64_t)ISR51, 0x08, 0x8E, 0);
    idt_set_gate(52, (uint64_t)ISR52, 0x08, 0x8E, 0);
    idt_set_gate(53, (uint64_t)ISR53, 0x08, 0x8E, 0);
    idt_set_gate(54, (uint64_t)ISR54, 0x08, 0x8E, 0);
    idt_set_gate(55, (uint64_t)ISR55, 0x08, 0x8E, 0);
    idt_set_gate(56, (uint64_t)ISR56, 0x08, 0x8E, 0);
    idt_set_gate(57, (uint64_t)ISR57, 0x08, 0x8E, 0);
    idt_set_gate(58, (uint64_t)ISR58, 0x08, 0x8E, 0);
    idt_set_gate(59, (uint64_t)ISR59, 0x08, 0x8E, 0);
    idt_set_gate(60, (uint64_t)ISR60, 0x08, 0x8E, 0);
    idt_set_gate(61, (uint64_t)ISR61, 0x08, 0x8E, 0);
    idt_set_gate(62, (uint64_t)ISR62, 0x08, 0x8E, 0);
    idt_set_gate(63, (uint64_t)ISR63, 0x08, 0x8E, 0);
    idt_set_gate(64, (uint64_t)ISR64, 0x08, 0x8E, 0);
    idt_set_gate(65, (uint64_t)ISR65, 0x08, 0x8E, 0);
    idt_set_gate(66, (uint64_t)ISR66, 0x08, 0x8E, 0);
    idt_set_gate(67, (uint64_t)ISR67, 0x08, 0x8E, 0);
    idt_set_gate(68, (uint64_t)ISR68, 0x08, 0x8E, 0);
    idt_set_gate(69, (uint64_t)ISR69, 0x08, 0x8E, 0);
    idt_set_gate(70, (uint64_t)ISR70, 0x08, 0x8E, 0);
    idt_set_gate(71, (uint64_t)ISR71, 0x08, 0x8E, 0);
    idt_set_gate(72, (uint64_t)ISR72, 0x08, 0x8E, 0);
    idt_set_gate(73, (uint64_t)ISR73, 0x08, 0x8E, 0);
    idt_set_gate(74, (uint64_t)ISR74, 0x08, 0x8E, 0);
    idt_set_gate(75, (uint64_t)ISR75, 0x08, 0x8E, 0);
    idt_set_gate(76, (uint64_t)ISR76, 0x08, 0x8E, 0);
    idt_set_gate(77, (uint64_t)ISR77, 0x08, 0x8E, 0);
    idt_set_gate(78, (uint64_t)ISR78, 0x08, 0x8E, 0);
    idt_set_gate(79, (uint64_t)ISR79, 0x08, 0x8E, 0);
    idt_set_gate(80, (uint64_t)ISR80, 0x08, 0x8E, 0);
    idt_set_gate(81, (uint64_t)ISR81, 0x08, 0x8E, 0);
    idt_set_gate(82, (uint64_t)ISR82, 0x08, 0x8E, 0);
    idt_set_gate(83, (uint64_t)ISR83, 0x08, 0x8E, 0);
    idt_set_gate(84, (uint64_t)ISR84, 0x08, 0x8E, 0);
    idt_set_gate(85, (uint64_t)ISR85, 0x08, 0x8E, 0);
    idt_set_gate(86, (uint64_t)ISR86, 0x08, 0x8E, 0);
    idt_set_gate(87, (uint64_t)ISR87, 0x08, 0x8E, 0);
    idt_set_gate(88, (uint64_t)ISR88, 0x08, 0x8E, 0);
    idt_set_gate(89, (uint64_t)ISR89, 0x08, 0x8E, 0);
    idt_set_gate(90, (uint64_t)ISR90, 0x08, 0x8E, 0);
    idt_set_gate(91, (uint64_t)ISR91, 0x08, 0x8E, 0);
    idt_set_gate(92, (uint64_t)ISR92, 0x08, 0x8E, 0);
    idt_set_gate(93, (uint64_t)ISR93, 0x08, 0x8E, 0);
    idt_set_gate(94, (uint64_t)ISR94, 0x08, 0x8E, 0);
    idt_set_gate(95, (uint64_t)ISR95, 0x08, 0x8E, 0);
    idt_set_gate(96, (uint64_t)ISR96, 0x08, 0x8E, 0);
    idt_set_gate(97, (uint64_t)ISR97, 0x08, 0x8E, 0);
    idt_set_gate(98, (uint64_t)ISR98, 0x08, 0x8E, 0);
    idt_set_gate(99, (uint64_t)ISR99, 0x08, 0x8E, 0);
    idt_set_gate(100, (uint64_t)ISR100, 0x08, 0x8E, 0);
    idt_set_gate(101, (uint64_t)ISR101, 0x08, 0x8E, 0);
    idt_set_gate(102, (uint64_t)ISR102, 0x08, 0x8E, 0);
    idt_set_gate(103, (uint64_t)ISR103, 0x08, 0x8E, 0);
    idt_set_gate(104, (uint64_t)ISR104, 0x08, 0x8E, 0);
    idt_set_gate(105, (uint64_t)ISR105, 0x08, 0x8E, 0);
    idt_set_gate(106, (uint64_t)ISR106, 0x08, 0x8E, 0);
    idt_set_gate(107, (uint64_t)ISR107, 0x08, 0x8E, 0);
    idt_set_gate(108, (uint64_t)ISR108, 0x08, 0x8E, 0);
    idt_set_gate(109, (uint64_t)ISR109, 0x08, 0x8E, 0);
    idt_set_gate(110, (uint64_t)ISR110, 0x08, 0x8E, 0);
    idt_set_gate(111, (uint64_t)ISR111, 0x08, 0x8E, 0);
    idt_set_gate(112, (uint64_t)ISR112, 0x08, 0x8E, 0);
    idt_set_gate(113, (uint64_t)ISR113, 0x08, 0x8E, 0);
    idt_set_gate(114, (uint64_t)ISR114, 0x08, 0x8E, 0);
    idt_set_gate(115, (uint64_t)ISR115, 0x08, 0x8E, 0);
    idt_set_gate(116, (uint64_t)ISR116, 0x08, 0x8E, 0);
    idt_set_gate(117, (uint64_t)ISR117, 0x08, 0x8E, 0);
    idt_set_gate(118, (uint64_t)ISR118, 0x08, 0x8E, 0);
    idt_set_gate(119, (uint64_t)ISR119, 0x08, 0x8E, 0);
    idt_set_gate(120, (uint64_t)ISR120, 0x08, 0x8E, 0);
    idt_set_gate(121, (uint64_t)ISR121, 0x08, 0x8E, 0);
    idt_set_gate(122, (uint64_t)ISR122, 0x08, 0x8E, 0);
    idt_set_gate(123, (uint64_t)ISR123, 0x08, 0x8E, 0);
    idt_set_gate(124, (uint64_t)ISR124, 0x08, 0x8E, 0);
    idt_set_gate(125, (uint64_t)ISR125, 0x08, 0x8E, 0);
    idt_set_gate(126, (uint64_t)ISR126, 0x08, 0x8E, 0);
    idt_set_gate(127, (uint64_t)ISR127, 0x08, 0x8E, 0);
    idt_set_gate(128, (uint64_t)ISR128, 0x08, 0x8E, 0);
    idt_set_gate(129, (uint64_t)ISR129, 0x08, 0x8E, 0);
    idt_set_gate(130, (uint64_t)ISR130, 0x08, 0x8E, 0);
    idt_set_gate(131, (uint64_t)ISR131, 0x08, 0x8E, 0);
    idt_set_gate(132, (uint64_t)ISR132, 0x08, 0x8E, 0);
    idt_set_gate(133, (uint64_t)ISR133, 0x08, 0x8E, 0);
    idt_set_gate(134, (uint64_t)ISR134, 0x08, 0x8E, 0);
    idt_set_gate(135, (uint64_t)ISR135, 0x08, 0x8E, 0);
    idt_set_gate(136, (uint64_t)ISR136, 0x08, 0x8E, 0);
    idt_set_gate(137, (uint64_t)ISR137, 0x08, 0x8E, 0);
    idt_set_gate(138, (uint64_t)ISR138, 0x08, 0x8E, 0);
    idt_set_gate(139, (uint64_t)ISR139, 0x08, 0x8E, 0);
    idt_set_gate(140, (uint64_t)ISR140, 0x08, 0x8E, 0);
    idt_set_gate(141, (uint64_t)ISR141, 0x08, 0x8E, 0);
    idt_set_gate(142, (uint64_t)ISR142, 0x08, 0x8E, 0);
    idt_set_gate(143, (uint64_t)ISR143, 0x08, 0x8E, 0);
    idt_set_gate(144, (uint64_t)ISR144, 0x08, 0x8E, 0);
    idt_set_gate(145, (uint64_t)ISR145, 0x08, 0x8E, 0);
    idt_set_gate(146, (uint64_t)ISR146, 0x08, 0x8E, 0);
    idt_set_gate(147, (uint64_t)ISR147, 0x08, 0x8E, 0);
    idt_set_gate(148, (uint64_t)ISR148, 0x08, 0x8E, 0);
    idt_set_gate(149, (uint64_t)ISR149, 0x08, 0x8E, 0);
    idt_set_gate(150, (uint64_t)ISR150, 0x08, 0x8E, 0);
    idt_set_gate(151, (uint64_t)ISR151, 0x08, 0x8E, 0);
    idt_set_gate(152, (uint64_t)ISR152, 0x08, 0x8E, 0);
    idt_set_gate(153, (uint64_t)ISR153, 0x08, 0x8E, 0);
    idt_set_gate(154, (uint64_t)ISR154, 0x08, 0x8E, 0);
    idt_set_gate(155, (uint64_t)ISR155, 0x08, 0x8E, 0);
    idt_set_gate(156, (uint64_t)ISR156, 0x08, 0x8E, 0);
    idt_set_gate(157, (uint64_t)ISR157, 0x08, 0x8E, 0);
    idt_set_gate(158, (uint64_t)ISR158, 0x08, 0x8E, 0);
    idt_set_gate(159, (uint64_t)ISR159, 0x08, 0x8E, 0);
    idt_set_gate(160, (uint64_t)ISR160, 0x08, 0x8E, 0);
    idt_set_gate(161, (uint64_t)ISR161, 0x08, 0x8E, 0);
    idt_set_gate(162, (uint64_t)ISR162, 0x08, 0x8E, 0);
    idt_set_gate(163, (uint64_t)ISR163, 0x08, 0x8E, 0);
    idt_set_gate(164, (uint64_t)ISR164, 0x08, 0x8E, 0);
    idt_set_gate(165, (uint64_t)ISR165, 0x08, 0x8E, 0);
    idt_set_gate(166, (uint64_t)ISR166, 0x08, 0x8E, 0);
    idt_set_gate(167, (uint64_t)ISR167, 0x08, 0x8E, 0);
    idt_set_gate(168, (uint64_t)ISR168, 0x08, 0x8E, 0);
    idt_set_gate(169, (uint64_t)ISR169, 0x08, 0x8E, 0);
    idt_set_gate(170, (uint64_t)ISR170, 0x08, 0x8E, 0);
    idt_set_gate(171, (uint64_t)ISR171, 0x08, 0x8E, 0);
    idt_set_gate(172, (uint64_t)ISR172, 0x08, 0x8E, 0);
    idt_set_gate(173, (uint64_t)ISR173, 0x08, 0x8E, 0);
    idt_set_gate(174, (uint64_t)ISR174, 0x08, 0x8E, 0);
    idt_set_gate(175, (uint64_t)ISR175, 0x08, 0x8E, 0);
    idt_set_gate(176, (uint64_t)ISR176, 0x08, 0x8E, 0);
    idt_set_gate(177, (uint64_t)ISR177, 0x08, 0x8E, 0);
    idt_set_gate(178, (uint64_t)ISR178, 0x08, 0x8E, 0);
    idt_set_gate(179, (uint64_t)ISR179, 0x08, 0x8E, 0);
    idt_set_gate(180, (uint64_t)ISR180, 0x08, 0x8E, 0);
    idt_set_gate(181, (uint64_t)ISR181, 0x08, 0x8E, 0);
    idt_set_gate(182, (uint64_t)ISR182, 0x08, 0x8E, 0);
    idt_set_gate(183, (uint64_t)ISR183, 0x08, 0x8E, 0);
    idt_set_gate(184, (uint64_t)ISR184, 0x08, 0x8E, 0);
    idt_set_gate(185, (uint64_t)ISR185, 0x08, 0x8E, 0);
    idt_set_gate(186, (uint64_t)ISR186, 0x08, 0x8E, 0);
    idt_set_gate(187, (uint64_t)ISR187, 0x08, 0x8E, 0);
    idt_set_gate(188, (uint64_t)ISR188, 0x08, 0x8E, 0);
    idt_set_gate(189, (uint64_t)ISR189, 0x08, 0x8E, 0);
    idt_set_gate(190, (uint64_t)ISR190, 0x08, 0x8E, 0);
    idt_set_gate(191, (uint64_t)ISR191, 0x08, 0x8E, 0);
    idt_set_gate(192, (uint64_t)ISR192, 0x08, 0x8E, 0);
    idt_set_gate(193, (uint64_t)ISR193, 0x08, 0x8E, 0);
    idt_set_gate(194, (uint64_t)ISR194, 0x08, 0x8E, 0);
    idt_set_gate(195, (uint64_t)ISR195, 0x08, 0x8E, 0);
    idt_set_gate(196, (uint64_t)ISR196, 0x08, 0x8E, 0);
    idt_set_gate(197, (uint64_t)ISR197, 0x08, 0x8E, 0);
    idt_set_gate(198, (uint64_t)ISR198, 0x08, 0x8E, 0);
    idt_set_gate(199, (uint64_t)ISR199, 0x08, 0x8E, 0);
    idt_set_gate(200, (uint64_t)ISR200, 0x08, 0x8E, 0);
    idt_set_gate(201, (uint64_t)ISR201, 0x08, 0x8E, 0);
    idt_set_gate(202, (uint64_t)ISR202, 0x08, 0x8E, 0);
    idt_set_gate(203, (uint64_t)ISR203, 0x08, 0x8E, 0);
    idt_set_gate(204, (uint64_t)ISR204, 0x08, 0x8E, 0);
    idt_set_gate(205, (uint64_t)ISR205, 0x08, 0x8E, 0);
    idt_set_gate(206, (uint64_t)ISR206, 0x08, 0x8E, 0);
    idt_set_gate(207, (uint64_t)ISR207, 0x08, 0x8E, 0);
    idt_set_gate(208, (uint64_t)ISR208, 0x08, 0x8E, 0);
    idt_set_gate(209, (uint64_t)ISR209, 0x08, 0x8E, 0);
    idt_set_gate(210, (uint64_t)ISR210, 0x08, 0x8E, 0);
    idt_set_gate(211, (uint64_t)ISR211, 0x08, 0x8E, 0);
    idt_set_gate(212, (uint64_t)ISR212, 0x08, 0x8E, 0);
    idt_set_gate(213, (uint64_t)ISR213, 0x08, 0x8E, 0);
    idt_set_gate(214, (uint64_t)ISR214, 0x08, 0x8E, 0);
    idt_set_gate(215, (uint64_t)ISR215, 0x08, 0x8E, 0);
    idt_set_gate(216, (uint64_t)ISR216, 0x08, 0x8E, 0);
    idt_set_gate(217, (uint64_t)ISR217, 0x08, 0x8E, 0);
    idt_set_gate(218, (uint64_t)ISR218, 0x08, 0x8E, 0);
    idt_set_gate(219, (uint64_t)ISR219, 0x08, 0x8E, 0);
    idt_set_gate(220, (uint64_t)ISR220, 0x08, 0x8E, 0);
    idt_set_gate(221, (uint64_t)ISR221, 0x08, 0x8E, 0);
    idt_set_gate(222, (uint64_t)ISR222, 0x08, 0x8E, 0);
    idt_set_gate(223, (uint64_t)ISR223, 0x08, 0x8E, 0);
    idt_set_gate(224, (uint64_t)ISR224, 0x08, 0x8E, 0);
    idt_set_gate(225, (uint64_t)ISR225, 0x08, 0x8E, 0);
    idt_set_gate(226, (uint64_t)ISR226, 0x08, 0x8E, 0);
    idt_set_gate(227, (uint64_t)ISR227, 0x08, 0x8E, 0);
    idt_set_gate(228, (uint64_t)ISR228, 0x08, 0x8E, 0);
    idt_set_gate(229, (uint64_t)ISR229, 0x08, 0x8E, 0);
    idt_set_gate(230, (uint64_t)ISR230, 0x08, 0x8E, 0);
    idt_set_gate(231, (uint64_t)ISR231, 0x08, 0x8E, 0);
    idt_set_gate(232, (uint64_t)ISR232, 0x08, 0x8E, 0);
    idt_set_gate(233, (uint64_t)ISR233, 0x08, 0x8E, 0);
    idt_set_gate(234, (uint64_t)ISR234, 0x08, 0x8E, 0);
    idt_set_gate(235, (uint64_t)ISR235, 0x08, 0x8E, 0);
    idt_set_gate(236, (uint64_t)ISR236, 0x08, 0x8E, 0);
    idt_set_gate(237, (uint64_t)ISR237, 0x08, 0x8E, 0);
    idt_set_gate(238, (uint64_t)ISR238, 0x08, 0x8E, 0);
    idt_set_gate(239, (uint64_t)ISR239, 0x08, 0x8E, 0);
    idt_set_gate(240, (uint64_t)ISR240, 0x08, 0x8E, 0);
    idt_set_gate(241, (uint64_t)ISR241, 0x08, 0x8E, 0);
    idt_set_gate(242, (uint64_t)ISR242, 0x08, 0x8E, 0);
    idt_set_gate(243, (uint64_t)ISR243, 0x08, 0x8E, 0);
    idt_set_gate(244, (uint64_t)ISR244, 0x08, 0x8E, 0);
    idt_set_gate(245, (uint64_t)ISR245, 0x08, 0x8E, 0);
    idt_set_gate(246, (uint64_t)ISR246, 0x08, 0x8E, 0);
    idt_set_gate(247, (uint64_t)ISR247, 0x08, 0x8E, 0);
    idt_set_gate(248, (uint64_t)ISR248, 0x08, 0x8E, 0);
    idt_set_gate(249, (uint64_t)ISR249, 0x08, 0x8E, 0);
    idt_set_gate(250, (uint64_t)ISR250, 0x08, 0x8E, 0);
    idt_set_gate(251, (uint64_t)ISR251, 0x08, 0x8E, 0);
    idt_set_gate(252, (uint64_t)ISR252, 0x08, 0x8E, 0);
    idt_set_gate(253, (uint64_t)ISR253, 0x08, 0x8E, 0);
    idt_set_gate(254, (uint64_t)ISR254, 0x08, 0x8E, 0);
    idt_set_gate(255, (uint64_t)ISR255, 0x08, 0x8E, 0);

    idt_set_gate(32, (uint64_t)timer_irq, 0x08, 0x8E, 0);
    idt_set_gate(33, (uint64_t)keyboard_irq, 0x08, 0x8E, 0);

    idt_load();
    remap_pic(0x20, 0x28);
    init_pit(1000);
    __asm__ volatile ("sti");
}


volatile uint64_t timer_ticks = 0;

void timer_interrupt(uint64_t irq_n) {
    timer_ticks++;
    
    pic_send_eoi(irq_n);
}
static const char* const exceptions[] = {
    "Divide by zero error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception ",
    "",
    "",
    "",
    "",
    "",
    "",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    ""
};
void isr_handler(uint64_t exception_num) {
    tty_clear();
    gfx_fill_slow(0xFF0000);
    kprintf("\n{o}Exception:{r} {d}, {s}\n", 0x00FF00 ,exception_num, exceptions[exception_num]);
    hlt_loop();
}
