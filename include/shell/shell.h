#ifndef SHELL_H
#define SHELL_H
#include <drivers/input/keyboard.h>
#include <drivers/display/gfx.h>
#include <shell/programs/gol.h>
#include <shell/programs/fractal.h>
#include <shell/programs/minesweeper.h>
#include <shell/programs/matrix.h>

#define MAX_ARGS 8
int split_args(char *buf, char **argv);
void command_handler(char* buf);
void shell_loop();
#endif
