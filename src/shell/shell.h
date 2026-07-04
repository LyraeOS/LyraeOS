#ifndef SHELL_H
#define SHELL_H
#include "intr/keyboard.h"
#include "programs/gol.h"
#include "programs/fractal.h"
#include "programs/minesweeper.h"
#include "programs/matrix.h"
#include "screen/gfx.h"

#define MAX_ARGS 8
int split_args(char *buf, char **argv);
void command_handler(char* buf);
void shell_loop();
#endif
