#include "mem.h"

void init(struct Mem * m, int max_value){
	*(m->mem) = (struct Pair *) malloc(max_value * sizeof(struct Pair));
	m->len = 0;
	m->max = max_value;
}

unsigned int get_byte(struct Mem * m, unsigned int addr){
	int i;
	for(i = 0;i < m->len;i++){
		if((*(m->mem))[i].addr == addr){
			return (*(m->mem))[i].value & 0x000000ff;
		}
	}
	return 0;
}

void write_byte(struct Mem * m, unsigned int addr, unsigned int value){
	int i;
	for(i = 0;i < m->len;i++){
		if((*(m->mem))[i].addr == addr){
			(*(m->mem))[i].value = value;
		}
	}

	if(m->len < m->max){
		(*(m->mem))[m->len].addr = addr;
		(*(m->mem))[m->len].value = value;
		m->len = m->len + 1;
	} else {
		// double array length
		printf("double array length\n");
		struct Pair * tmp = (struct Pair *) malloc(m->max * 2 * sizeof(struct Pair));
		memcpy((void *) tmp, (void *) *(m->mem), sizeof(struct Pair) * m->max);
		free((void *) *(m->mem));
		*(m->mem) = tmp;
		m->max = m->max * 2;
		(*(m->mem))[m->len].addr = addr;
		(*(m->mem))[m->len].value = value;
		m->len = m->len + 1;
	}
}

unsigned int get_half_word(struct Mem * m, unsigned int addr){
	unsigned int high = get_byte(m, addr);
	unsigned int low = get_byte(m, addr + 1);
	return (unsigned int) (high << 8 | low);
}

unsigned int get_word(struct Mem * m, unsigned int addr){
	unsigned int a = get_byte(m, addr);
	unsigned int b = get_byte(m, addr + 1);
	unsigned int c = get_byte(m, addr + 2);
	unsigned int d = get_byte(m, addr + 3);
	return (unsigned int) (a << 24 | b << 16 | c << 8 | d);
}

void write_half_word(struct Mem * m, unsigned int addr, unsigned int value){
	unsigned int high = (unsigned int) (0x0000ff00 & value);
	unsigned int low = (unsigned int) (0x000000ff & value);
	write_byte(m, addr, high);
	write_byte(m, addr + 1, low);
}

void write_word(struct Mem * m, unsigned int addr, unsigned int value){
	unsigned int a = (unsigned int) (0xff000000 & value);
	unsigned int b = (unsigned int) (0x00ff0000 & value);
	unsigned int c = (unsigned int) (0x0000ff00 & value);
	unsigned int d = (unsigned int) (0x000000ff & value);

	write_byte(m, addr, a);
	write_byte(m, addr + 1, b);
	write_byte(m, addr + 2, c);
	write_byte(m, addr + 3, d);
}