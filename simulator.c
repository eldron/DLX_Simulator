#include "mem.h"
#include "register.h"
#include "instruction.h"

#define TMP_INSTRUCTION_FILE "tmp_instruction_file"
#define TMP_MEMORY_FILE "tmp_memory_file"

struct Mem mem;
struct Instruction * ins;
struct Registers regs;
int PC = 0;
int ins_count;
int halt_flag = 0;
FILE * out_file;
int debug_flag;

void normalize_memory(char * filename){
	// process memory file
	FILE * fin = fopen(filename, "r");
	FILE * fout = fopen(TMP_MEMORY_FILE, "w");
	int state = 0;
	char c;
	while((c = fgetc(fin)) != EOF){
		if(c == ' ' || c == '\t' || c == '\n'){
			if(state == 2){
				fputc('\n', fout);
				state = 0;
			}
		} else if('a' <= c && c <= 'z'){
			fputc(c + 'A' - 'a', fout);
			if(state == 1){
				state = 2;
			}
		} else if(c == ':'){
			fputc(c, fout);
			state = 1;
		} else {
			fputc(c, fout);
			if(state == 1){
				state = 2;
			}
		}
	}
	fclose(fin);
	fclose(fout);
}

int normalize_instructions(char * filename){
	// process instruction file
	FILE * fin = fopen(filename, "r");
	FILE * fout = fopen(TMP_INSTRUCTION_FILE, "w");
	int count = 0;
	int seen = 0;
	int new_ins = 1;
	int label = 0;
	char c;
	while((c = fgetc(fin)) != EOF){
		if(c == ' ' || c == '\t' || c == '\n'){
			if(label){
				seen = 0;
			} else {
				seen = 1;
			}
		} else if('a' <= c && c <= 'z'){
			label = 0;
			if(new_ins == 0){
				if(seen){
					fputc(' ', fout);
					seen = 0;
				}
			}
			fputc(c + 'A' - 'a', fout);
			new_ins = 0;
			seen = 0;
		} else if(c == ';'){
			count++;
			label = 0;
			seen = 0;
			new_ins = 1;
			fputc(c, fout);
			fputc('\n', fout);
		} else if(c == ':'){
			fputc(c, fout);
			new_ins = 0;
			seen = 0;
			label = 1;
		} else {
			if(new_ins == 0){
				if(seen){
					fputc(' ', fout);
					seen = 0;
				}
			}
			label = 0;
			fputc(c, fout);
			new_ins = 0;
			seen = 0;
		}
	}
	fputc('\n', fout);
	fclose(fin);
	fclose(fout);
	return count;
}

unsigned int hex_to_decimal(char * a, int begin, int end){
	unsigned int value = 0;
	int i;
	int j = 0;
	for(i = end;i >= begin;i--){
		if('9' <= a[i] && a[i] <= '9'){
			unsigned int tmp = a[i] - '0';
			value = value | (tmp << j);
		} else {
			unsigned int tmp = 10 + a[i] - 'A';
			value = value | (tmp << j);
		}
		j += 4;
	}
	return value;
}

void init_mem(){
	init(&mem, 0xffff);
	FILE * fin = fopen(TMP_MEMORY_FILE, "r");
	char s[100];
	while(fgets(s, 100, fin) != NULL){
		unsigned int addr = hex_to_decimal(s, 2, 9);
		unsigned int value = hex_to_decimal(s, 13, 14);
		write_byte(&mem, addr, value);
	}
	fclose(fin);
}

void get_rd_rs1_imm(char * s, int j, int * rd, int * rs1, int * imm){
	j = j + 2;
	*rd = 0;
	if(s[j + 1] == ' '){
		*rd = s[j] - '0';
		j = j + 3;
	} else if(s[j + 2] == ' '){
		*rd = (s[j] - '0') * 10 + (s[j + 1] - '0');
		j = j + 4;
	} else {
		printf("parse Rd error\n");
	}
	*rs1 = 0;
	if(s[j + 1] == ' '){
		*rs1 = s[j] - '0';
		j += 2;
	} else if(s[j + 2] == ' '){
		*rs1 = (s[j] - '0') * 10 + (s[j + 1] - '0');
		j += 3;
	} else {
		printf("parse rs1 error\n");
	}
	*imm = atoi(&s[j]);
}
void get_rd_rs1_rs2(char * s, int j, int * rd, int * rs1, int * rs2){
	j += 2;
	if(s[j + 1] == ' '){
		*rd = s[j] - '0';
		j += 3;
	} else if(s[j + 2] == ' '){
		*rd = (s[j] - '0') * 10 + (s[j + 1] - '0');
		j += 4;
	} else {
		printf("parse Rd error\n");
	}
	if(s[j + 1] == ' '){
		*rs1 = s[j] - '0';
		j += 3;
	} else if(s[j + 2] == ' '){
		*rs1 = (s[j] - '0') * 10 + (s[j + 1] - '0');
		j += 4;
	} else {
		printf("parse rs1 error\n");
	}
	if(s[j + 1] == ';'){
		*rs2 = s[j] - '0';
	} else if(s[j + 2] == ';'){
		*rs2 = (s[j] - '0') * 10 + (s[j + 1] - '0');
	} else {
		printf("parse rs2 error");
	}
}

