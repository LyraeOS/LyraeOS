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
    keyboard_pop();
    kprintf("Welcome to {accent}LyraeOS{reset}!\n");
    kprintf("Type {accent}commands{reset} for a list of commands.\n");
    kprintf("And {accent}help{reset} for the list with descriptions.\n");
    char command_buf[129] = {0};

    for (;;) {
        kprintf("{accent}kernel@lyraeos{reset} $ ");
        char* command = tty_read_line(command_buf);
        command_handler(command);
        memset(command_buf, 0, 129);
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
