#ifndef SERIAL_H
#define SERIAL_H
#include <lib/util.h>
#define PORT 0x3f8

int init_serial();
int is_transmit_empty();

void write_serial(char a);


#endif // SERIAL_H
