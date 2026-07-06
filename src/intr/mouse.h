#ifndef LYRAEOS_MOUSE_H
#define LYRAEOS_MOUSE_H
#include <stddef.h>
#include <stdint.h>
#include "util.h"

#define PS2_SC_REG 0x64 // status & command register
#define PS2_DATA 0x60

#define CURSOR_WIDTH 10
#define CURSOR_HEIGHT 10

typedef struct {
    int x;
    int y;
    bool pressed;

    uint8_t data;
    uint8_t cycle;
    uint8_t packet[3];
    bool enabled;
} mouse_state_t;

typedef enum {
  PS2_WAIT_READ,
  PS2_WAIT_WRITE,
} ps2_wait_type_t;

void mouse_init();
void mouse_handler();
void mouse_draw(uint32_t color);
void mouse_save_position();
void mouse_clear_trail(int px, int py);
void mouse_set_enable(bool en);
mouse_state_t* mouse_get_state();

inline void mouse_wait(ps2_wait_type_t wait_type) {
    switch (wait_type) {
        case PS2_WAIT_READ:
            while (inb(PS2_SC_REG) & 0x02);
            break;
        case PS2_WAIT_WRITE:
            while (inb(PS2_SC_REG) & 0x01);
            break;
    }
}

#endif // LYRAEOS_MOUSE_H
