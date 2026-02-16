#include "shell.h"
void shell_loop()
{
    tty_clear();
    keyboard_pop(&keypress_queue);
    kprintf("Welcome to {o}LyraeOS{r}!\n", 0xc603fc);
    kprintf("kernel@lyraeos $ ");
    char command_buf[50];
    for (;;) {
        if (!keyboard_empty(&keypress_queue))
        {
            char c = keyboard_pop(&keypress_queue);
            if (c == '\n') {
                kputchar('\n');
                if (strcmp(command_buf, "gol")) {
                    ConwaysMain();
                    tty_clear();
                }
                else if (strcmp(command_buf, "clear")) {
                    tty_clear();
                } else if (!strcmp(command_buf, "")) {
                    kprintf("Unknown command: {s}\n", command_buf);
                }
                kprintf("kernel@lyraeos $ ");
                memset(command_buf, 0, 50);
            } else if (c == '\x08') {
                if (kstrlen(command_buf) > 0) {
                    command_buf[kstrlen(command_buf)-1] = '\0';
                    tty_backspace();
                }
            } else
            {
                charcat(command_buf, c);
                kputchar(c);
            }
        }
    }
}