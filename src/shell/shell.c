#include "shell.h"

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

    if (strcmp(argv[0], "gol")) {
        if (argc < 2) {
            kprintf("Usage:\ngol [cell size > 2 ]\n");
        } else {
            int size = atoi(argv[1]);
            if (size < 2)
                size = 2;
            tty_set_cursor_enabled(false);
            ConwaysMain(size);
            tty_set_cursor_enabled(true);
            tty_clear();
        }
    } else if (strcmp(argv[0], "mine")) {
        tty_set_cursor_enabled(false);
        minesweeperMain();
        tty_set_cursor_enabled(true);
    } else if (strcmp(argv[0], "clear")) {
        tty_clear();
    } else if (strcmp(argv[0], "panic")) {
	*(volatile int *)0xDEADBEEF = 1;
    } else if (strcmp(argv[0], "echo")) {
        for (int i = 1; i < argc; i++) {
            kprintf("{s} ", argv[i]);
        }
        kprintf("\n");
    } else if (strcmp(argv[0], "serp")) {
        if (argc < 2) {
            kprintf("Usage:\nserp [iterations size >= 0 ]\n");
        } else {
            int size = atoi(argv[1]);
            if (size < 0)
                size = 0;
            ScreenSize sc = tty_get_screen_size();
            sierpinski(vec2_new(sc.x - 1, sc.y - 1), vec2_new((sc.x - 1) / 2, 1), vec2_new(1, sc.y - 1), size);
        }
    } else if (strcmp(argv[0], "matrix")) {
        tty_set_cursor_enabled(false);
        matrix();
        tty_set_cursor_enabled(true);
        tty_clear();
    } else if (strcmp(argv[0], "theme")) {
        if (argc < 2) {
            kprintf("Usage:\ntheme [light/dark]\n");
        } else {
            if (strcmp(argv[1], "light")) tty_change_theme(LYRAE_LIGHT); 
            if (strcmp(argv[1], "dark")) tty_change_theme(LYRAE_DARK); 
        }
    } else if (strcmp(argv[0], "scale")) {
      if (argc < 2) {
	kprintf("Usage:\nscale [scale factor as int]\n");
      } else {
            int size = atoi(argv[1]);
            if (size < 1)
                size = 1;
	    
	    gfx_update_scale(size);
      }
    } else if (!strcmp(argv[0], "")) {
      kprintf("{o}Unknown command: {s}{r}\n", tty_cur_theme()->error, argv[0]);
    }
}
void shell_loop()
{
    TTYTheme* cur_theme = tty_cur_theme();
    keyboard_pop(&keypress_queue);
    kprintf("Welcome to {o}LyraeOS{r}!\n", cur_theme->accent);
    kprintf("{o}kernel@lyraeos{r} $ ", cur_theme->accent);
    char command_buf[50];
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
