#include "idt.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

void idt_set_gate(u8 num, u64 base, u16 sel, u8 flags, u8 ist) {
    idt[num].offset_low  = base & 0xFFFF;
    idt[num].selector    = sel;
    idt[num].ist         = ist & 0x7;
    idt[num].type_attr   = flags;
    idt[num].offset_mid  = (base >> 16) & 0xFFFF;
    idt[num].offset_high = (base >> 32) & 0xFFFFFFFF;
    idt[num].zero        = 0;
}
void init_pit(u32 frequency) {
    u32 divisor = 1193180 / frequency;  // PIT base frequency
    
    outb(0x43, 0x36);  // Command: channel 0, lohi, rate generator
    outb(0x40, divisor & 0xFF);         // Low byte
    outb(0x40, (divisor >> 8) & 0xFF);  // High byte
}
void idt_install()
{
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (u64)&idt;

    memset(&idt, 0, sizeof(struct idt_entry) * 256);
    
    idt_set_gate(0, (u64)ISR0, 0x08, 0x8E, 0);
    idt_set_gate(1, (u64)ISR1, 0x08, 0x8E, 0);
    idt_set_gate(2, (u64)ISR2, 0x08, 0x8E, 0);
    idt_set_gate(3, (u64)ISR3, 0x08, 0x8E, 0);
    idt_set_gate(4, (u64)ISR4, 0x08, 0x8E, 0);
    idt_set_gate(5, (u64)ISR5, 0x08, 0x8E, 0);
    idt_set_gate(6, (u64)ISR6, 0x08, 0x8E, 0);
    idt_set_gate(7, (u64)ISR7, 0x08, 0x8E, 0);
    idt_set_gate(8, (u64)ISR8, 0x08, 0x8E, 0);
    idt_set_gate(9, (u64)ISR9, 0x08, 0x8E, 0);
    idt_set_gate(10, (u64)ISR10, 0x08, 0x8E, 0);
    idt_set_gate(11, (u64)ISR11, 0x08, 0x8E, 0);
    idt_set_gate(12, (u64)ISR12, 0x08, 0x8E, 0);
    idt_set_gate(13, (u64)ISR13, 0x08, 0x8E, 0);
    idt_set_gate(14, (u64)ISR14, 0x08, 0x8E, 0);
    idt_set_gate(15, (u64)ISR15, 0x08, 0x8E, 0);
    idt_set_gate(16, (u64)ISR16, 0x08, 0x8E, 0);
    idt_set_gate(17, (u64)ISR17, 0x08, 0x8E, 0);
    idt_set_gate(18, (u64)ISR18, 0x08, 0x8E, 0);
    idt_set_gate(19, (u64)ISR19, 0x08, 0x8E, 0);
    idt_set_gate(20, (u64)ISR20, 0x08, 0x8E, 0);
    idt_set_gate(21, (u64)ISR21, 0x08, 0x8E, 0);
    idt_set_gate(22, (u64)ISR22, 0x08, 0x8E, 0);
    idt_set_gate(23, (u64)ISR23, 0x08, 0x8E, 0);
    idt_set_gate(24, (u64)ISR24, 0x08, 0x8E, 0);
    idt_set_gate(25, (u64)ISR25, 0x08, 0x8E, 0);
    idt_set_gate(26, (u64)ISR26, 0x08, 0x8E, 0);
    idt_set_gate(27, (u64)ISR27, 0x08, 0x8E, 0);
    idt_set_gate(28, (u64)ISR28, 0x08, 0x8E, 0);
    idt_set_gate(29, (u64)ISR29, 0x08, 0x8E, 0);
    idt_set_gate(30, (u64)ISR30, 0x08, 0x8E, 0);
    idt_set_gate(31, (u64)ISR31, 0x08, 0x8E, 0);
    idt_set_gate(34, (u64)ISR34, 0x08, 0x8E, 0);
    idt_set_gate(35, (u64)ISR35, 0x08, 0x8E, 0);
    idt_set_gate(36, (u64)ISR36, 0x08, 0x8E, 0);
    idt_set_gate(37, (u64)ISR37, 0x08, 0x8E, 0);
    idt_set_gate(38, (u64)ISR38, 0x08, 0x8E, 0);
    idt_set_gate(39, (u64)ISR39, 0x08, 0x8E, 0);
    idt_set_gate(40, (u64)ISR40, 0x08, 0x8E, 0);
    idt_set_gate(41, (u64)ISR41, 0x08, 0x8E, 0);
    idt_set_gate(42, (u64)ISR42, 0x08, 0x8E, 0);
    idt_set_gate(43, (u64)ISR43, 0x08, 0x8E, 0);
    idt_set_gate(44, (u64)ISR44, 0x08, 0x8E, 0);
    idt_set_gate(45, (u64)ISR45, 0x08, 0x8E, 0);
    idt_set_gate(46, (u64)ISR46, 0x08, 0x8E, 0);
    idt_set_gate(47, (u64)ISR47, 0x08, 0x8E, 0);
    idt_set_gate(48, (u64)ISR48, 0x08, 0x8E, 0);
    idt_set_gate(49, (u64)ISR49, 0x08, 0x8E, 0);
    idt_set_gate(50, (u64)ISR50, 0x08, 0x8E, 0);
    idt_set_gate(51, (u64)ISR51, 0x08, 0x8E, 0);
    idt_set_gate(52, (u64)ISR52, 0x08, 0x8E, 0);
    idt_set_gate(53, (u64)ISR53, 0x08, 0x8E, 0);
    idt_set_gate(54, (u64)ISR54, 0x08, 0x8E, 0);
    idt_set_gate(55, (u64)ISR55, 0x08, 0x8E, 0);
    idt_set_gate(56, (u64)ISR56, 0x08, 0x8E, 0);
    idt_set_gate(57, (u64)ISR57, 0x08, 0x8E, 0);
    idt_set_gate(58, (u64)ISR58, 0x08, 0x8E, 0);
    idt_set_gate(59, (u64)ISR59, 0x08, 0x8E, 0);
    idt_set_gate(60, (u64)ISR60, 0x08, 0x8E, 0);
    idt_set_gate(61, (u64)ISR61, 0x08, 0x8E, 0);
    idt_set_gate(62, (u64)ISR62, 0x08, 0x8E, 0);
    idt_set_gate(63, (u64)ISR63, 0x08, 0x8E, 0);
    idt_set_gate(64, (u64)ISR64, 0x08, 0x8E, 0);
    idt_set_gate(65, (u64)ISR65, 0x08, 0x8E, 0);
    idt_set_gate(66, (u64)ISR66, 0x08, 0x8E, 0);
    idt_set_gate(67, (u64)ISR67, 0x08, 0x8E, 0);
    idt_set_gate(68, (u64)ISR68, 0x08, 0x8E, 0);
    idt_set_gate(69, (u64)ISR69, 0x08, 0x8E, 0);
    idt_set_gate(70, (u64)ISR70, 0x08, 0x8E, 0);
    idt_set_gate(71, (u64)ISR71, 0x08, 0x8E, 0);
    idt_set_gate(72, (u64)ISR72, 0x08, 0x8E, 0);
    idt_set_gate(73, (u64)ISR73, 0x08, 0x8E, 0);
    idt_set_gate(74, (u64)ISR74, 0x08, 0x8E, 0);
    idt_set_gate(75, (u64)ISR75, 0x08, 0x8E, 0);
    idt_set_gate(76, (u64)ISR76, 0x08, 0x8E, 0);
    idt_set_gate(77, (u64)ISR77, 0x08, 0x8E, 0);
    idt_set_gate(78, (u64)ISR78, 0x08, 0x8E, 0);
    idt_set_gate(79, (u64)ISR79, 0x08, 0x8E, 0);
    idt_set_gate(80, (u64)ISR80, 0x08, 0x8E, 0);
    idt_set_gate(81, (u64)ISR81, 0x08, 0x8E, 0);
    idt_set_gate(82, (u64)ISR82, 0x08, 0x8E, 0);
    idt_set_gate(83, (u64)ISR83, 0x08, 0x8E, 0);
    idt_set_gate(84, (u64)ISR84, 0x08, 0x8E, 0);
    idt_set_gate(85, (u64)ISR85, 0x08, 0x8E, 0);
    idt_set_gate(86, (u64)ISR86, 0x08, 0x8E, 0);
    idt_set_gate(87, (u64)ISR87, 0x08, 0x8E, 0);
    idt_set_gate(88, (u64)ISR88, 0x08, 0x8E, 0);
    idt_set_gate(89, (u64)ISR89, 0x08, 0x8E, 0);
    idt_set_gate(90, (u64)ISR90, 0x08, 0x8E, 0);
    idt_set_gate(91, (u64)ISR91, 0x08, 0x8E, 0);
    idt_set_gate(92, (u64)ISR92, 0x08, 0x8E, 0);
    idt_set_gate(93, (u64)ISR93, 0x08, 0x8E, 0);
    idt_set_gate(94, (u64)ISR94, 0x08, 0x8E, 0);
    idt_set_gate(95, (u64)ISR95, 0x08, 0x8E, 0);
    idt_set_gate(96, (u64)ISR96, 0x08, 0x8E, 0);
    idt_set_gate(97, (u64)ISR97, 0x08, 0x8E, 0);
    idt_set_gate(98, (u64)ISR98, 0x08, 0x8E, 0);
    idt_set_gate(99, (u64)ISR99, 0x08, 0x8E, 0);
    idt_set_gate(100, (u64)ISR100, 0x08, 0x8E, 0);
    idt_set_gate(101, (u64)ISR101, 0x08, 0x8E, 0);
    idt_set_gate(102, (u64)ISR102, 0x08, 0x8E, 0);
    idt_set_gate(103, (u64)ISR103, 0x08, 0x8E, 0);
    idt_set_gate(104, (u64)ISR104, 0x08, 0x8E, 0);
    idt_set_gate(105, (u64)ISR105, 0x08, 0x8E, 0);
    idt_set_gate(106, (u64)ISR106, 0x08, 0x8E, 0);
    idt_set_gate(107, (u64)ISR107, 0x08, 0x8E, 0);
    idt_set_gate(108, (u64)ISR108, 0x08, 0x8E, 0);
    idt_set_gate(109, (u64)ISR109, 0x08, 0x8E, 0);
    idt_set_gate(110, (u64)ISR110, 0x08, 0x8E, 0);
    idt_set_gate(111, (u64)ISR111, 0x08, 0x8E, 0);
    idt_set_gate(112, (u64)ISR112, 0x08, 0x8E, 0);
    idt_set_gate(113, (u64)ISR113, 0x08, 0x8E, 0);
    idt_set_gate(114, (u64)ISR114, 0x08, 0x8E, 0);
    idt_set_gate(115, (u64)ISR115, 0x08, 0x8E, 0);
    idt_set_gate(116, (u64)ISR116, 0x08, 0x8E, 0);
    idt_set_gate(117, (u64)ISR117, 0x08, 0x8E, 0);
    idt_set_gate(118, (u64)ISR118, 0x08, 0x8E, 0);
    idt_set_gate(119, (u64)ISR119, 0x08, 0x8E, 0);
    idt_set_gate(120, (u64)ISR120, 0x08, 0x8E, 0);
    idt_set_gate(121, (u64)ISR121, 0x08, 0x8E, 0);
    idt_set_gate(122, (u64)ISR122, 0x08, 0x8E, 0);
    idt_set_gate(123, (u64)ISR123, 0x08, 0x8E, 0);
    idt_set_gate(124, (u64)ISR124, 0x08, 0x8E, 0);
    idt_set_gate(125, (u64)ISR125, 0x08, 0x8E, 0);
    idt_set_gate(126, (u64)ISR126, 0x08, 0x8E, 0);
    idt_set_gate(127, (u64)ISR127, 0x08, 0x8E, 0);
    idt_set_gate(128, (u64)ISR128, 0x08, 0x8E, 0);
    idt_set_gate(129, (u64)ISR129, 0x08, 0x8E, 0);
    idt_set_gate(130, (u64)ISR130, 0x08, 0x8E, 0);
    idt_set_gate(131, (u64)ISR131, 0x08, 0x8E, 0);
    idt_set_gate(132, (u64)ISR132, 0x08, 0x8E, 0);
    idt_set_gate(133, (u64)ISR133, 0x08, 0x8E, 0);
    idt_set_gate(134, (u64)ISR134, 0x08, 0x8E, 0);
    idt_set_gate(135, (u64)ISR135, 0x08, 0x8E, 0);
    idt_set_gate(136, (u64)ISR136, 0x08, 0x8E, 0);
    idt_set_gate(137, (u64)ISR137, 0x08, 0x8E, 0);
    idt_set_gate(138, (u64)ISR138, 0x08, 0x8E, 0);
    idt_set_gate(139, (u64)ISR139, 0x08, 0x8E, 0);
    idt_set_gate(140, (u64)ISR140, 0x08, 0x8E, 0);
    idt_set_gate(141, (u64)ISR141, 0x08, 0x8E, 0);
    idt_set_gate(142, (u64)ISR142, 0x08, 0x8E, 0);
    idt_set_gate(143, (u64)ISR143, 0x08, 0x8E, 0);
    idt_set_gate(144, (u64)ISR144, 0x08, 0x8E, 0);
    idt_set_gate(145, (u64)ISR145, 0x08, 0x8E, 0);
    idt_set_gate(146, (u64)ISR146, 0x08, 0x8E, 0);
    idt_set_gate(147, (u64)ISR147, 0x08, 0x8E, 0);
    idt_set_gate(148, (u64)ISR148, 0x08, 0x8E, 0);
    idt_set_gate(149, (u64)ISR149, 0x08, 0x8E, 0);
    idt_set_gate(150, (u64)ISR150, 0x08, 0x8E, 0);
    idt_set_gate(151, (u64)ISR151, 0x08, 0x8E, 0);
    idt_set_gate(152, (u64)ISR152, 0x08, 0x8E, 0);
    idt_set_gate(153, (u64)ISR153, 0x08, 0x8E, 0);
    idt_set_gate(154, (u64)ISR154, 0x08, 0x8E, 0);
    idt_set_gate(155, (u64)ISR155, 0x08, 0x8E, 0);
    idt_set_gate(156, (u64)ISR156, 0x08, 0x8E, 0);
    idt_set_gate(157, (u64)ISR157, 0x08, 0x8E, 0);
    idt_set_gate(158, (u64)ISR158, 0x08, 0x8E, 0);
    idt_set_gate(159, (u64)ISR159, 0x08, 0x8E, 0);
    idt_set_gate(160, (u64)ISR160, 0x08, 0x8E, 0);
    idt_set_gate(161, (u64)ISR161, 0x08, 0x8E, 0);
    idt_set_gate(162, (u64)ISR162, 0x08, 0x8E, 0);
    idt_set_gate(163, (u64)ISR163, 0x08, 0x8E, 0);
    idt_set_gate(164, (u64)ISR164, 0x08, 0x8E, 0);
    idt_set_gate(165, (u64)ISR165, 0x08, 0x8E, 0);
    idt_set_gate(166, (u64)ISR166, 0x08, 0x8E, 0);
    idt_set_gate(167, (u64)ISR167, 0x08, 0x8E, 0);
    idt_set_gate(168, (u64)ISR168, 0x08, 0x8E, 0);
    idt_set_gate(169, (u64)ISR169, 0x08, 0x8E, 0);
    idt_set_gate(170, (u64)ISR170, 0x08, 0x8E, 0);
    idt_set_gate(171, (u64)ISR171, 0x08, 0x8E, 0);
    idt_set_gate(172, (u64)ISR172, 0x08, 0x8E, 0);
    idt_set_gate(173, (u64)ISR173, 0x08, 0x8E, 0);
    idt_set_gate(174, (u64)ISR174, 0x08, 0x8E, 0);
    idt_set_gate(175, (u64)ISR175, 0x08, 0x8E, 0);
    idt_set_gate(176, (u64)ISR176, 0x08, 0x8E, 0);
    idt_set_gate(177, (u64)ISR177, 0x08, 0x8E, 0);
    idt_set_gate(178, (u64)ISR178, 0x08, 0x8E, 0);
    idt_set_gate(179, (u64)ISR179, 0x08, 0x8E, 0);
    idt_set_gate(180, (u64)ISR180, 0x08, 0x8E, 0);
    idt_set_gate(181, (u64)ISR181, 0x08, 0x8E, 0);
    idt_set_gate(182, (u64)ISR182, 0x08, 0x8E, 0);
    idt_set_gate(183, (u64)ISR183, 0x08, 0x8E, 0);
    idt_set_gate(184, (u64)ISR184, 0x08, 0x8E, 0);
    idt_set_gate(185, (u64)ISR185, 0x08, 0x8E, 0);
    idt_set_gate(186, (u64)ISR186, 0x08, 0x8E, 0);
    idt_set_gate(187, (u64)ISR187, 0x08, 0x8E, 0);
    idt_set_gate(188, (u64)ISR188, 0x08, 0x8E, 0);
    idt_set_gate(189, (u64)ISR189, 0x08, 0x8E, 0);
    idt_set_gate(190, (u64)ISR190, 0x08, 0x8E, 0);
    idt_set_gate(191, (u64)ISR191, 0x08, 0x8E, 0);
    idt_set_gate(192, (u64)ISR192, 0x08, 0x8E, 0);
    idt_set_gate(193, (u64)ISR193, 0x08, 0x8E, 0);
    idt_set_gate(194, (u64)ISR194, 0x08, 0x8E, 0);
    idt_set_gate(195, (u64)ISR195, 0x08, 0x8E, 0);
    idt_set_gate(196, (u64)ISR196, 0x08, 0x8E, 0);
    idt_set_gate(197, (u64)ISR197, 0x08, 0x8E, 0);
    idt_set_gate(198, (u64)ISR198, 0x08, 0x8E, 0);
    idt_set_gate(199, (u64)ISR199, 0x08, 0x8E, 0);
    idt_set_gate(200, (u64)ISR200, 0x08, 0x8E, 0);
    idt_set_gate(201, (u64)ISR201, 0x08, 0x8E, 0);
    idt_set_gate(202, (u64)ISR202, 0x08, 0x8E, 0);
    idt_set_gate(203, (u64)ISR203, 0x08, 0x8E, 0);
    idt_set_gate(204, (u64)ISR204, 0x08, 0x8E, 0);
    idt_set_gate(205, (u64)ISR205, 0x08, 0x8E, 0);
    idt_set_gate(206, (u64)ISR206, 0x08, 0x8E, 0);
    idt_set_gate(207, (u64)ISR207, 0x08, 0x8E, 0);
    idt_set_gate(208, (u64)ISR208, 0x08, 0x8E, 0);
    idt_set_gate(209, (u64)ISR209, 0x08, 0x8E, 0);
    idt_set_gate(210, (u64)ISR210, 0x08, 0x8E, 0);
    idt_set_gate(211, (u64)ISR211, 0x08, 0x8E, 0);
    idt_set_gate(212, (u64)ISR212, 0x08, 0x8E, 0);
    idt_set_gate(213, (u64)ISR213, 0x08, 0x8E, 0);
    idt_set_gate(214, (u64)ISR214, 0x08, 0x8E, 0);
    idt_set_gate(215, (u64)ISR215, 0x08, 0x8E, 0);
    idt_set_gate(216, (u64)ISR216, 0x08, 0x8E, 0);
    idt_set_gate(217, (u64)ISR217, 0x08, 0x8E, 0);
    idt_set_gate(218, (u64)ISR218, 0x08, 0x8E, 0);
    idt_set_gate(219, (u64)ISR219, 0x08, 0x8E, 0);
    idt_set_gate(220, (u64)ISR220, 0x08, 0x8E, 0);
    idt_set_gate(221, (u64)ISR221, 0x08, 0x8E, 0);
    idt_set_gate(222, (u64)ISR222, 0x08, 0x8E, 0);
    idt_set_gate(223, (u64)ISR223, 0x08, 0x8E, 0);
    idt_set_gate(224, (u64)ISR224, 0x08, 0x8E, 0);
    idt_set_gate(225, (u64)ISR225, 0x08, 0x8E, 0);
    idt_set_gate(226, (u64)ISR226, 0x08, 0x8E, 0);
    idt_set_gate(227, (u64)ISR227, 0x08, 0x8E, 0);
    idt_set_gate(228, (u64)ISR228, 0x08, 0x8E, 0);
    idt_set_gate(229, (u64)ISR229, 0x08, 0x8E, 0);
    idt_set_gate(230, (u64)ISR230, 0x08, 0x8E, 0);
    idt_set_gate(231, (u64)ISR231, 0x08, 0x8E, 0);
    idt_set_gate(232, (u64)ISR232, 0x08, 0x8E, 0);
    idt_set_gate(233, (u64)ISR233, 0x08, 0x8E, 0);
    idt_set_gate(234, (u64)ISR234, 0x08, 0x8E, 0);
    idt_set_gate(235, (u64)ISR235, 0x08, 0x8E, 0);
    idt_set_gate(236, (u64)ISR236, 0x08, 0x8E, 0);
    idt_set_gate(237, (u64)ISR237, 0x08, 0x8E, 0);
    idt_set_gate(238, (u64)ISR238, 0x08, 0x8E, 0);
    idt_set_gate(239, (u64)ISR239, 0x08, 0x8E, 0);
    idt_set_gate(240, (u64)ISR240, 0x08, 0x8E, 0);
    idt_set_gate(241, (u64)ISR241, 0x08, 0x8E, 0);
    idt_set_gate(242, (u64)ISR242, 0x08, 0x8E, 0);
    idt_set_gate(243, (u64)ISR243, 0x08, 0x8E, 0);
    idt_set_gate(244, (u64)ISR244, 0x08, 0x8E, 0);
    idt_set_gate(245, (u64)ISR245, 0x08, 0x8E, 0);
    idt_set_gate(246, (u64)ISR246, 0x08, 0x8E, 0);
    idt_set_gate(247, (u64)ISR247, 0x08, 0x8E, 0);
    idt_set_gate(248, (u64)ISR248, 0x08, 0x8E, 0);
    idt_set_gate(249, (u64)ISR249, 0x08, 0x8E, 0);
    idt_set_gate(250, (u64)ISR250, 0x08, 0x8E, 0);
    idt_set_gate(251, (u64)ISR251, 0x08, 0x8E, 0);
    idt_set_gate(252, (u64)ISR252, 0x08, 0x8E, 0);
    idt_set_gate(253, (u64)ISR253, 0x08, 0x8E, 0);
    idt_set_gate(254, (u64)ISR254, 0x08, 0x8E, 0);
    idt_set_gate(255, (u64)ISR255, 0x08, 0x8E, 0);

    idt_set_gate(32, (u64)timer_irq, 0x08, 0x8E, 0);
    idt_set_gate(33, (u64)keyboard_irq, 0x08, 0x8E, 0);

    idt_load();
    remap_pic(0x20, 0x28);
    init_pit(1000);
    __asm__ volatile ("sti");
}


volatile u64 timer_ticks = 0;

void timer_interrupt(u64 irq_n) {
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
void isr_handler(u64 exception_num) {
    tty_clear();
    gfx_fill_slow(0xFF0000);
    kprintf("\n{o}Exception:{r} {d}, {s}\n", 0x00FF00 ,exception_num, exceptions[exception_num]);
    hlt_loop();
}
