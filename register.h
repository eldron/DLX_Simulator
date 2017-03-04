#ifndef __register__h
#define __register__h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Registers{
	unsigned int reg[32];
};

void init_registers(struct Registers * regs);

unsigned int get_register_value(struct Registers * regs, unsigned int addr);

void write_register_value(struct Registers * regs, unsigned int addr, unsigned int value);
#endif