int is_label(char * s){
	int idx = 0;
	while(s[idx] != ';'){
		idx++;
	}
	int i;
	for(i = 0;i < idx;i++){
		if(s[i] == '-' || ('0' <= s[i] && s[i] <= '9')){

		} else {
			return 1;
		}
	}
	return 0;
}
void init_ins(int count){
	ins = (struct Instruction *) malloc((count + 1) * sizeof(struct Instruction));
	FILE * fin = fopen(TMP_INSTRUCTION_FILE, "r");
	char s[100];
	int i;
	ins[count].func = &halt_func;
	for(i = 0;i < count;i++){
		memset(ins[i].label, '\0', 100);
		memset(s, '\0', 100);
		fgets(s, 100, fin);
		int len = strlen(s);
		int j;
		int has_label = 0;
		for(j = 0;j < len;j++){
			if(s[j] == ':'){
				has_label = 1;
				break;
			}
		}
		if(has_label){
			memcpy(ins[i].label, s, j);
			ins[i].has_label = 1;
		} else {
			ins[i].has_label = 0;
		}

		char op[10];
		memset(op, '\0', 10);
		len = 0;
		if(has_label){
			j++;
		} else {
			j = 0;
		}
		while(s[j] != ' '){
			op[len] = s[j];
			len++;
			j++;
		}
		if(debug_flag){
			printf("%d:%s\n", i, op);
		}
		if(strcmp(op, LB) == 0){
			ins[i].func = &lb_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, LW) == 0){
			ins[i].func = &lw_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, SB) == 0){
			ins[i].func = &sb_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, SW) == 0){
			ins[i].func = &sw_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, ADDI) == 0){
			ins[i].func = &addi_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, SUBI) == 0){
			ins[i].func = &subi_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, ANDI) == 0){
			ins[i].func = &andi_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, ORI) == 0){
			ins[i].func = &ori_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, XORI) == 0){
			ins[i].func = &xori_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, SGTI) == 0){
			ins[i].func = &sgti_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, SEQI) == 0){
			ins[i].func = &seqi_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, SGEI) == 0){
			ins[i].func = &sgei_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, SLTI) == 0){
			ins[i].func = &slti_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, SNEI) == 0){
			ins[i].func = &snei_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, SLEI) == 0){
			ins[i].func = &slei_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, AND) == 0){
			ins[i].func = &and_func;
			get_rd_rs1_rs2(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].rs2));
		} else if(strcmp(op, OR) == 0){
			ins[i].func = &or_func;
			get_rd_rs1_rs2(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].rs2));
		} else if(strcmp(op, XOR) == 0){
			ins[i].func = &xor_func;
			get_rd_rs1_rs2(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].rs2));
		} else if(strcmp(op, ADD) == 0){
			ins[i].func = &add_func;
			get_rd_rs1_rs2(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].rs2));
		} else if(strcmp(op, SUB) == 0){
			ins[i].func = &sub_func;
			get_rd_rs1_rs2(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].rs2));
		} else if(strcmp(op, SLLI) == 0){
			ins[i].func = &slli_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, SRLI) == 0){
			ins[i].func = &srli_func;
			get_rd_rs1_imm(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].imm));
		} else if(strcmp(op, SGT) == 0){
			ins[i].func = &sgt_func;
			get_rd_rs1_rs2(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].rs2));
		} else if(strcmp(op, SEQ) == 0){
			ins[i].func = &seq_func;
			get_rd_rs1_rs2(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].rs2));
		} else if(strcmp(op, SGE) == 0){
			ins[i].func = &sge_func;
			get_rd_rs1_rs2(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].rs2));
		} else if(strcmp(op, SLT) == 0){
			ins[i].func = &slt_func;
			get_rd_rs1_rs2(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].rs2));
		} else if(strcmp(op, SNE) == 0){
			ins[i].func = &sne_func;
			get_rd_rs1_rs2(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].rs2));
		} else if(strcmp(op, SLE) == 0){
			ins[i].func = &sle_func;
			get_rd_rs1_rs2(s, j, &(ins[i].rd), &(ins[i].rs1), &(ins[i].rs2));
		} else if(strcmp(op, HALT) == 0){
			ins[i].func = &halt_func;
		} else if(strcmp(op, OP) == 0){
			ins[i].func = &op_func;
			j += 2;
			if(s[j + 1] == ';'){
				// no imm
				ins[i].rs1 = s[j] - '0';
				ins[i].rs2 = 0;
			} else if(s[j + 2] == ';'){
				// no imm
				ins[i].rs1 = (s[j] - '0') * 10 + (s[j + 1] - '0');
				ins[i].rs2 = 0;
			} else if(s[j + 1] == ' '){
				// has imm
				ins[i].rs1 = s[j] - '0';
				ins[i].rs2 = 1;
				ins[i].imm = atoi(&s[j + 2]);
			} else if(s[j + 2] == ' '){
				// has imm
				ins[i].rs1 = (s[j] - '0') * 10 + (s[j + 1] - '0');
				ins[i].rs2 = 1;
				ins[i].imm = atoi(&s[j + 3]);
			} else {
				printf("parse op instruction error\n");
			}
		} else if(strcmp(op, BEQZ) == 0){
			ins[i].func = &beqz_func;
			j += 2;
			if(s[j + 1] == ' '){
				ins[i].rs1 = s[j] - '0';
				j += 2;
			} else if(s[j + 2] == ' '){
				ins[i].rs1 = (s[j] - '0') * 10 + (s[j + 1] - '0');
				j += 3;
			} else {
				printf("BEQZ parse rs1 error\n");
			}
			if(is_label(&s[j]) == 0){
				// imm
				ins[i].imm = atoi(&s[j]);
				ins[i].rs2 = 0;
			} else {
				// label
				ins[i].rs2 = 1;
				memset(ins[i].branch_label, '\0', 100);
				int k = j;
				int idx = 0;
				while(s[k] != ';'){
					ins[i].branch_label[idx] = s[k];
					idx++;
					k++;
				}
			}
		} else if(strcmp(op, BNEZ) == 0){
			ins[i].func = &bnez_func;
			j += 2;
			if(s[j + 1] == ' '){
				ins[i].rs1 = s[j] - '0';
				j += 2;
			} else if(s[j + 2] == ' '){
				ins[i].rs1 = (s[j] - '0') * 10 + (s[j + 1] - '0');
				j += 3;
			} else {
				printf("BNEZ parse rs1 error\n");
			}
			if(is_label(&s[j]) == 0){
				// imm
				ins[i].imm = atoi(&s[j]);
				ins[i].rs2 = 0;
			} else {
				// label
				ins[i].rs2 = 1;
				memset(ins[i].branch_label, '\0', 100);
				int k = j;
				int idx = 0;
				while(s[k] != ';'){
					ins[i].branch_label[idx] = s[k];
					idx++;
					k++;
				}
			}
		} else if(strcmp(op, JR) == 0){
			ins[i].func = &jr_func;
			j += 1;
			if(s[j] == 'R' && 0 <= s[j + 1] && s[j + 1] <= 9 && s[j + 2] == ';'){
				// jump to rs1
				ins[i].rs1 = s[j + 1] - '0';
				ins[i].rs2 = 0;
			} else if(s[j] == 'R' && 0 <= s[j + 1] && s[j + 1] <= '9' && 0 <= s[j + 2] && s[j + 2] <= 9){
				// jump to rs1
				ins[i].rs2 = 0;
				ins[i].rs1 = (s[j + 1] - '0') * 10 + (s[j + 2] - '0');
			} else {
				// jump to label
				ins[i].rs2 = 1;
				memset(ins[i].branch_label, '\0', 100);
				int k = j;
				int idx = 0;
				while(s[k] != ';'){
					ins[i].branch_label[idx] = s[k];
					idx++;
					k++;
				}
			}
		} else {
			printf("error: unsupported instruction %s\n", op);
		}
	}
}
int main(int argc, char ** args){
	if(argc != 7 && argc != 5 && argc != 6 && argc != 8){
		printf("usage: simulator -i instruction_file [-m memory_file] -o output_file [-d]\n");
		return 0;
	}

	int ins_idx = 0;
	int mem_idx = 0;
	int out_idx = 0;
	debug_flag = 0;
	int i;
	for(i = 1;i < argc;i++){
		if(strcmp(args[i], "-i") == 0){
			ins_idx = i + 1;
		}
		if(strcmp(args[i], "-m") == 0){
			mem_idx = i + 1;
		}
		if(strcmp(args[i], "-o") == 0){
			out_idx = i + 1;
		}
		if(strcmp(args[i], "-d") == 0){
			debug_flag = 1;
		}
	}
	ins_count = normalize_instructions(args[ins_idx]);
	if(debug_flag){
		printf("ins_count = %d\n", ins_count);
	}
	if(mem_idx){
		normalize_memory(args[mem_idx]);
		init_mem();
	}
	init_registers(&regs);
	init_ins(ins_count);
	
	out_file = fopen(args[out_idx], "w");
	PC = 0;
	halt_flag = 0;
	while(halt_flag == 0){
		(*(ins[PC / 4].func))(ins[PC / 4].rs1, ins[PC / 4].rs2, ins[PC / 4].rd, ins[PC / 4].imm, ins[PC / 4].branch_label);
	}
	fclose(out_file);
	free(ins);
	if(mem_idx){
		free(*(mem.mem));
	}
	return 0;
}