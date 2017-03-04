#include "instruction.h"

extern int PC;
extern struct Registers regs;
extern struct Mem mem;
extern int halt_flag;
extern FILE * out_file;
extern int ins_count;
extern struct Instruction * ins;
extern int debug_flag;

void lb_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	int rs1_value = (int) get_register_value(&regs, rs1);
	int addr = rs1_value + imm;
	unsigned value = get_byte(&mem, addr);
	write_register_value(&regs, rd, value);
	PC += 4;
	if(debug_flag){
		printf("lb r%d r%d %d, rs1_value = %d result = %d\n", rd, rs1, imm, rs1_value, value);
	}
}

void lw_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	int rs1_value = (int) get_register_value(&regs, rs1);
	int addr = rs1_value + imm;
	unsigned value = get_word(&mem, addr);
	write_register_value(&regs, rd, value);
	PC += 4;
	if(debug_flag){
		printf("lw r%d r%d %d, rs1_value = %d result = %d\n", rd, rs1, imm, rs1_value, value);
	}
}

void sb_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	int rs1_value = (int) get_register_value(&regs, rs1);
	int addr = rs1_value + imm;
	write_byte(&mem, addr, get_register_value(&regs, rd) & 0x000000ff);
	PC += 4;
	if(debug_flag){
		printf("sb r%d r%d %d, rs1_value = %d, result = %d\n", rd, rs1, imm, rs1_value, get_register_value(&regs, rd) & 0x000000ff);
	}
}

void sw_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	int rs1_value = (int) get_register_value(&regs, rs1);
	int addr = rs1_value + imm;
	write_word(&mem, addr, get_register_value(&regs, rd));
	PC +=4;
	if(debug_flag){
		printf("sw r%d r%d %d, rs1_value = %d, result = %d\n", rd, rs1, imm, rs1_value, get_register_value(&regs, rd));
	}
}

void addi_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	int rs1_value = (int) get_register_value(&regs, rs1);
	int result = rs1_value + imm;
	write_register_value(&regs, rd, (unsigned int) result);
	if(debug_flag){
		printf("addi r%d r%d %d: %d %d %d\n", rd, rs1, imm, result, rs1_value, imm);
	}
	PC += 4;
}

void subi_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	int rs1_value = (int) get_register_value(&regs, rs1);
	int result = rs1_value - imm;
	write_register_value(&regs, rd, (unsigned int) result);
	PC += 4;
	if(debug_flag){
		printf("subi r%d r%d %d: %d %d %d\n", rd, rs1, imm, result, rs1_value, imm);
	}
}

void andi_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned rs1_value = get_register_value(&regs, rs1);
	unsigned int result = rs1_value & ((unsigned int) imm);
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("andi r%d r%d %d: %d %d %d\n", rd, rs1, imm, result, rs1_value, imm);
	}
}

void ori_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned rs1_value = get_register_value(&regs, rs1);
	unsigned int result = rs1_value | ((unsigned int) imm);
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("ori r%d r%d %d: %d %d %d\n", rd, rs1, imm, result, rs1_value, imm);
	}
}

void xori_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int result = rs1_value ^ ((unsigned int) imm);
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("xori r%d r%d %d: %d %d %d\n", rd, rs1, imm, result, rs1_value, imm);
	}
}

void sgti_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	int rs1_value = (int) get_register_value(&regs, rs1);
	int result = rs1_value > imm ? 1 : 0;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("sgti r%d r%d %d: %d %d %d\n", rd, rs1, imm, result, rs1_value, imm);
	}
}

void seqi_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	int rs1_value = (int) get_register_value(&regs, rs1);
	int result = rs1_value == imm ? 1 : 0;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("seqi r%d r%d %d: %d %d %d\n", rd, rs1, imm, result, rs1_value, imm);
	}
}

void sgei_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	int rs1_value = (int) get_register_value(&regs, rs1);
	int result = rs1_value >= imm ? 1 : 0;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("sgei r%d r%d %d: %d %d %d\n", rd, rs1, imm, result, rs1_value, imm);
	}
}

void slti_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	int rs1_value = (int) get_register_value(&regs, rs1);
	int result = rs1_value < imm ? 1 : 0;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("slti r%d r%d %d: %d %d %d\n", rd, rs1, imm, result, rs1_value, imm);
	}
}

void snei_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	int rs1_value = (int) get_register_value(&regs, rs1);
	int result = rs1_value != imm ? 1 : 0;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("snei r%d r%d %d: %d %d %d\n", rd, rs1, imm, result, rs1_value, imm);
	}
}

void slei_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	int rs1_value = (int) get_register_value(&regs, rs1);
	int result = rs1_value <= imm ? 1 : 0;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("slei r%d r%d %d: %d %d %d\n", rd, rs1, imm, result, rs1_value, imm);
	}
}

void beqz_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	if(debug_flag){
		if(rs2 == 0){
			printf("beqz r%d %d\n, rs1_value = %d\n", rs1, imm, rs1_value);
		} else {
			printf("beqz r%d %s, rs1_value = %d\n", rs1, branch_label, rs1_value);
		}
	}
	if(rs2 == 0){
		PC = PC + 4 + (rs1_value == 0 ? imm : 0);
	} else {
		int i;
		int flag = 0;
		for(i = 0;i < ins_count;i++){
			if(strcmp(ins[i].label, branch_label) == 0){
				flag = 1;
				break;
			}
		}
		if(flag){
			if(rs1_value == 0){
				PC = i * 4;
			} else {
				PC += 4;
			}
		} else {
			printf("BEQZ error: can not find branch label %s\n", branch_label);
		}
	}
}

