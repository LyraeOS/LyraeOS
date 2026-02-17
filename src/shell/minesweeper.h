#ifndef MINE_H
#define MINE_H
#include <stdbool.h>
#include "mem.h"
#include "screen/gfx.h"
#include "util.h"
#include "intr/idt.h"
#include "screen/tty.h"
#include "gol.h"

void minesweeperMain(void);

#endif