#include "keyboard.h"
char keyboard[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\x08',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
	'\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9',
	'-', '4', '5', '6', '+', '1', '2', '3', '0', '.'
};
char keyboard_shift[] = {
	0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\x08',
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
	'\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',
	0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9',
	'-', '4', '5', '6', '+', '1', '2', '3', '0', '.'
};
void keyboard_init(struct KeyboardQueue *q) {
    q->front = -1;
    q->back = 0;
}
int keyboard_empty(struct KeyboardQueue *q) {
    return q->front == q->back;
}

int keyboard_full(struct KeyboardQueue *q) {
    return (q->back + 1) % QCAP == q->front;
}

void keyboard_push(struct KeyboardQueue *q, char c) {
    if (keyboard_full(q)) {
        return; // queue full, drop input or handle error
    }
    q->items[q->back] = c;
    q->back = (q->back + 1) % QCAP;
}

char keyboard_pop(struct KeyboardQueue *q) {
    if (keyboard_empty(q)) {
        return 0; // or some error value
    }
    char value = q->items[q->front];
    q->front = (q->front + 1) % QCAP;
    return value;
}


KeyboardModifer state = NONE;
struct KeyboardQueue keypress_queue = {0};
void keyboard_interrupt_handler() {
    u8 scancode = inb(0x60);
    if (scancode & 0x80) {
        if (scancode == 0xAA) {
            state = NONE;
        }
    } else {
        if (scancode == 0x2A) {
            state = SHIFT;
        } else {
            if (keyboard[scancode] != 0)
                keyboard_push(&keypress_queue, state == SHIFT ? keyboard_shift[scancode] : keyboard[scancode]);
        }
         
    }
    pic_send_eoi(33);
}
char wait_for_key(struct KeyboardQueue *q) {
    for (;;){
        if (!keyboard_empty(q)) {
            return keyboard_pop(q);
        }
        tty_update_cursor();
        asm("hlt");
    }
}