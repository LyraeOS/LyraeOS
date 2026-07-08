#ifndef LYRAEOS_COMMAND_H
#define LYRAEOS_COMMAND_H

typedef int (*command_fn_t)(int argc, char **argv);

typedef struct {
    const char *name;
    const char *description;
    command_fn_t fn;
} command_t;

#define COMMAND(name, desc)                                      \
    static int cmd_##name(int argc, char **argv);                \
    static const command_t __cmd_##name                          \
    __attribute__((section(".commands"), used, aligned(8))) = {  \
        #name, desc, cmd_##name                                  \
    };                                                           \
    static int cmd_##name(int argc, char **argv)





#endif // LYRAEOS_COMMAND_H
