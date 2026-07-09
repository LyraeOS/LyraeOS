#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <lib/util.h>
#include <drivers/display/tty.h>
#include <cpu/pic.h>
#include <stdbool.h>


typedef enum {
    KEYBOARD_MODIFIER_NONE,
    KEYBOARD_MODIFIER_SHIFT,
} KeyboardModifier;

#define QCAP 100

struct KeyboardQueue {
    char items[100];
    int back;
    int front;
};
void keyboard_init();
int keyboard_empty();
int keyboard_full();
void keyboard_push(char c);
char keyboard_pop();
void keyboard_interrupt_handler();
char wait_for_key();
#endif
