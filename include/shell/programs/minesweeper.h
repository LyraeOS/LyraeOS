#ifndef MINE_H
#define MINE_H
#include <stdbool.h>
#include <mm/mem.h>
#include <drivers/display/gfx.h>
#include <drivers/display/tty.h>
#include <lib/util.h>
#include <cpu/idt.h>
#include <shell/programs/gol.h>

void minesweeperMain(void);

#endif