void bnez_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	if(debug_flag){
		if(rs2 == 0){
			printf("bnez r%d %d\n, rs1_value = %d\n", rs1, imm, rs1_value);
		} else {
			printf("bnez r%d %s, rs1_value = %d\n", rs1, branch_label, rs1_value);
		}
	}
	if(rs2 == 0){
		PC = PC + 4 + (rs1_value != 0 ? imm : 0);
	} else {
		int i;
		int flag = 0;
		for(i = 0;i < ins_count;i++){
			if(strcmp(ins[i].label, branch_label) == 0){
				flag = 1;
				break;
			}
		}
		if(flag){
			if(rs1_value != 0){
				PC = i * 4;
			} else {
				PC += 4;
			}
		} else {
			printf("BNEZ error: can not find branch label %s\n", branch_label);
		}
	}
}

void jr_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	if(debug_flag){
		if(rs2 == 0){
			printf("jr r%d\n", rs1);
		} else {
			printf("jr %s\n", branch_label);
		}
	}
	if(rs2 == 0){
		PC = get_register_value(&regs, rs1);
	} else {
		// jump to branch label
		int i;
		int flag = 0;
		for(i = 0;i < ins_count;i++){
			if(strcmp(ins[i].label, branch_label) == 0){
				flag = 1;
				break;
			}
		}
		if(flag){
			PC = i * 4;
		} else {
			printf("JR error: can not find branch label %s\n", branch_label);
		}
	}
}

void add_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int rs2_value = get_register_value(&regs, rs2);
	unsigned int result = rs1_value + rs2_value;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("add r%d r%d r%d: %d %d %d\n", rd, rs1, rs2, result, rs1_value, rs2_value);
	}
}

void sub_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int rs2_value = get_register_value(&regs, rs2);
	unsigned int result  = rs1_value- rs2_value;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("sub r%d r%d r%d: %d %d %d\n", rd, rs1, rs2, result, rs1_value, rs2_value);
	}
}

void and_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int rs2_value = get_register_value(&regs, rs2);
	unsigned int result = rs1_value & rs2_value;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("and r%d r%d r%d: %d %d %d\n", rd, rs1, rs2, result, rs1_value, rs2_value);
	}
}

void or_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int rs2_value = get_register_value(&regs, rs2);
	unsigned int result = rs1_value | rs2_value;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("or r%d r%d r%d: %d %d %d\n", rd, rs1, rs2, result, rs1_value, rs2_value);
	}
}

void xor_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int rs2_value = get_register_value(&regs, rs2);
	unsigned int result = rs1_value ^ rs2_value;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("xor r%d r%d r%d: %d %d %d\n", rd, rs1, rs2, result, rs1_value, rs2_value);
	}
}

void slli_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int result = rs1_value << imm;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("slli r%d r%d %d: %d %d %d\n", rd, rs1, imm, result, rs1_value, imm);
	}
}

void srli_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int result = rs1_value >> imm;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("srli r%d r%d %d: %d %d %d\n", rd, rs1, imm, result, rs1_value, imm);
	}
}

void sgt_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int rs2_value = get_register_value(&regs, rs2);
	unsigned int result = rs1_value > rs2_value ? 1 : 0;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("sgt r%d r%d r%d: %d %d %d\n", rd, rs1, rs2, result, rs1_value, rs2_value);
	}
}

void slt_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int rs2_value = get_register_value(&regs, rs2);
	unsigned int result = rs1_value < rs2_value ? 1 : 0;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("slt r%d r%d r%d: %d %d %d\n", rd, rs1, rs2, result, rs1_value, rs2_value);
	}
}

void seq_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int rs2_value = get_register_value(&regs, rs2);
	unsigned int result = rs1_value == rs2_value ? 1 : 0;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("seq r%d r%d r%d: %d %d %d\n", rd, rs1, rs2, result, rs1_value, rs2_value);
	}
}

void sne_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int rs2_value = get_register_value(&regs, rs2);
	unsigned int result = rs1_value != rs2_value ? 1 : 0;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("sne r%d r%d r%d: %d %d %d\n", rd, rs1, rs2, result, rs1_value, rs2_value);
	}
}

void sge_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int rs2_value = get_register_value(&regs, rs2);
	unsigned int result = rs1_value >= rs2_value ? 1 : 0;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("sge r%d r%d r%d: %d %d %d\n", rd, rs1, rs2, result, rs1_value, rs2_value);
	}
}

void sle_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	unsigned int rs1_value = get_register_value(&regs, rs1);
	unsigned int rs2_value = get_register_value(&regs, rs2);
	unsigned int result = rs1_value <= rs2_value ? 1 : 0;
	write_register_value(&regs, rd, result);
	PC += 4;
	if(debug_flag){
		printf("sle r%d r%d r%d: %d %d %d\n", rd, rs1, rs2, result, rs1_value, rs2_value);
	}
}

void halt_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	if(debug_flag){
		printf("halt\n");
	}
	halt_flag = 1;
}

void op_func(int rs1, int rs2, int rd, int imm, const char * branch_label){
	int rs1_value;
	if(rs2){
		rs1_value = (int) get_register_value(&regs, rs1);
		int addr = rs1_value + imm;
		unsigned int value = get_byte(&mem, addr);
		printf("%u\n", value);
		fprintf(out_file, "%u\n", value);
	} else {
		rs1_value = get_register_value(&regs, rs1);
		printf("%d\n", rs1_value);
		fprintf(out_file, "%d\n", rs1_value);
	}

	if(debug_flag){
		if(rs2){
			printf("op r%d %d, rs1_value = %d", rs1, imm, rs1_value);
		} else {
			printf("op r%d, rs1_value = %d\n", rs1, rs1_value);
		}
	}
	PC += 4;
}