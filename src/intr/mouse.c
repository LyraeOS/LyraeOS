#include "mouse.h"
#include "screen/gfx.h"
#include "screen/tty.h"
#include "idt.h"

mouse_state_t mouse_state = {0};

void mouse_init() {
    outb(PS2_SC_REG, 0xA8); // enable mouse
    mouse_wait();

    // setup irq

    // request config byte
    outb(PS2_SC_REG, 0x20);
    // enable mouse interrupts
    uint8_t status = (inb(PS2_SC_REG) | 2);
    // define byte config
    outb(PS2_SC_REG, 0x60);
    outb(PS2_DATA, status);

    // send command to mouse
    outb(PS2_SC_REG, 0xD4);
    //enable streaming
    outb(0x60, 0xF4);
};

void mouse_handler() {
    mouse_state.data = inb(0x60);

    if (mouse_state.cycle == 0 && !(mouse_state.data & 0x08)) return;

    mouse_state.packet[mouse_state.cycle++] = mouse_state.data;

    if (mouse_state.cycle == 3) {
	mouse_state.cycle = 0;

	mouse_state.pressed = (mouse_state.packet[0] & 0x1);

	int dx = (int)(char)mouse_state.packet[1];
	int dy = (int)(char)mouse_state.packet[2];

	/* int px = mouse_state.x; */
	/* int py = mouse_state.y; */

	mouse_state.x += dx;
	mouse_state.y -= dy;

	ScreenSize ss = tty_get_screen_size();

	// clamp within screen
	// yes, i know its spaghetti, ill fix it later
	mouse_state.x = (mouse_state.x < 0) ? 0 : ((mouse_state.x > (int)ss.x - 1) ? (int)ss.x - 1 : mouse_state.x);
	mouse_state.y = (mouse_state.y < 0) ? 0 : ((mouse_state.y > (int)ss.y - 1) ? (int)ss.y - 1 : mouse_state.y);

	mouse_draw(0xffffff);
    }
}

// 9x9 square until fancier cursor
void mouse_draw(uint32_t color) {
    for (int x = 0; x < 9; x++) {
	for (int y = 0; y < 9; y++) {
	    gfx_set_pixel(mouse_state.x + x, mouse_state.y + y, color);
	}
    }
}

mouse_state_t* mouse_get_state() {
    return &mouse_state;
};
