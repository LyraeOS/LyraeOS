#ifndef SHELL_H
#define SHELL_H
#include "intr/keyboard.h"
#include "gol.h"
#include "fractal.h"
#include "minesweeper.h"

#define MAX_ARGS 8
int split_args(char *buf, char **argv);
void command_handler(char* buf);
void shell_loop();
#endif