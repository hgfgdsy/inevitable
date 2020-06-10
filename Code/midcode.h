#include <stdlib.h>
#include <stdio.h>
#include "mps.h"


typedef struct Operand_* Operand;
struct Operand_
{
	enum {VARIABLE, CONSTANT, OP, ADDRESS, TEMP, LABEL, SUBS} kind;
	union
	{
		char* name;
		char* value;
		char* op;
		int adr_no;
		int tmp_no;
		int lab_no;
		int subs;
	} u;
};

typedef struct InterCode_* InterCode;
struct InterCode_
{
	enum {LAB, FUC, ASSIGN, ADD, SUB, MUL, DIV, GT, IFGT, RET, DEC, ARG, CALL, PARAM, READ, WRITE, ASSIGNA, ASSIGNB, ASSIGNC} kind;
	union
	{
		char* fname;
		Operand lonely;
		struct { Operand right, left; } assign;
		struct { Operand result, op1, op2; } binop;
		struct { char* name; int size; } dec;
		struct { Operand OL, OR, rop, ltf; } ig;
		struct { Operand fname, tv; } call;
	} u;
};

struct InterCodes
{
	InterCode code;
	struct InterCodes *prev, *next;
};


struct InterCodes* mc;

void insert_InterCode(InterCode ICode)
{
	if(mc == NULL)
	{
		mc = (struct InterCodes*)malloc(sizeof(struct InterCode_));
		mc->code = ICode;
		mc->prev = NULL;
		mc->next = NULL;
	}
	else
	{
		struct InterCodes* tmp = (struct InterCodes*)malloc(sizeof(struct InterCode_));
		mc->prev = tmp;
		tmp->code = ICode;
		tmp->prev = NULL;
		tmp->next = mc;
		mc = tmp;
	}
}

extern LVtable now;

