#include <shell/shell.h>
#include <kernel/command.h>

extern const command_t __commands_start[];
extern const command_t __commands_end[];
int split_args(char *buf, char **argv)
{
    int argc = 0;

    while (*buf && argc < MAX_ARGS)
    {
        while (*buf == ' ')
            buf++;
        if (!*buf)
            break;

        argv[argc++] = buf;

        while (*buf && *buf != ' ')
            buf++;
        if (*buf)
        {
            *buf = 0;
            buf++;
        }
    }

    return argc;
}
void command_handler(char *buf) {
    char *argv[MAX_ARGS];
    int argc = split_args(buf, argv);
    if (argc == 0)
        return;
    const command_t *cmd = NULL;
    for (const command_t *c = __commands_start; c < __commands_end; c++) {
        if (strcmp(c->name, argv[0])) {
            cmd = c;
            break;
        }
    }
    
    if (cmd) {
        cmd->fn(argc, argv);
    } else {
        kprintf("Unknown command: {s}\n", argv[0]);
    }
}
void shell_loop()
{
    TTYTheme* cur_theme = tty_cur_theme();
    keyboard_pop(&keypress_queue);
    kprintf("Welcome to {o}LyraeOS{r}!\n", cur_theme->accent);
    kprintf("Type {o}commands{r} for a list of commands.\n", cur_theme->accent);
    kprintf("And {o}help{r} for the list with descriptions.\n", cur_theme->accent);
    kprintf("{o}kernel@lyraeos{r} $ ", cur_theme->accent);
    char command_buf[50] = {0};
    for (;;)
    {
        char c = wait_for_key(&keypress_queue);
        if (c == '\n') {
            kputchar('\n');
            command_handler(command_buf);
            kprintf("{o}kernel@lyraeos{r} $ ", cur_theme->accent);
            memset(command_buf, 0, 50);
        } else if (c == '\x08') {
            if (kstrlen(command_buf) > 0) {
                command_buf[kstrlen(command_buf) - 1] = '\0';
                tty_backspace();
            }
        } else {
            charcat(command_buf, c);
            kputchar(c);
        }
    }
}

COMMAND(help, "gets command help") {
    for (const command_t* c = __commands_start; c < __commands_end; c++) {
        kprintf("{s} => {s}\n", c->name, c->description);
    }
    return 0;
}
COMMAND(commands, "lists all the commands") {
    for (const command_t* c = __commands_start; c < __commands_end; c++) {
        kprintf("{s} ", c->name);
    }
    kprintf("\ntype help for descriptions of the commands\n");
    return 0;
}
