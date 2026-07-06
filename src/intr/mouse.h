#ifndef LYRAEOS_MOUSE_H
#define LYRAEOS_MOUSE_H
#include <stddef.h>
#include <stdint.h>
#include "util.h"

#define PS2_SC_REG 0x64 // status & command register
#define PS2_DATA 0x60

typedef struct {
    int x;
    int y;
    bool pressed;

    uint8_t data;
    uint8_t cycle;
    uint8_t packet[3];
} mouse_state_t;

void mouse_init();
void mouse_handler();
void mouse_draw(uint32_t color);
mouse_state_t* mouse_get_state();

inline void mouse_wait() { while (inb(PS2_SC_REG) & 1); }

#endif // LYRAEOS_MOUSE_H