void generate_midcode(FILE* mid)
{
	struct InterCodes* tmp = mc;
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	int tag = 1;
	while(tmp != NULL)
	{
		InterCode code = tmp->code;
		int kind = code->kind;
		if(kind == LAB)
		{
			fprintf(mid, "LABEL label%d :\n", code->u.lonely->u.lab_no);
		}
		else if(kind == ARG)
		{
//			if(code->u.lonely->u.tmp_no > 0)
				fprintf(mid, "ARG t%d\n", code->u.lonely->u.tmp_no);
//			else
//				fprintf(mid, "ARG &t%d\n", -code->u.lonely->u.tmp_no);
		}
		else if(kind == WRITE)
		{
			fprintf(mid, "WRITE t%d\n", code->u.lonely->u.tmp_no);
		}
		else if(kind == READ)
		{
			fprintf(mid, "READ t%d\n", code->u.lonely->u.tmp_no);

		}
		else if(kind == CALL)
		{
//			if(code->u.call.tv->u.tmp_no > 0)
			fprintf(mid, "t%d := CALL %s\n",code->u.call.tv->u.tmp_no, code->u.call.fname->u.name);
		}
		else if(kind == GT)
		{
			fprintf(mid, "GOTO label%d\n", code->u.lonely->u.lab_no);
		}
		else if(kind == FUC)
		{
			if(tag == 0)
				fprintf(mid, "\n");
			fprintf(mid, "FUNCTION %s :\n", code->u.fname);
		}
		else if(kind == DEC)
		{
			fprintf(mid, "DEC %s %d\n", code->u.dec.name, code->u.dec.size);
		}
		else if(kind == PARAM)
		{
			fprintf(mid, "PARAM %s\n", code->u.fname);
		}
		else if(kind == ASSIGNC)
		{
			Operand R = code->u.assign.right;
			Operand L = code->u.assign.left;
			int recite = 1;
			if(L->kind == TEMP)
			{
				if(L->u.tmp_no == 0)
					recite = 0;
				else
					fprintf(mid, "*t%d := ", L->u.tmp_no);
			}
			if(L->kind == VARIABLE)
			{
				fprintf(mid, "%s := ",L->u.name);
			}




			if(recite == 1){
			if(R->kind == VARIABLE)
			{
				fprintf(mid, "%s\n", R->u.name);
			}
			if(R->kind == CONSTANT)
			{
				fprintf(mid, "#%s\n", R->u.value);
			}
			if(R->kind == TEMP)
			{
				fprintf(mid, "t%d\n", R->u.tmp_no);
			}
			}


		}
		else if(kind == ASSIGNB)
		{
			Operand R = code->u.assign.right;
			Operand L = code->u.assign.left;
			int recite = 1;
			if(L->kind == TEMP)
			{
				if(L->u.tmp_no == 0)
					recite = 0;
				else
					fprintf(mid, "t%d := ", L->u.tmp_no);
			}
			if(L->kind == VARIABLE)
			{
				fprintf(mid, "%s := ",L->u.name);
			}




			if(recite == 1){
			if(R->kind == VARIABLE)
			{
				fprintf(mid, "&%s\n", R->u.name);
			}
			if(R->kind == CONSTANT)
			{
				fprintf(mid, "#%s\n", R->u.value);
			}
			if(R->kind == TEMP)
			{
				fprintf(mid, "t%d\n", R->u.tmp_no);
			}
			}


		}

		else if(kind == ASSIGNA)
		{
			Operand R = code->u.assign.right;
			Operand L = code->u.assign.left;
			int recite = 1;
			if(L->kind == TEMP)
			{
				if(L->u.tmp_no == 0)
					recite = 0;
				else
					fprintf(mid, "t%d := ", L->u.tmp_no);
			}
			if(L->kind == VARIABLE)
			{
				fprintf(mid, "%s := ",L->u.name);
			}




			if(recite == 1){
			if(R->kind == VARIABLE)
			{
				fprintf(mid, "%s\n", R->u.name);
			}
			if(R->kind == CONSTANT)
			{
				fprintf(mid, "#%s\n", R->u.value);
			}
			if(R->kind == TEMP)
			{
				fprintf(mid, "*t%d\n", R->u.tmp_no);
			}
			}


		}
		else if(kind == ASSIGN)
		{
			Operand R = code->u.assign.right;
			Operand L = code->u.assign.left;
			int recite = 1;
			if(L->kind == TEMP)
			{
				if(L->u.tmp_no == 0)
					recite = 0;
				else
					fprintf(mid, "t%d := ", L->u.tmp_no);
			}
			if(L->kind == VARIABLE)
			{
				fprintf(mid, "%s := ",L->u.name);
//				DATAtable ret = search_DATAtable(now, L->u.name, 3);
			}




			if(recite == 1){
			if(R->kind == VARIABLE)
			{
				fprintf(mid, "%s\n", R->u.name);
			}
			if(R->kind == CONSTANT)
			{
				fprintf(mid, "#%s\n", R->u.value);
			}
			if(R->kind == TEMP)
			{
				fprintf(mid, "t%d\n", R->u.tmp_no);
			}
			}
		}
		else if(kind == MUL || kind == ADD || kind == SUB || kind == DIV)
		{
			Operand o1 = code->u.binop.op1;
			Operand o2 = code->u.binop.op2;
			Operand RL = code->u.binop.result;
			if(RL->kind == TEMP)
			{
				fprintf(mid, "t%d := ", RL->u.tmp_no);
			}

			if(o1->kind == CONSTANT)
			{
				fprintf(mid, "#%s ", o1->u.value);
			}
			if(o1->kind == TEMP)
			{
				fprintf(mid, "t%d ", o1->u.tmp_no);
			}
			if(o1->kind == SUBS)
			{
				fprintf(mid, "#%d ", o1->u.subs);
			}

			if(kind == SUB)
			{
				fprintf(mid, "- ");
			}
			if(kind == ADD)
			{
				fprintf(mid, "+ ");
			}
			if(kind == MUL)
			{
				fprintf(mid, "* ");
			}
			if(kind == DIV)
			{
				fprintf(mid, "/ ");
			}

			if(o2->kind == TEMP)
			{
				fprintf(mid, "t%d\n", o2->u.tmp_no);
			}
			if(o2->kind == SUBS)
			{
				fprintf(mid, "#%d\n", o2->u.subs);
			}

		}
		else if(kind == RET)
		{
			fprintf(mid, "RETURN t%d\n", code->u.lonely->u.tmp_no);
		}
		else if(kind == IFGT)
		{
			Operand OL = code->u.ig.OL;
			Operand OR1 = code->u.ig.OR;
			Operand rop = code->u.ig.rop;
			Operand ltf = code->u.ig.ltf;
			fprintf(mid, "IF ");
			if(OL->kind == TEMP)
			{
				fprintf(mid, "t%d ", OL->u.tmp_no);
			}
			fprintf(mid, "%s ", rop->u.op);
			if(OR1->kind == TEMP)
			{
				fprintf(mid, "t%d ", OR1->u.tmp_no);
			}
			if(OR1->kind == CONSTANT)
			{
				fprintf(mid, "#%s ", OR1->u.value);
			}
			fprintf(mid, "GOTO ");
			fprintf(mid, "label%d\n", ltf->u.lab_no);
		}
		tag = 0;
		tmp = tmp -> prev;
	}
}

int t_no;
int l_no;
int new_temp()
{
	t_no = t_no + 1;
	return t_no;
}
int new_label()
{
	l_no = l_no + 1;
	return l_no;
}

extern TEMPtable tc;
extern VARtable vc;

