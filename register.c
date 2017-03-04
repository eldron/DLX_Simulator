#include "register.h"

void init_registers(struct Registers * regs){
	memset((void *) regs->reg, 0, 32 * sizeof(unsigned int));
}

unsigned int get_register_value(struct Registers * regs, unsigned int addr){
	if(0 <= addr && addr < 32){
		return regs->reg[addr];
	} else {
		printf("invalid address for register read\n");
		return 0;
	}
}

void write_register_value(struct Registers * regs, unsigned int addr, unsigned int value){
	if(0 < addr && addr < 32){
		regs->reg[addr] = value;
	} else {
		printf("invalid address for register write\n");
	}
}