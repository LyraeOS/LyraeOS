#ifndef FRACTAL_H
#define FRACTAL_H
#include <stddef.h>
#include <stdint.h>
#include "screen/gfx.h"
#include "intr/idt.h"
void sierpinski(vec2 p1, vec2 p2, vec2 p3, int depth);
#endif