void generate_mips(FILE* mid)
{
	struct InterCodes* tmp = mc;
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	int tag = 1;
	int offset = 1;
	int pnn = 0;
	int pmm = 1;
	tc = NULL;
	vc = NULL;
	fprintf(mid, ".data\n");
	fprintf(mid, "_prompt: .asciiz \"Enter an integer:\"\n");
	fprintf(mid, "_ret: .asciiz \"\\n\"\n");
	fprintf(mid, ".globl main\n");
	fprintf(mid, ".text\n");

	fprintf(mid, "read:\n");
	fprintf(mid, "li $v0, 4\n");
	fprintf(mid, "la $a0, _prompt\n");
	fprintf(mid, "syscall\n");
	fprintf(mid, "li $v0, 5\n");
	fprintf(mid, "syscall\n");
	fprintf(mid, "jr $ra\n\n");

	fprintf(mid, "write:\n");
	fprintf(mid, "li $v0, 1\n");
	fprintf(mid, "syscall\n");
	fprintf(mid, "li $v0, 4\n");
	fprintf(mid, "la $a0, _ret\n");
	fprintf(mid, "syscall\n");
	fprintf(mid, "move $v0, $0\n");
	fprintf(mid, "jr $ra\n\n");


	while(tmp != NULL)
	{
		InterCode code = tmp->code;
		int kind = code->kind;
		if(kind == LAB)
		{
//			fprintf(mid, "LABEL label%d :\n", code->u.lonely->u.lab_no);
			fprintf(mid, "label%d:\n", code->u.lonely->u.lab_no);
		}
		else if(kind == ARG)
		{
//			fprintf(mid, "ARG t%d\n", code->u.lonely->u.tmp_no);
			offset = offset + 1;
			pnn = pnn + 1;
			fprintf(mid, "subu $sp, $fp, %d\n",offset*4);
			TEMPtable ret = search_TEMPtable(code->u.lonely->u.tmp_no);
			fprintf(mid, "lw $t0, -%d($fp)\n", ret->offset*4);
			fprintf(mid, "sw $t0, -%d($fp)\n", offset*4);
		}
		else if(kind == WRITE)
		{
//			fprintf(mid, "WRITE t%d\n", code->u.lonely->u.tmp_no);
                        TEMPtable ret = search_TEMPtable(code->u.lonely->u.tmp_no);
			fprintf(mid, "lw $a0, -%d($fp)\n", ret->offset*4);
			fprintf(mid, "subu $sp, $fp, %d\n", offset*4);
			fprintf(mid, "subu $sp, $sp, 4\n");
			fprintf(mid, "sw $ra, 0($sp)\n");
			fprintf(mid, "jal write\n");
			fprintf(mid, "lw $ra, 0($sp)\n");
			fprintf(mid, "addu $sp, $sp, 4\n");
		}
		else if(kind == READ)
		{
//			fprintf(mid, "READ t%d\n", code->u.lonely->u.tmp_no);
			fprintf(mid, "subu $sp, $fp, %d\n", offset*4);
			fprintf(mid, "subu $sp, $sp, 4\n");
			fprintf(mid, "sw $ra, 0($sp)\n");
			fprintf(mid, "jal read\n");
			fprintf(mid, "lw $ra, 0($sp)\n");
			fprintf(mid, "addu $sp, $sp, 4\n");
			TEMPtable ret = search_TEMPtable(code->u.lonely->u.tmp_no);
			if(ret == NULL)
			{
				offset = offset + 1;
				fprintf(mid, "subu $sp, $sp, 4\n");
				insert_TEMPtable(code->u.lonely->u.tmp_no, offset);
				ret = tc;
			}
			fprintf(mid, "sw $v0, -%d($fp)\n", ret->offset*4);
		}
		else if(kind == CALL)
		{
			fprintf(mid, "subu $sp, $fp, %d\n", offset*4);
			fprintf(mid, "subu $sp, $sp, 4\n");
			fprintf(mid, "sw $ra, 0($sp)\n");
			fprintf(mid, "jal %s\n",code->u.call.fname->u.name);
			fprintf(mid, "lw $ra, 0($sp)\n");
			fprintf(mid, "addu $sp, $sp, 4\n");
			fprintf(mid, "addu $sp, $sp, %d\n", pnn*4);
			offset = offset - pnn;
			pnn = 0;
			TEMPtable ret = search_TEMPtable(code->u.call.tv->u.tmp_no);
			if(ret == NULL)
			{
				offset = offset + 1;
				fprintf(mid, "subu $sp, $sp, 4\n");
				insert_TEMPtable(code->u.call.tv->u.tmp_no, offset);
				ret = tc;
			}
			fprintf(mid, "sw $v0, -%d($fp)\n", ret->offset*4);
		}
		else if(kind == GT)
		{
//			fprintf(mid, "GOTO label%d\n", code->u.lonely->u.lab_no);
			fprintf(mid, "j label%d\n", code->u.lonely->u.lab_no);
		}
		else if(kind == FUC)
		{
			if(tag == 0)
				fprintf(mid, "\n");
//			fprintf(mid, "FUNCTION %s :\n", code->u.fname);
			offset = 1;
			pmm = 1;
			fprintf(mid, "\n");
			fprintf(mid, "%s:\n", code->u.fname);
			fprintf(mid, "subu $sp, $sp, 4\n");
			fprintf(mid, "sw $fp, 0($sp)\n");
			fprintf(mid, "addi $fp, $sp, 4\n");
		}
		else if(kind == DEC)
		{
//			fprintf(mid, "DEC %s %d\n", code->u.dec.name, code->u.dec.size);
//			offset = offset + 1;
//			insert_VARtable(code->u.dec.name, offset);
			int exo = code->u.dec.size/4;
			offset = offset + exo;
			insert_VARtable(code->u.dec.name, offset);
			fprintf(mid, "subu $sp, $sp, %d\n", code->u.dec.size);
		}
		else if(kind == PARAM)
		{
//			fprintf(mid, "PARAM %s\n", code->u.fname);
			fprintf(mid, "lw $t0, %d($fp)\n", pmm*4);	
			offset = offset + 1;
			fprintf(mid, "subu $sp, $sp, 4\n");
			insert_VARtable(code->u.fname, offset);
			fprintf(mid, "sw $t0, -%d($fp)\n", offset*4);
			pmm = pmm + 1;
		}
		else if(kind == ASSIGNC)
		{
			Operand R = code->u.assign.right;
			Operand L = code->u.assign.left;
/*			int recite = 1;
			if(L->kind == TEMP)
			{
				if(L->u.tmp_no == 0)
					recite = 0;
				else
				{
					fprintf(mid, "*t%d := ", L->u.tmp_no);
//					fprintf(mid, "")
				}
			}
			if(L->kind == VARIABLE)
			{
				fprintf(mid, "%s := ",L->u.name);
			}




			if(recite == 1){
			if(R->kind == VARIABLE)
			{
				fprintf(mid, "%s\n", R->u.name);
			}
			if(R->kind == CONSTANT)
			{
				fprintf(mid, "#%s\n", R->u.value);
			}
			if(R->kind == TEMP)
			{
				fprintf(mid, "t%d\n", R->u.tmp_no);
			}
			}

*/
			if(L->kind == TEMP && R->kind == VARIABLE && L->u.tmp_no != 0)
			{
				TEMPtable rl = search_TEMPtable(L->u.tmp_no);
				if(rl == NULL)
				{
					offset = offset + 1;
					fprintf(mid, "subu $sp, $fp, %d\n",offset*4);
					insert_TEMPtable(L->u.tmp_no, offset);
					rl = tc;
				}
				VARtable rr = search_VARtable(R->u.name);
				fprintf(mid, "lw $t0, -%d($fp)\n", rl->offset*4);
				fprintf(mid, "lw $t1, -%d($fp)\n", rr->offset*4);
				fprintf(mid, "sw $t1, 0($t0)\n");
			}
			else if(L->kind == TEMP && R->kind == TEMP && L->u.tmp_no != 0)
			{
				TEMPtable rl = search_TEMPtable(L->u.tmp_no);
				if(rl == NULL)
				{
					offset = offset + 1;
					fprintf(mid, "subu $sp, $fp, %d\n",offset*4);
					insert_TEMPtable(L->u.tmp_no, offset);
					rl = tc;
				}
				TEMPtable rr = search_TEMPtable(R->u.tmp_no);
				fprintf(mid, "lw $t0, -%d($fp)\n", rl->offset*4);
				fprintf(mid, "lw $t1, -%d($fp)\n", rr->offset*4);
				fprintf(mid, "sw $t1, 0($t0)\n");
			}
		}
		else if(kind == ASSIGNB)
		{
			Operand R = code->u.assign.right;
			Operand L = code->u.assign.left;
/*			int recite = 1;
			if(L->kind == TEMP)
			{
				if(L->u.tmp_no == 0)
					recite = 0;
				else
					fprintf(mid, "t%d := ", L->u.tmp_no);
			}
			if(L->kind == VARIABLE)
			{
				fprintf(mid, "%s := ",L->u.name);
			}




			if(recite == 1){
			if(R->kind == VARIABLE)
			{
				fprintf(mid, "&%s\n", R->u.name);
			}
			if(R->kind == CONSTANT)
			{
				fprintf(mid, "#%s\n", R->u.value);
			}
			if(R->kind == TEMP)
			{
				fprintf(mid, "t%d\n", R->u.tmp_no);
			}
			}
*/
			if(L->kind == TEMP && R->kind == VARIABLE && L->u.tmp_no != 0)
			{
				TEMPtable rl = search_TEMPtable(L->u.tmp_no);
				VARtable rr = search_VARtable(R->u.name);
				fprintf(mid, "subu $t0, $fp, %d\n", rr->offset*4);
				if(rl == NULL)
				{
					offset = offset + 1;
					fprintf(mid, "subu $sp, $fp, %d\n",offset*4);
					insert_TEMPtable(L->u.tmp_no, offset);
					fprintf(mid, "sw $t0, -%d($fp)\n", offset*4);
				}
				else
					fprintf(mid, "sw $t0, -%d($fp)\n", rr->offset*4);
			}
			else if(L->kind == VARIABLE && R->kind == VARIABLE)
			{
				VARtable rl = search_VARtable(L->u.name);
				VARtable rr = search_VARtable(R->u.name);
				fprintf(mid, "subu $t0, $fp, %d\n", rr->offset*4);
				if(rl == NULL)
				{
					offset = offset + 1;
					fprintf(mid, "subu $sp, $fp, %d\n",offset*4);
					insert_VARtable(L->u.name, offset);
					fprintf(mid, "sw $t0, -%d($fp)\n", offset*4);
				}
				else
					fprintf(mid, "sw $t0, -%d($fp)\n", rr->offset*4);
			}

		}

		else if(kind == ASSIGNA)
		{
			Operand R = code->u.assign.right;
			Operand L = code->u.assign.left;
/*			int recite = 1;
			if(L->kind == TEMP)
			{
				if(L->u.tmp_no == 0)
					recite = 0;
				else
					fprintf(mid, "t%d := ", L->u.tmp_no);
			}
			if(L->kind == VARIABLE)
			{
				fprintf(mid, "%s := ",L->u.name);
			}




			if(recite == 1){
			if(R->kind == VARIABLE)
			{
				fprintf(mid, "%s\n", R->u.name);
			}
			if(R->kind == CONSTANT)
			{
				fprintf(mid, "#%s\n", R->u.value);
			}
			if(R->kind == TEMP)
			{
				fprintf(mid, "*t%d\n", R->u.tmp_no);
			}
			}
*/
			if(L->kind == TEMP && R->kind == TEMP && L->u.tmp_no != 0)
			{
				TEMPtable rl = search_TEMPtable(L->u.tmp_no);
				TEMPtable rr = search_TEMPtable(R->u.tmp_no);
				fprintf(mid, "lw $t0, -%d($fp)\n", rr->offset*4);
				fprintf(mid, "lw $t1, 0($t0)\n");
				if(rl == NULL)
				{
					offset = offset + 1;
					fprintf(mid, "subu $sp, $fp, %d\n",offset*4);
					insert_TEMPtable(L->u.tmp_no, offset);
					fprintf(mid, "sw $t1, -%d($fp)\n", offset*4);
				}
				else
					fprintf(mid, "sw $t1, -%d($fp)\n", rr->offset*4);
			}
			else if(L->kind == VARIABLE && R->kind == TEMP)
			{
				VARtable rl = search_VARtable(L->u.name);
				TEMPtable rr = search_TEMPtable(R->u.tmp_no);
				fprintf(mid, "lw $t0, -%d($fp)\n", rr->offset*4);
				fprintf(mid, "lw $t1, 0($t0)\n");
				if(rl == NULL)
				{
					offset = offset + 1;
					fprintf(mid, "subu $sp, $fp, %d\n",offset*4);
					insert_VARtable(L->u.name, offset);
					fprintf(mid, "sw $t1, -%d($fp)\n", offset*4);
				}
				else
					fprintf(mid, "sw $t1, -%d($fp)\n", rr->offset*4);
			}


		}
		else if(kind == ASSIGN)
		{
			Operand R = code->u.assign.right;
			Operand L = code->u.assign.left;
/*			int recite = 1;
			if(L->kind == TEMP)
			{
				if(L->u.tmp_no == 0)
					recite = 0;
				else
					fprintf(mid, "t%d := ", L->u.tmp_no);
			}
			if(L->kind == VARIABLE)
			{
				fprintf(mid, "%s := ",L->u.name);
			}




			if(recite == 1){
			if(R->kind == VARIABLE)
			{
				fprintf(mid, "%s\n", R->u.name);
			}
			if(R->kind == CONSTANT)
			{
				fprintf(mid, "#%s\n", R->u.value);
			}
			if(R->kind == TEMP)
			{
				fprintf(mid, "t%d\n", R->u.tmp_no);
			}
			}
*/
			int recite = 1;
			TEMPtable l1 = NULL;
			VARtable l2 = NULL;
			int rec = -1;
			if(L->kind == TEMP)
			{
				if(L->u.tmp_no == 0)
					recite = 0;
				else
				{
					l1 = search_TEMPtable(L->u.tmp_no);
					rec = 1;
				}
			}
			if(L->kind == VARIABLE)
			{
				l2 = search_VARtable(L->u.name);
				rec = 2;
			}




			if(recite == 1){
				if(rec == 1)
				{
					if(l1 == NULL)
					{
						offset = offset + 1;
						fprintf(mid, "subu $sp, $fp, %d\n",offset*4);
						insert_TEMPtable(L->u.tmp_no, offset);
						l1 = tc;
					}
				}
				else if(rec == 2)
				{
					if(l2 == NULL)
					{
						offset = offset + 1;
						fprintf(mid, "subu $sp, $fp, %d\n",offset*4);
						insert_VARtable(L->u.name, offset);
						l2 = vc;
					}
				}
			if(R->kind == VARIABLE)
			{
				VARtable rr = search_VARtable(R->u.name);
				fprintf(mid, "lw $t1, -%d($fp)\n", rr->offset*4);
				if(rec == 1)
				{
					fprintf(mid, "sw $t1, -%d($fp)\n", l1->offset*4);
				}
				else
				{
					fprintf(mid, "sw $t1, -%d($fp)\n", l2->offset*4);
				}
			}
			if(R->kind == CONSTANT)
			{
				fprintf(mid, "li $t0, %s\n", R->u.value);
				if(rec == 1)
				{
					fprintf(mid, "sw $t0, -%d($fp)\n", l1->offset*4);
				}
				else
				{
					fprintf(mid, "sw $t0, -%d($fp)\n", l2->offset*4);
				}
			}
			if(R->kind == TEMP)
			{
				TEMPtable rr = search_TEMPtable(R->u.tmp_no);
				fprintf(mid, "lw $t1, -%d($fp)\n", rr->offset*4);
				if(rec == 1)
				{
					fprintf(mid, "sw $t1, -%d($fp)\n", l1->offset*4);
				}
				else
				{
					fprintf(mid, "sw $t1, -%d($fp)\n", l2->offset*4);
				}			
			}
			}
		
		}
		else if(kind == MUL || kind == ADD || kind == SUB || kind == DIV)
		{
			Operand o1 = code->u.binop.op1;
			Operand o2 = code->u.binop.op2;
			Operand RL = code->u.binop.result;
			TEMPtable rl = NULL;
			if(RL->kind == TEMP)
			{
				rl = search_TEMPtable(RL->u.tmp_no);
				if(rl == NULL)
				{
						fprintf(mid, "subu $sp, $fp, %d\n",offset*4);
						offset = offset + 1;
						insert_TEMPtable(RL->u.tmp_no, offset);
						rl = tc;
				}
			}
			if(kind == ADD)
			{
				if(o1->kind == CONSTANT)
				{
					TEMPtable r2 = search_TEMPtable(o2->u.tmp_no);
					fprintf(mid, "lw $t0, -%d($fp)\n", r2->offset*4);
					fprintf(mid, "addi $t1, $t0, %s\n", o1->u.value);
					fprintf(mid, "sw $t1, -%d($fp)\n", rl->offset*4);
				}
				else if(o1->kind == SUBS)
				{
					TEMPtable r2 = search_TEMPtable(o2->u.tmp_no);
					fprintf(mid, "lw $t0, -%d($fp)\n", r2->offset*4);
					fprintf(mid, "addi $t1, $t0, %d\n", o1->u.subs);
					fprintf(mid, "sw $t1, -%d($fp)\n", rl->offset*4);
				}
				else if(o1->kind == TEMP)
				{
					TEMPtable r1 = search_TEMPtable(o1->u.tmp_no);
					if(o2->kind == SUBS)
					{
						fprintf(mid, "lw $t0, -%d($fp)\n", r1->offset*4);
						fprintf(mid, "addi $t1, $t0, %d\n",o2->u.subs);
						fprintf(mid, "sw $t1, -%d($fp)\n", rl->offset*4);
					}
					else
					{
						TEMPtable r2 = search_TEMPtable(o2->u.tmp_no);
						fprintf(mid, "lw $t0, -%d($fp)\n", r1->offset*4);
						fprintf(mid, "lw $t1, -%d($fp)\n", r2->offset*4);
						fprintf(mid, "add $t2, $t0, $t1\n");
						fprintf(mid, "sw $t2, -%d($fp)\n", rl->offset*4);
					}
				}
			}
			else if(kind == SUB)
			{
				if(o1->kind == CONSTANT)
				{
					TEMPtable r2 = search_TEMPtable(o2->u.tmp_no);
					fprintf(mid, "lw $t0, -%d($fp)\n", r2->offset*4);
					fprintf(mid, "li $t2, %s\n", o1->u.value);
					fprintf(mid, "sub $t1, $t2, $t0\n");
					fprintf(mid, "sw $t1, -%d($fp)\n", rl->offset*4);
				}
				else if(o1->kind == SUBS)
				{
					TEMPtable r2 = search_TEMPtable(o2->u.tmp_no);
					fprintf(mid, "lw $t0, -%d($fp)\n", r2->offset*4);
					fprintf(mid, "li $t2, %d\n", o1->u.subs);
					fprintf(mid, "sub $t1, $t2, $t0\n");
					fprintf(mid, "sw $t1, -%d($fp)\n", rl->offset*4);
				}
				else if(o1->kind == TEMP)
				{
					TEMPtable r1 = search_TEMPtable(o1->u.tmp_no);
					if(o2->kind == SUBS)
					{
						fprintf(mid, "lw $t0, -%d($fp)\n", r1->offset*4);
						fprintf(mid, "li $t2, %d\n", o2->u.subs);
						fprintf(mid, "sub $t1, $t0, $t2\n");
						fprintf(mid, "sw $t1, -%d($fp)\n", rl->offset*4);
					}
					else
					{
						TEMPtable r2 = search_TEMPtable(o2->u.tmp_no);
						fprintf(mid, "lw $t0, -%d($fp)\n", r1->offset*4);
						fprintf(mid, "lw $t1, -%d($fp)\n", r2->offset*4);
						fprintf(mid, "sub $t2, $t0, $t1\n");
						fprintf(mid, "sw $t2, -%d($fp)\n", rl->offset*4);
					}
				}
			}
			else if(kind == MUL)
			{
				if(o1->kind == CONSTANT)
				{
					TEMPtable r2 = search_TEMPtable(o2->u.tmp_no);
					fprintf(mid, "lw $t0, -%d($fp)\n", r2->offset*4);
					fprintf(mid, "li $t2, %s\n", o1->u.value);
					fprintf(mid, "mul $t1, $t2, $t0\n");
					fprintf(mid, "sw $t1, -%d($fp)\n", rl->offset*4);
				}
				else if(o1->kind == SUBS)
				{
					TEMPtable r2 = search_TEMPtable(o2->u.tmp_no);
					fprintf(mid, "lw $t0, -%d($fp)\n", r2->offset*4);
					fprintf(mid, "li $t2, %d\n", o1->u.subs);
					fprintf(mid, "mul $t1, $t2, $t0\n");
					fprintf(mid, "sw $t1, -%d($fp)\n", rl->offset*4);
				}
				else if(o1->kind == TEMP)
				{
					TEMPtable r1 = search_TEMPtable(o1->u.tmp_no);
					if(o2->kind == SUBS)
					{
						fprintf(mid, "lw $t0, -%d($fp)\n", r1->offset*4);
						fprintf(mid, "li $t2, %d\n", o2->u.subs);
						fprintf(mid, "mul $t1, $t0, $t2\n");
						fprintf(mid, "sw $t1, -%d($fp)\n", rl->offset*4);
					}
					else
					{
						
						TEMPtable r2 = search_TEMPtable(o2->u.tmp_no);
//						if(r2 == NULL) printf("oops3\n");
						fprintf(mid, "lw $t0, -%d($fp)\n", r1->offset*4);
						fprintf(mid, "lw $t1, -%d($fp)\n", r2->offset*4);
						fprintf(mid, "mul $t2, $t0, $t1\n");
						fprintf(mid, "sw $t2, -%d($fp)\n", rl->offset*4);
					}
				}
			}
			else if(kind == DIV)
			{
				if(o1->kind == CONSTANT)
				{
					TEMPtable r2 = search_TEMPtable(o2->u.tmp_no);
					fprintf(mid, "lw $t0, -%d($fp)\n", r2->offset*4);
					fprintf(mid, "li $t2, %s\n", o1->u.value);
					fprintf(mid, "div $t2, $t0\n");
					fprintf(mid, "mflo $t1\n");
					fprintf(mid, "sw $t1, -%d($fp)\n", rl->offset*4);
				}
				else if(o1->kind == SUBS)
				{
					TEMPtable r2 = search_TEMPtable(o2->u.tmp_no);
					fprintf(mid, "lw $t0, -%d($fp)\n", r2->offset*4);
					fprintf(mid, "li $t2, %d\n", o1->u.subs);
					fprintf(mid, "div $t2, $t0\n");
					fprintf(mid, "mflo $t1\n");
					fprintf(mid, "sw $t1, -%d($fp)\n", rl->offset*4);
				}
				else if(o1->kind == TEMP)
				{
					TEMPtable r1 = search_TEMPtable(o1->u.tmp_no);
					if(o2->kind == SUBS)
					{
						fprintf(mid, "lw $t0, -%d($fp)\n", r1->offset*4);
						fprintf(mid, "li $t2, %d\n", o2->u.subs);
						fprintf(mid, "div $t0, $t2\n");
						fprintf(mid, "mflo $t1\n");
						fprintf(mid, "sw $t1, -%d($fp)\n", rl->offset*4);
					}
					else
					{
						TEMPtable r2 = search_TEMPtable(o2->u.tmp_no);
						fprintf(mid, "lw $t0, -%d($fp)\n", r1->offset*4);
						fprintf(mid, "lw $t1, -%d($fp)\n", r2->offset*4);
						fprintf(mid, "div $t0, $t1\n");
						fprintf(mid, "mflo $t2\n");
						fprintf(mid, "sw $t2, -%d($fp)\n", rl->offset*4);
					}
				}
			}
/*
			if(o1->kind == CONSTANT)
			{
				fprintf(mid, "#%s ", o1->u.value);
			}
			if(o1->kind == TEMP)
			{
				fprintf(mid, "t%d ", o1->u.tmp_no);
			}
			if(o1->kind == SUBS)
			{
				fprintf(mid, "#%d ", o1->u.subs);
			}

			if(kind == SUB)
			{
				fprintf(mid, "- ");
			}
			if(kind == ADD)
			{
				fprintf(mid, "+ ");
			}
			if(kind == MUL)
			{
				fprintf(mid, "* ");
			}
			if(kind == DIV)
			{
				fprintf(mid, "/ ");
			}

			if(o2->kind == TEMP)
			{
				fprintf(mid, "t%d\n", o2->u.tmp_no);
			}
			if(o2->kind == SUBS)
			{
				fprintf(mid, "#%d\n", o2->u.subs);
			}
*/

		}
		else if(kind == RET)
		{
//			fprintf(mid, "RETURN t%d\n", code->u.lonely->u.tmp_no);
			TEMPtable ret = search_TEMPtable(code->u.lonely->u.tmp_no);
			fprintf(mid, "lw $t0, -%d($fp)\n",ret->offset*4);
			fprintf(mid, "move $t1, $fp\n");
			fprintf(mid, "lw $fp, -4($t1)\n");
//			fprintf(mid, "addi $sp, $sp, %d\n", offset*4);
			fprintf(mid, "move $sp, $t1\n");
			fprintf(mid, "move $v0, $t0\n");
			fprintf(mid, "jr $ra\n");
		}
		else if(kind == IFGT)
		{
			Operand OL = code->u.ig.OL;
			Operand OR1 = code->u.ig.OR;
			Operand rop = code->u.ig.rop;
			Operand ltf = code->u.ig.ltf;
//			fprintf(mid, "IF ");
			if(OL->kind == TEMP)
			{
//				fprintf(mid, "t%d ", OL->u.tmp_no);
				TEMPtable ol = search_TEMPtable(OL->u.tmp_no);
				fprintf(mid, "lw $t0, -%d($fp)\n", ol->offset*4);
			}
//			fprintf(mid, "%s ", rop->u.op);
			if(OR1->kind == TEMP)
			{
//				fprintf(mid, "t%d ", OR1->u.tmp_no);
				TEMPtable o2 = search_TEMPtable(OR1->u.tmp_no);
				fprintf(mid, "lw $t1, -%d($fp)\n", o2->offset*4);
			}
			if(OR1->kind == CONSTANT)
			{
//				fprintf(mid, "#%s ", OR1->u.value);
				fprintf(mid, "li $t1, %s\n", OR1->u.value);
			}
//			fprintf(mid, "GOTO ");
//			fprintf(mid, "label%d\n", ltf->u.lab_no);

			if(!strcmp(rop->u.op, "=="))
			{
				fprintf(mid, "beq $t0, $t1, label%d\n", ltf->u.lab_no);
			}
			else if(!strcmp(rop->u.op, "!="))
			{
				fprintf(mid, "bne $t0, $t1, label%d\n", ltf->u.lab_no);

			}
			else if(!strcmp(rop->u.op, ">"))
			{
				fprintf(mid, "bgt $t0, $t1, label%d\n", ltf->u.lab_no);

			}
			else if(!strcmp(rop->u.op, "<"))
			{
				fprintf(mid, "blt $t0, $t1, label%d\n", ltf->u.lab_no);

			}
			else if(!strcmp(rop->u.op, ">="))
			{
				fprintf(mid, "bge $t0, $t1, label%d\n", ltf->u.lab_no);

			}
			else if(!strcmp(rop->u.op, "<="))
			{
				fprintf(mid, "ble $t0, $t1, label%d\n", ltf->u.lab_no);

			}
		}
		tag = 0;
		tmp = tmp -> prev;
	}
}

