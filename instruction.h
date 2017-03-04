#ifndef __instruction__h
#define __instruction__h

#include "register.h"
#include "mem.h"

#define LB "LB"
#define LW "LW"
#define SB "SB"
#define SW "SW"
#define ADDI "ADDI"
#define SUBI "SUBI"
#define ANDI "ANDI"
#define ORI "ORI"
#define XORI "XORI"
#define SGTI "SGTI"
#define SEQI "SEQI"
#define SGEI "SGEI"
#define SLTI "SLTI"
#define SNEI "SNEI"
#define SLEI "SLEI"

#define BEQZ "BEQZ"
#define BNEZ "BNEZ"
#define JR "JR"

#define ADD "ADD"
#define SUB "SUB"
#define OR "OR"
#define AND "AND"
#define XOR "XOR"

#define SLLI "SLLI"
#define SRLI "SRLI"

#define SGT "SGT"
#define SEQ "SEQ"
#define SGE "SGE"
#define SLE "SLE"
#define SNE "SNE"
#define SLT "SLT"

#define OP "OP"
#define HALT "HALT"

typedef void (* INS_FUNC)(int, int, int, int, const char *);
struct Instruction{
	char label[100];
	int rs1;
	int rs2;
	int rd;
	int imm;
	INS_FUNC func;
	int has_label;
	char branch_label[100];
};

void lb_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void lw_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void sb_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void sw_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void addi_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void subi_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void andi_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void ori_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void xori_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void sgti_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void seqi_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void sgei_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void slti_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void snei_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void slei_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void beqz_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void bnez_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void jr_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void add_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void sub_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void and_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void or_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void xor_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void slli_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void srli_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void sgt_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void slt_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void seq_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void sne_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void sge_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void sle_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void halt_func(int rs1, int rs2, int rd, int imm, const char * branch_label);

void op_func(int rs1, int rs2, int rd, int imm, const char * branch_label);
#endif