#include <drivers/input/keyboard.h>
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

struct KeyboardQueue keypress_queue = {
    .front = -1,
    .back = 0,
    .items = {0},
};
KeyboardModifier state = KEYBOARD_MODIFIER_NONE;

/* void keyboard_init() { */
/*     q->front = -1; */
/*     q->back = 0; */
/* } */
int keyboard_empty() {
    return keypress_queue.front == keypress_queue.back;
}

int keyboard_full() {
    return (keypress_queue.back + 1) % QCAP == keypress_queue.front;
}

void keyboard_push(char c) {
    if (keyboard_full()) {
        return; // queue full, drop input or handle error
    }
    keypress_queue.items[keypress_queue.back] = c;
    keypress_queue.back = (keypress_queue.back + 1) % QCAP;
}

char keyboard_pop() {
    if (keyboard_empty()) {
        return 0; // or some error value
    }
    char value = keypress_queue.items[keypress_queue.front];
    keypress_queue.front = (keypress_queue.front + 1) % QCAP;
    return value;
}

bool keyboard_has_key();

void keyboard_interrupt_handler() {
    uint8_t scancode = inb(0x60);
    if (scancode & 0x80) {
        if (scancode == 0xAA) {
            state = KEYBOARD_MODIFIER_NONE;
        }
    } else {
        if (scancode == 0x2A) {
            state = KEYBOARD_MODIFIER_SHIFT;
        } else {
            if (keyboard[scancode] != 0)
                keyboard_push(state == KEYBOARD_MODIFIER_SHIFT ? keyboard_shift[scancode] : keyboard[scancode]);
        }
         
    }
    pic_send_eoi(33);
}
char wait_for_key() {
    for (;;){
        if (!keyboard_empty()) {
            return keyboard_pop();
        }
        tty_update_cursor();
        asm("hlt");
    }
}
