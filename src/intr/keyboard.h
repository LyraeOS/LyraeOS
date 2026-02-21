#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "util.h"
#include "screen/tty.h"
#include "pic.h"
#include "liblyr.h"


typedef enum {
    NONE,
    SHIFT,
} KeyboardModifer;

#define QCAP 100

struct KeyboardQueue {
    char items[100];
    int back;
    int front;
};
extern struct KeyboardQueue keypress_queue;
void keyboard_init(struct KeyboardQueue *q);
int keyboard_empty(struct KeyboardQueue *q);
int keyboard_full(struct KeyboardQueue *q);
void keyboard_push(struct KeyboardQueue *q, char c);
char keyboard_pop(struct KeyboardQueue *q);
void keyboard_interrupt_handler();
char wait_for_key(struct KeyboardQueue *q);
#endif