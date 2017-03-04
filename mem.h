#ifndef __mem__h
#define __mem__h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Pair{
	unsigned int addr;
	unsigned int value;
};

struct Mem{
	struct Pair ** mem;
	int max;
	int len;
};

void init(struct Mem * m, int max_value);

unsigned int get_byte(struct Mem * m, unsigned int addr);

void write_byte(struct Mem * m, unsigned int addr, unsigned int value);

unsigned int get_half_word(struct Mem * m, unsigned int addr);

unsigned int get_word(struct Mem * m, unsigned int addr);

void write_half_word(struct Mem * m, unsigned int addr, unsigned int value);

void write_word(struct Mem * m, unsigned int addr, unsigned int value);
#endif