#ifndef SHELL_H
#define SHELL_H
#include "intr/keyboard.h"
#include "gol.h"
#include "fractal.h"
#include "minesweeper.h"
#include "matrix.h"
extern int32_t zig_add(int32_t a, int32_t b);

#define MAX_ARGS 8
int split_args(char *buf, char **argv);
void command_handler(char* buf);
void shell_loop();
#endif
