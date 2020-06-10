#include "tree.h"
#include "table.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "midcode.h"


LVtable now;
extern FUCtable head;
char* nfn;
extern struct InterCodes* mc;
char* C0;
char* C1;
char* NEQ;
bsize nlist;

void dfs_VarDec(struct Node* nd, Type tp1, Type *tp2, char* *name)
{
	if(nd->ncld == 1)
	{
		*tp2 = tp1;
		*name = (nd->cld)[0]->value;
	}
	else
	{
		Type new = (Type)malloc(sizeof(struct Type_));
		int size = 0;
		struct Node* inode = (nd->cld)[2];
		int len = strlen(inode->value);
		for(int i=0;i<len;i++)
		{
			size*=10;
			size+=*(inode->value+i)-'0';
		}

		if(nlist == NULL)
		{
			nlist = (bsize)malloc(sizeof(struct bsize_));
			nlist->size = size;
			nlist->next = NULL;
		}
		else
		{
			bsize tlist = nlist;
			while(tlist->next != NULL)
			{
				tlist = tlist->next;
			}
			bsize new = (bsize)malloc(sizeof(struct bsize_));
			new->size = size;
			new->next = NULL;
			tlist->next = new;
		}

		dfs_VarDec((nd->cld)[0], tp1, tp2, name);
		new->kind = ARRAY;
		new->u.array.elem = *tp2;
		new->u.array.size = nlist->size;
		nlist = nlist->next;
		*tp2 = new;
	}
}



void tk_DC(FieldList record, struct Node* nd, int label, Type tp)
{
	if(nd->ncld == 1)
	{
		Type ans = NULL;
		char* name = NULL;
		dfs_VarDec((nd->cld)[0], tp, &ans, &name);
		if(record -> name == NULL)
		{
			FieldList tmp_record = (FieldList)malloc(sizeof(struct FieldList_));
			record->name = name;
			record->type = ans;
			record->next = tmp_record;
			tmp_record->next = NULL;
		}
		else
		{
			FieldList tmp = record;
			int label1 = 1;
			while(tmp->next != NULL)
			{
				if(!NAME(name, tmp->name) && label == 0)
				{
					label1=0;
					printf("Error type 15 at Line %d: Redefined field \"%s\".\n", nd->line, name);
					break;
				}
				tmp = tmp->next;
			}
			if(label1)
			{
				tmp->next = (FieldList)malloc(sizeof(struct FieldList_));
				tmp->name = name;
				tmp->type = ans;
				tmp->next->next=NULL;
			}
		}

	}
	else
	{
		if(label == 0)
			printf("Error type 15 at Line %d: assign value when declare field in struct.\n",nd->line);
		else
		{
		Type ans = NULL;
		char* name = NULL;
		dfs_VarDec((nd->cld)[0], tp, &ans, &name);
		int CT = 0;


		int t1 = new_temp();
		Type ET = tk_Exp((nd->cld)[2], t1, -1, -1, -1);
		if(ans->kind == BASIC)
		{
			Operand opt1 = (Operand)malloc(sizeof(struct Operand_));
			opt1->kind = TEMP;
			opt1->u.tmp_no = t1;
			Operand opt2 = (Operand)malloc(sizeof(struct Operand_));
			opt2->kind = VARIABLE;
			opt2->u.name = name;
			InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = ASSIGN;
			mctmp->u.assign.left = opt2;
			mctmp->u.assign.right = opt1;
			insert_InterCode(mctmp);
		}


		if(ET != NULL)
		{
			CT = check_type(ET, ans);
		}
		if(!CT)	printf("Error type 5 at Line %d: Type mismatched for assignment.\n", nd->line);	
		if(record -> name == NULL)
		{
			FieldList tmp_record = (FieldList)malloc(sizeof(struct FieldList_));
			record->name = name;
			record->type = ans;
			record->next = tmp_record;
			tmp_record->next = NULL;
		}
		else
		{
			FieldList tmp = record;
			int label1 = 1;
			while(tmp->next != NULL)
			{
				if(!NAME(name, tmp->name) && label == 0)
				{
					label1=0;
					printf("Impossible\n");
					break;
				}
				tmp = tmp->next;
			}
			if(label1)
			{
				tmp->next = (FieldList)malloc(sizeof(struct FieldList_));
				tmp->name = name;
				tmp->type = ans;
				tmp->next->next=NULL;
			}
		}
		}
	}
}


void dfs_DecList(FieldList record, struct Node* nd, int label, Type tp)
{
	if(nd!=NULL)
		{
			if(nd->type == 0) return;
			if(!NAME(nd->tag, "Dec"))
				tk_DC(record, nd, label, tp);
			else
				for(int i = 0; i < nd->ncld; i++)
					dfs_DecList(record, (nd->cld)[i], label, tp);
		}
}

void tk_DE(FieldList record, struct Node* nd, int label)
{
	Type tmp = (Type)malloc(sizeof(struct Type_));
	char* name = NULL;
	tk_SP(tmp, (nd->cld)[0], &name);
	dfs_DecList(record, (nd->cld)[1], label, tmp);
	if(label == 1)
	{
		FieldList reh = record;
		while(record->next != NULL)
		{
			insert_DATAtable(now, record->name, record->type, nd->line, 1, 0);
			Type midtp = record->type;
			if(midtp->kind == ARRAY)
			{
				int size = calarray(midtp);
				InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = DEC;
				mctmp->u.dec.name = record->name;
				mctmp->u.dec.size = size;
				insert_InterCode(mctmp);
			}
			if(midtp->kind == STRUCTURE && midtp->u.structure != NULL)
			{
				int size = calstructure(midtp);
				InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = DEC;
				mctmp->u.dec.name = record->name;
				mctmp->u.dec.size = size;
				insert_InterCode(mctmp);
			}
			record = record->next;
		}
		reh -> name = NULL;
	}
}

void dfs_DefList(FieldList record, struct Node* nd, int label)
{
	if(nd != NULL)
	{
		if(nd->type == 0) return;
		if(!NAME(nd->tag, "Def"))
			tk_DE(record, nd, label);
		else
			for(int i = 0; i < nd->ncld; i++)
				dfs_DefList(record, (nd->cld)[i], label);
	}
}

void tk_SP(Type record, struct Node* nd, char* *name)
{
	if(!NAME((nd->cld)[0]->tag, "TYPE"))
	{
		record -> kind = BASIC;
		if(!NAME((nd->cld)[0]->value, "int"))
			record -> u.basic = 0;
		else record -> u.basic = 1;
	}
	else
	{
		struct Node* STP = (nd->cld)[0];
		record->kind = STRUCTURE;
		if(STP->ncld == 5)
		{
			FieldList tmp = (FieldList)malloc(sizeof(struct FieldList_));
			tmp->name = NULL;
			dfs_DefList(tmp, (STP->cld)[3], 0);
			record->u.structure = tmp;
			if((STP->cld)[1]->ncld == 1)
				*name = ((STP->cld)[1]->cld)[0] -> value;
			if(*name != NULL)
				insert_DATAtable(now, *name, record, nd->line, 2, 0);
		}
		else
		{
			DATAtable x = search_DATAtable(now, ((STP->cld)[1]->cld)[0] -> value, 2);
			*name = ((STP->cld)[1]->cld)[0] -> value;
			if(x==NULL)
			{
				printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",nd->line, *name);
				record->u.structure = NULL;
			}
			else
			{
				record->u.structure = x->type->u.structure;
			}
		}
	}
}


void dfs_ExtDecList(struct Node* nd, Type tp)
{
	if(nd->ncld==1)
	{
		Type ans = NULL;
		char *name = NULL;
		dfs_VarDec((nd->cld)[0], tp, &ans, &name);
		insert_DATAtable(now, name, ans, nd->line, 1, 0);
	}
	else
	{
		Type ans = NULL;
		char *name = NULL;
		dfs_VarDec((nd->cld)[0], tp, &ans, &name);
		insert_DATAtable(now, name, ans, nd->line, 1, 0);
		dfs_ExtDecList((nd->cld)[2], tp);
	}
}


void tk_PA(struct Node* nd, FieldList record)
{
	char* name1 = NULL;
	Type tmp = (Type)malloc(sizeof(struct Type_));
	tk_SP(tmp, (nd->cld)[0], &name1);
	Type ans = NULL;
	char* name = NULL;
	dfs_VarDec((nd->cld)[1], tmp, &ans, &name);
	if(record -> name == NULL)
	{
		FieldList tmp_record = (FieldList)malloc(sizeof(struct FieldList_));
		record->name = name;
		record->type = ans;
		record->next = tmp_record;
		tmp_record->next = NULL;
	}
	else
	{
		FieldList tmp = record;
		int label1 = 1;
		while(tmp->next != NULL)
		{
			if(!NAME(name, tmp->name))
			{
				label1=0;
				printf("Redefined function Parameter\n");
				break;
			}
			tmp = tmp->next;
		}
		if(label1)
		{
			tmp->next = (FieldList)malloc(sizeof(struct FieldList_));
			tmp->name = name;
			tmp->type = ans;
			tmp->next->next=NULL;
		}
	}
}


void dfs_VarList(struct Node* nd, FieldList record)
{
	if(nd->ncld == 1)
	{
		tk_PA((nd->cld)[0], record);
	}
	else
	{
		tk_PA((nd->cld)[0], record);
		dfs_VarList((nd->cld)[2], record);
	}
}

void tk_FD(struct Node* nd, int oval, Type tp, char* *fname)
{
	char* name = (nd->cld)[0]->value;
	FUCtable x = search_FUCtable(name);
	if(nd->ncld == 3)
	{
		if(x == NULL)
		{
			insert_FUCtable(name, oval, tp, NULL, nd->line);
			*fname = name;
		}
		else
		{
			if(x->state == 1 && oval == 1)
				printf("Error type 4 at Line %d: Redefined function \"%s\".\n",nd->line, name);
			else if(x->para != NULL)
				printf("Error type 19 at Line %d: Inconsistent declaration or definition of function \"%s\".\n", nd->line, name);
			else
			{
				if(x->state == 2 && oval == 1)
				{
					int art = check_type(x->type, tp);
					if(art)
					{
						x->state =1;
						x->type = tp;
						*fname = name;
					}
					else
					{
						printf("Error type 19 at Line %d: Confilict about function type.\n", nd->line);
						x->state = 1;
						x->type = tp;
						*fname = name;
					}
				}
			}
		}
	}
	else
	{
		FieldList tmp = (FieldList)malloc(sizeof(struct FieldList_));
		tmp->name = NULL;
		dfs_VarList((nd->cld)[2], tmp);
		if(x == NULL)
		{
			insert_FUCtable(name, oval, tp, tmp, nd->line);
			*fname = name;
		}
		else
		{
			if(x->state == 1)
			{
				if(oval == 1)
					printf("Error type 4 at Line %d: Redefined function \"%s\".\n",nd->line, name);
				else
				{
					int ret = check_FieldList(tmp, x->para);
					int art = check_type(x->type, tp);
					if(!ret || !art) printf("Error type 19 at Line %d: Inconsistent declaration of function \"%s\".\n",nd->line,name);
				}
			}
			else
			{
				if(oval == 2)
				{
					int ret = check_FieldList(tmp, x->para);
					int art = check_type(x->type, tp);
					if(!ret || !art) printf("Error type 19 at Line %d: Inconsistent declaration of function \"%s\".\n",nd->line,name);
				}
				else
				{
					int ret = check_FieldList(tmp, x->para);
					int art = check_type(x->type, tp);
					if(ret) {x->state = 1; *fname = name;x->para = tmp; x->type = tp;}
					if(!art) printf("Error type 19 at Line %d: diffrent from the before declatarion of function \"%s\".\n", nd->line, name);
				}
			}
		}

	}

}

void tk_Args(struct Node* nd, FieldList record, int *num, arglist al)
{
	if(nd->ncld == 1)
	{
		int t1 = new_temp();
		int t2 = new_temp();
		Type ans;
		if(((nd->cld)[0]->ncld) == 1 && NAME("INT", ((nd->cld)[0]->cld)[0]->tag)!=0 )
			ans = tk_Exp((nd->cld)[0], t1, -1, -1, t2);
		else
		{
			ans = tk_Exp((nd->cld)[0], t1, -1, -1, -1);
			if(ans!=NULL && ans->kind != BASIC)
			{
				if((nd->cld)[0]->ncld == 4)
				{
					tk_Exp((nd->cld)[0], t1, -1, -1, t2);
					Operand o1 = (Operand)malloc(sizeof(struct Operand_));
					o1->kind = TEMP;
					o1->u.tmp_no = t1;
					Operand o5 = (Operand)malloc(sizeof(struct Operand_));
					o5->kind = SUBS;
					int sss;
					if(ans->kind == ARRAY)
						sss = calarray(ans);
					else
						sss = calstructure(ans);
//					printf("%d\n",sss);
					o5->u.subs = sss;
					InterCode mctmp=(InterCode)malloc(sizeof(struct InterCode_));
					mctmp->kind = MUL;
					mctmp->u.binop.op1 = o1;
					mctmp->u.binop.op2 = o5;
					mctmp->u.binop.result = o1;
					insert_InterCode(mctmp);
					Operand o2 = (Operand)malloc(sizeof(struct Operand_));
					o2->kind = TEMP;
					o2->u.tmp_no = t2;
					mctmp=(InterCode)malloc(sizeof(struct InterCode_));
					mctmp->kind = ADD;
					mctmp->u.binop.op1 = o1;
					mctmp->u.binop.op2 = o2;
					mctmp->u.binop.result = o2;
					insert_InterCode(mctmp);
//					printf("SSS\n");
				}
				else
				{
					tk_Exp((nd->cld)[0], t1, -1, -1, t2);
//					printf("%d\n", calstructure(ans));
				}
			}
		}		
		if(ans!=NULL)
		{
			*num = *num + 1;
			int t0;
			if(ans->kind != BASIC)
				t0 = t2;
			else t0 = t1;

			if(al->tag == -1)
			{
				arglist nar = (arglist)malloc(sizeof(struct arglist_));
				al->tar = ans;
				al->tag = t0;
				al->next = nar;
				nar->prev = al;
			}
			else
			{
				arglist tmp = al;
				while(tmp->next!=NULL)
				{
					tmp = tmp->next;
				}
				arglist nar = (arglist)malloc(sizeof(struct arglist_));
				tmp->tar = ans;
				tmp->tag = t0;
				tmp->next = nar;
				nar->prev = tmp;
			}

			if(record->type == NULL)
			{
				FieldList new = (FieldList)malloc(sizeof(struct FieldList_));
				record->type = ans;
				record->next = new;
				record->name = NULL;
			}
			else
			{
				FieldList tmp = record;
				while(tmp->next!=NULL)
				{
					tmp = tmp -> next;
				}
				FieldList new = (FieldList)malloc(sizeof(struct FieldList_));
				tmp->type = ans;
				tmp->next = new;
				tmp->name = NULL;
			}
		}
		
	}
	else
	{
		int t1 = new_temp();
		int t2 = new_temp();
		Type ans;
		if(((nd->cld)[0]->ncld) == 1 && NAME("INT", ((nd->cld)[0]->cld)[0]->tag)!=0 )
			ans = tk_Exp((nd->cld)[0], t1, -1, -1, t2);
		else
		{
			ans = tk_Exp((nd->cld)[0], t1, -1, -1, -1);
			if(ans!=NULL && ans->kind != BASIC)
			{
				if((nd->cld)[0]->ncld == 4)
				{
					tk_Exp((nd->cld)[0], t1, -1, -1, t2);
					Operand o1 = (Operand)malloc(sizeof(struct Operand_));
					o1->kind = TEMP;
					o1->u.tmp_no = t1;
					Operand o5 = (Operand)malloc(sizeof(struct Operand_));
					o5->kind = SUBS;
					int sss;
					if(ans->kind == ARRAY)
						sss = calarray(ans);
					else
						sss = calstructure(ans);
					o5->u.subs = sss;
					InterCode mctmp=(InterCode)malloc(sizeof(struct InterCode_));
					mctmp->kind = MUL;
					mctmp->u.binop.op1 = o1;
					mctmp->u.binop.op2 = o5;
					mctmp->u.binop.result = o1;
					insert_InterCode(mctmp);
					Operand o2 = (Operand)malloc(sizeof(struct Operand_));
					o2->kind = TEMP;
					o2->u.tmp_no = t2;
					mctmp=(InterCode)malloc(sizeof(struct InterCode_));
					mctmp->kind = ADD;
					mctmp->u.binop.op1 = o1;
					mctmp->u.binop.op2 = o2;
					mctmp->u.binop.result = o1;
					insert_InterCode(mctmp);
				}
				else
				{
					tk_Exp((nd->cld)[0], t1, -1, -1, t2);
				}
			}
		}
		if(ans!=NULL)
		{
			*num = *num + 1;
//			if(ans->kind != BASIC)
//				t1 = -t1;
			int t0;
			if(ans->kind != BASIC)
				t0 = t2;
			else t0 = t1;

			if(al->tag == -1)
			{
				arglist nar = (arglist)malloc(sizeof(struct arglist_));
				al->tar = ans;
				al->tag = t0;
				al->next = nar;
				nar->prev = al;
			}
			else
			{
				arglist tmp = al;
				while(tmp->next!=NULL)
				{
					tmp = tmp->next;
				}
				arglist nar = (arglist)malloc(sizeof(struct arglist_));
				tmp->tar = ans;
				tmp->tag = t0;
				tmp->next = nar;
				nar->prev = tmp;
			}


			if(record->type == NULL)
			{
				FieldList new = (FieldList)malloc(sizeof(struct FieldList_));
				record->type = ans;
				record->next = new;
				record->name = NULL;
			}
			else
			{
				FieldList tmp = record;
				while(tmp->next!=NULL)
				{
					tmp = tmp -> next;
				}
				FieldList new = (FieldList)malloc(sizeof(struct FieldList_));
				tmp->type = ans;
				tmp->next = new;
				tmp->name = NULL;
			}
		}
		

	tk_Args((nd->cld)[2], record, num, al);
	}
}


Type tk_Exp(struct Node* nd, int temp_no, int l1, int l2, int VOA)
{
	Type ret = NULL;
	if(l1 != -1)
	{
		int llb = 1;
		if(nd->ncld == 2)
		{
			if(!NAME((nd->cld)[0]->tag, "NOT"))
				llb = 0;
		}
		if(nd->ncld == 3)
		{
			if(!NAME((nd->cld)[1]->tag, "RELOP") || !NAME((nd->cld)[1]->tag, "AND") || !NAME((nd->cld)[1]->tag, "OR"))
				llb = 0;
		}
		if(llb == 1)
		{
			int t1 = new_temp();
			ret = tk_Exp(nd, t1, -1, -1, -1);
			Operand ol = (Operand)malloc(sizeof(struct Operand_));
			ol->kind = TEMP;
			ol->u.tmp_no = t1;
			Operand oR = (Operand)malloc(sizeof(struct Operand_));
			oR->kind = CONSTANT;
			oR->u.value = C0;
			Operand rop = (Operand)malloc(sizeof(struct Operand_));
			rop->kind = OP;
			rop->u.op = NEQ;
			Operand lb = (Operand)malloc(sizeof(struct Operand_));
			lb->kind = LABEL;
			lb->u.lab_no = l1;
			InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = IFGT;
			mctmp->u.ig.rop = rop;
			mctmp->u.ig.OL = ol;
			mctmp->u.ig.OR = oR;
			mctmp->u.ig.ltf = lb;
			insert_InterCode(mctmp);

			Operand lonely = (Operand)malloc(sizeof(struct Operand_));
			lonely->kind = LABEL;
			lonely->u.lab_no = l2;
			mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = GT;
			mctmp->u.lonely = lonely;
			insert_InterCode(mctmp);
//			printf("%s : SSS\n", (nd->cld)[1]->tag);

			return ret;

		}
	}

	if(nd->ncld == 1)
	{
		if(!NAME((nd->cld)[0]->tag, "ID"))
		{
			DATAtable ans = search_DATAtable(now, (nd->cld)[0]->value, 1);
			if(ans == NULL)
				printf("Error type 1 at Line %d: Undefined variable \"%s\".\n", nd->line, (nd->cld)[0]->value);
			else
			{
				ret = ans->type;


				if(VOA ==- 1){
				Operand right = (Operand)malloc(sizeof(struct Operand_));
				right->kind = VARIABLE;
				right->u.name = (nd->cld)[0]->value;
				Operand left = (Operand)malloc(sizeof(struct Operand_));
				left->kind = TEMP;
				left->u.tmp_no = temp_no;
				InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = ASSIGN;
				mctmp->u.assign.right = right;
				mctmp->u.assign.left = left;
				insert_InterCode(mctmp);
				}
				else
				{
				Operand right = (Operand)malloc(sizeof(struct Operand_));
				right->kind = VARIABLE;
				right->u.name = (nd->cld)[0]->value;
				Operand left = (Operand)malloc(sizeof(struct Operand_));
				left->kind = TEMP;
				left->u.tmp_no = VOA;
				InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
				if(ans->inout == 0)
				{
					mctmp->kind = ASSIGNB;
				}
				else
				{
					mctmp->kind = ASSIGN;
				}
				mctmp->u.assign.right = right;
				mctmp->u.assign.left = left;
				insert_InterCode(mctmp);

				right = (Operand)malloc(sizeof(struct Operand_));
				if(ans->type->kind != BASIC)
				{
					right->kind = CONSTANT;
					right->u.value = C0;
				}
				else
				{
					right->kind = VARIABLE;
					right->u.name = (nd->cld)[0]->value;
				}
				left = (Operand)malloc(sizeof(struct Operand_));
				left->kind = TEMP;
				left->u.tmp_no = temp_no;
				mctmp = (InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = ASSIGN;
				mctmp->u.assign.right = right;
				mctmp->u.assign.left = left;
				insert_InterCode(mctmp);
				}
			}
		}
		else
		{
			ret = (Type)malloc(sizeof(struct Type_));
			if(!NAME((nd->cld)[0]->tag, "INT"))
			{
				ret->kind = 0;
				ret -> u.basic = 0;

				Operand right = (Operand)malloc(sizeof(struct Operand_));
				right->kind = CONSTANT;
				right->u.value = (nd->cld)[0]->value;
				Operand left = (Operand)malloc(sizeof(struct Operand_));
				left->kind = TEMP;
				left->u.tmp_no = temp_no;
				InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = ASSIGN;
				mctmp->u.assign.right = right;
				mctmp->u.assign.left = left;
				insert_InterCode(mctmp);
			}
			else
			{
				ret->kind = 0;
				ret-> u.basic = 1;
			}
		}
	}
	else if(nd->ncld == 2)
	{
		if(!NAME((nd->cld)[0]->tag, "NOT") && l1 != -1)
		{
			ret = tk_Exp((nd->cld)[1], -1, l2, l1, -1);
			if(ret == NULL)
			{
				printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);
			}
			else
			{
				if(ret -> kind != 0)
				{
					printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);
				}
				else
				{
					if(ret -> u.basic != 0)
						printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);
				}
			}
		}
		else if(!NAME((nd->cld)[0]->tag, "NOT") && l1 == -1)
		{
			int label1 = new_label();
			int label2 = new_label();
			Operand right = (Operand)malloc(sizeof(struct Operand_));
			right->kind = CONSTANT;
			right->u.value = C0;
			Operand left = (Operand)malloc(sizeof(struct Operand_));
			left->kind = TEMP;
			left->u.tmp_no = temp_no;
			InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = ASSIGN;
			mctmp->u.assign.right = right;
			mctmp->u.assign.left = left;
			insert_InterCode(mctmp);

			ret = tk_Exp(nd, -1, label1, label2, -1);

			Operand lonely = (Operand)malloc(sizeof(struct Operand_));
			lonely->kind = LABEL;
			lonely->u.lab_no = label1;
			mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = LAB;
			mctmp->u.lonely = lonely;
			insert_InterCode(mctmp);

			right = (Operand)malloc(sizeof(struct Operand_));
			right->kind = CONSTANT;
			right->u.value = C1;
			left = (Operand)malloc(sizeof(struct Operand_));
			left->kind = TEMP;
			left->u.tmp_no = temp_no;
			mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = ASSIGN;
			mctmp->u.assign.right = right;
			mctmp->u.assign.left = left;
			insert_InterCode(mctmp);

			lonely = (Operand)malloc(sizeof(struct Operand_));
			lonely->kind = LABEL;
			lonely->u.lab_no = label2;
			mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = LAB;
			mctmp->u.lonely = lonely;
			insert_InterCode(mctmp);


		}
		else
		{
			int t1 = new_temp();

			ret = tk_Exp((nd->cld)[1], t1, -1, -1, -1);

			Operand op1 = (Operand)malloc(sizeof(struct Operand_));
			op1->kind = CONSTANT;
			op1->u.value = C0;
			Operand op2 = (Operand)malloc(sizeof(struct Operand_));
			op2->kind = TEMP;
			op2->u.tmp_no = t1;
			Operand result = (Operand)malloc(sizeof(struct Operand_));
			result->kind = TEMP;
			result->u.tmp_no = temp_no;
			InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = SUB;
			mctmp->u.binop.op1 = op1;
			mctmp->u.binop.op2 = op2;
			mctmp->u.binop.result = result;
			insert_InterCode(mctmp);

			if(ret == NULL)
			{
				printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);		
			}
			else
			{
				if(ret -> kind != 0)
				{
					printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);
				}
			}
		}
	}
	else if(nd->ncld == 3)
	{
		if((!NAME((nd->cld)[1]->tag, "AND") || !NAME((nd->cld)[1]->tag, "OR")) && l1 != -1)
		{
			int label1 = new_label();
			Type ret1, ret2;
			if(!NAME((nd->cld)[1]->tag, "AND"))
			{
				ret1 = tk_Exp((nd->cld)[0], -1, label1, l2, -1);
				Operand lonely = (Operand)malloc(sizeof(struct Operand_));
				lonely->kind = LABEL;
				lonely->u.lab_no = label1;
				InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = LAB;
				mctmp->u.lonely = lonely;
				insert_InterCode(mctmp);
				ret2 = tk_Exp((nd->cld)[2], -1, l1, l2, -1);
			}
			if(!NAME((nd->cld)[1]->tag, "OR"))
			{
//				Type ret1 = tk_Exp((nd->cld)[0], -1, -1, -1);
//				Type ret2 = tk_Exp((nd->cld)[2], -1, -1, -1);
				ret1 = tk_Exp((nd->cld)[0], -1, l1, label1, -1);
				Operand lonely = (Operand)malloc(sizeof(struct Operand_));
				lonely->kind = LABEL;
				lonely->u.lab_no = label1;
				InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = LAB;
				mctmp->u.lonely = lonely;
				insert_InterCode(mctmp);
				ret2 = tk_Exp((nd->cld)[2], -1, l1, l2, -1);
			}
			if(ret1 == NULL || ret2 == NULL)
			{
				printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);
			}
			else
			{
				if(ret1->kind != 0 || ret2->kind != 0)
				{
					printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);
				}
				else
				{
					if(ret1->u.basic!=0||ret2->u.basic!=0)
						printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);
					else
						ret = ret1;
				}
			}
		}
		if(!NAME((nd->cld)[1]->tag, "PLUS")|| !NAME((nd->cld)[1]->tag, "MINUS")|| !NAME((nd->cld)[1]->tag, "STAR")|| !NAME((nd->cld)[1]->tag, "DIV"))
		{
			int t1 = new_temp();
			int t2 = new_temp();
			Type ret1 = tk_Exp((nd->cld)[0], t1, l1, l2, -1);
			Type ret2 = tk_Exp((nd->cld)[2], t2, l1, l2, -1);
			if(ret1 == NULL || ret2 == NULL)
			{
				printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);
			}
			else
			{
				if(ret1->kind != 0 || ret2->kind != 0)
				{
					printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);
				}
				else
				{
					if(ret1->u.basic != ret2->u.basic)
						printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);
					else
					{
							ret = ret1;
							Operand op1 = (Operand)malloc(sizeof(struct Operand_));
							op1->kind = TEMP;
							op1->u.tmp_no = t1;
							Operand op2 = (Operand)malloc(sizeof(struct Operand_));
							op2->kind = TEMP;
							op2->u.tmp_no = t2;
							Operand result = (Operand)malloc(sizeof(struct Operand_));
							result->kind = TEMP;
							result->u.tmp_no = temp_no;
							InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
							if(!NAME((nd->cld)[1]->tag, "MINUS"))
								mctmp->kind = SUB;
							if(!NAME((nd->cld)[1]->tag, "PLUS"))
								mctmp->kind = ADD;
							if(!NAME((nd->cld)[1]->tag, "STAR"))
								mctmp->kind = MUL;
							if(!NAME((nd->cld)[1]->tag, "DIV"))
								mctmp->kind = DIV;
							mctmp->u.binop.op1 = op1;
							mctmp->u.binop.op2 = op2;
							mctmp->u.binop.result = result;
							insert_InterCode(mctmp);

					}
				}
			}
		}
		if((!NAME((nd->cld)[1]->tag, "RELOP") || !NAME((nd->cld)[1]->tag, "AND") || !NAME((nd->cld)[1]->tag, "OR"))&& l1 == -1)
		{


			int label1 = new_label();
			int label2 = new_label();
			Operand right = (Operand)malloc(sizeof(struct Operand_));
			right->kind = CONSTANT;
			right->u.value = C0;
			Operand left = (Operand)malloc(sizeof(struct Operand_));
			left->kind = TEMP;
			left->u.tmp_no = temp_no;
			InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = ASSIGN;
			mctmp->u.assign.right = right;
			mctmp->u.assign.left = left;
			insert_InterCode(mctmp);

			ret = tk_Exp(nd, -1, label1, label2, -1);

			Operand lonely = (Operand)malloc(sizeof(struct Operand_));
			lonely->kind = LABEL;
			lonely->u.lab_no = label1;
			mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = LAB;
			mctmp->u.lonely = lonely;
			insert_InterCode(mctmp);

			right = (Operand)malloc(sizeof(struct Operand_));
			right->kind = CONSTANT;
			right->u.value = C1;
			left = (Operand)malloc(sizeof(struct Operand_));
			left->kind = TEMP;
			left->u.tmp_no = temp_no;
			mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = ASSIGN;
			mctmp->u.assign.right = right;
			mctmp->u.assign.left = left;
			insert_InterCode(mctmp);

			lonely = (Operand)malloc(sizeof(struct Operand_));
			lonely->kind = LABEL;
			lonely->u.lab_no = label2;
			mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = LAB;
			mctmp->u.lonely = lonely;
			insert_InterCode(mctmp);




		}
		if(!NAME((nd->cld)[1]->tag, "RELOP") && l1 != -1)
		{


			int t1 = new_temp();
			int t2 = new_temp();
			Type ret1 = tk_Exp((nd->cld)[0], t1, -1, -1, -1);
			Type ret2 = tk_Exp((nd->cld)[2], t2, -1, -1, -1);
			Operand ol = (Operand)malloc(sizeof(struct Operand_));
			ol->kind = TEMP;
			ol->u.tmp_no = t1;
			Operand oR = (Operand)malloc(sizeof(struct Operand_));
			oR->kind = TEMP;
			oR->u.tmp_no = t2;
			Operand rop = (Operand)malloc(sizeof(struct Operand_));
			rop->kind = OP;
			rop->u.op = (nd->cld)[1]->value;
			Operand lb = (Operand)malloc(sizeof(struct Operand_));
			lb->kind = LABEL;
			lb->u.lab_no = l1;
			InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = IFGT;
			mctmp->u.ig.rop = rop;
			mctmp->u.ig.OL = ol;
			mctmp->u.ig.OR = oR;
			mctmp->u.ig.ltf = lb;
			insert_InterCode(mctmp);

			Operand lonely = (Operand)malloc(sizeof(struct Operand_));
			lonely->kind = LABEL;
			lonely->u.lab_no = l2;
			mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = GT;
			mctmp->u.lonely = lonely;
			insert_InterCode(mctmp);





			if(ret1 == NULL || ret2 == NULL)
			{
				printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);
			}
			else
			{
				if(ret1->kind != 0 || ret2->kind != 0)
				{
					printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);
				}
				else
				{
					if(ret1->u.basic != ret2->u.basic)
						printf("Error type 7 at Line %d: Type mismatched for operands.\n", nd->line);
					else
					{
							ret = ret1;
					}
				}


			}

		}

		if(!NAME((nd->cld)[0]->tag, "LP"))
		{
			ret = tk_Exp((nd->cld)[1], temp_no, l1, l2, -1);
		}
		if(!NAME((nd->cld)[1]->tag, "ASSIGNOP"))
		{
			
			
//			Type ret1 = tk_Exp((nd->cld)[0], -1, -1, -1);
//			Type ret2 = tk_Exp((nd->cld)[2], -1, -1, -1);
			struct Node* second = (nd->cld)[0];
			int rec = 0;
			if(second->ncld == 1)
				if(!NAME((second->cld)[0]->tag, "ID"))
					rec = 1;
			if(second->ncld == 3)
				if(!NAME((second->cld)[1]->tag, "DOT"))
					rec = 1;
			if(second->ncld == 4)
				if(!NAME((second->cld)[1]->tag, "LB"))
					rec = 1;
			if(!rec)
				printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", nd->line);
			
			Type ret1 = NULL;
			Type ret2 = NULL;
			if(second->ncld == 1 && rec == 1)
			{
				int t1 = new_temp();
				struct Node* E1 = (nd->cld)[0];
				DATAtable ans = search_DATAtable(now, (E1->cld)[0]->value, 1);
				if(ans == NULL)
					printf("Error type 1 at Line %d: Undefined variable \"%s\".\n", E1->line, (E1->cld)[0]->value);
				else ret1 = ans->type;


				if(ans->type->kind == BASIC){
				ret2 = tk_Exp((nd->cld)[2], t1, -1, -1, -1);


				Operand v1 = (Operand)malloc(sizeof(struct Operand_));
				v1->kind = VARIABLE;
				v1->u.name = ((nd->cld)[0]->cld)[0]->value;

				Operand temp1 = (Operand)malloc(sizeof(struct Operand_));
				temp1->kind = TEMP;
				temp1->u.tmp_no = t1;

				Operand temp2 = (Operand)malloc(sizeof(struct Operand_));
				temp2->kind = TEMP;
				temp2->u.tmp_no = temp_no;

				InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = ASSIGN;
				mctmp->u.assign.right = temp1;
				mctmp->u.assign.left = v1;
				insert_InterCode(mctmp);

				InterCode mctmp1 = (InterCode)malloc(sizeof(struct InterCode_));
				mctmp1->kind = ASSIGN;
				mctmp1->u.assign.right = v1;
				mctmp1->u.assign.left = temp2;
				insert_InterCode(mctmp1);
				}
				else
				{
					struct Node* E1 = (nd->cld)[0];
					DATAtable ans1=search_DATAtable(now, (E1->cld)[0]->value, 1);
					struct Node* E2 = (nd->cld)[2];
					DATAtable ans2=search_DATAtable(now, (E2->cld)[0]->value, 1);
					int size1 = calsize(ans1->type)/4;
					int size2 = calsize(ans2->type)/4;
					int fsize;
					if(size1 < size2)
						fsize = size1;
					else
						fsize = size2;
					int zt1 = new_temp();
					int zt2 = new_temp();
					Operand o1 = (Operand)malloc(sizeof(struct Operand_));
					o1->kind = TEMP;
					o1->u.tmp_no = zt1;
					Operand o2 = (Operand)malloc(sizeof(struct Operand_));
					o2->kind = VARIABLE;
					o2->u.name = (E1->cld)[0]->value;
					InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
					mctmp->kind = ASSIGNB;
					mctmp->u.assign.left = o1;
					mctmp->u.assign.right = o2;
					insert_InterCode(mctmp);

					Operand o3 = (Operand)malloc(sizeof(struct Operand_));
					o3->kind = TEMP;
					o3->u.tmp_no = zt2;
					Operand o4 = (Operand)malloc(sizeof(struct Operand_));
					o4->kind = VARIABLE;
					o4->u.name = (E2->cld)[0]->value;
					mctmp = (InterCode)malloc(sizeof(struct InterCode_));
					mctmp->kind = ASSIGNB;
					mctmp->u.assign.left = o3;
					mctmp->u.assign.right = o4;
					insert_InterCode(mctmp);

					int tt = new_temp();
					Operand om = (Operand)malloc(sizeof(struct Operand_));
					om->kind = TEMP;
					om->u.tmp_no = tt;
					Operand o6 = (Operand)malloc(sizeof(struct Operand_));
					o6->kind = SUBS;
					o6->u.subs = 4;

					while(fsize > 0)
					{
						mctmp=(InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = ASSIGNA;
						mctmp->u.assign.left = om;
						mctmp->u.assign.right = o3;
						insert_InterCode(mctmp);
						mctmp=(InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = ASSIGNC;
						mctmp->u.assign.left = o1;
						mctmp->u.assign.right = om;
						insert_InterCode(mctmp);
						mctmp=(InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = ADD;
						mctmp->u.binop.op1 = o1;
						mctmp->u.binop.op2 = o6;
						mctmp->u.binop.result = o1;
						insert_InterCode(mctmp);
						mctmp=(InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = ADD;
						mctmp->u.binop.op1 = o3;
						mctmp->u.binop.op2 = o6;
						mctmp->u.binop.result = o3;
						insert_InterCode(mctmp);
						fsize = fsize-1;
					}

				}

			}
			else if(second->ncld == 3 && rec == 1)
			{
				int t1 = new_temp();
				int t2 = new_temp();
				int t3 = new_temp();

				ret1 = tk_Exp((nd->cld)[0], t1, -1, -1, t2);
				ret2 = tk_Exp((nd->cld)[2], t3, -1, -1, -1);

				Operand o1 = (Operand)malloc(sizeof(struct Operand_));
				o1->kind = TEMP;
				o1->u.tmp_no = t2;
				Operand o2 = (Operand)malloc(sizeof(struct Operand_));
				o2->kind = TEMP;
				o2->u.tmp_no = t3;
				Operand o3 = (Operand)malloc(sizeof(struct Operand_));
				o3->kind = TEMP;
				o3->u.tmp_no = temp_no;

				InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = ASSIGNC;
				mctmp->u.assign.left = o1;
				mctmp->u.assign.right = o2;
				insert_InterCode(mctmp);
				mctmp = (InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = ASSIGNA;
				mctmp->u.assign.left = o3;
				mctmp->u.assign.right = o1;
				insert_InterCode(mctmp);
			}
			else if(second->ncld == 4 && rec == 1)
			{
				int t1 = new_temp();
				int t2 = new_temp();
				int t3 = new_temp();

				ret1 = tk_Exp((nd->cld)[0], t1, -1, -1, t2);
				ret2 = tk_Exp((nd->cld)[2], t3, -1, -1, -1);
				Operand o1 = (Operand)malloc(sizeof(struct Operand_));
				o1->kind = TEMP;
				o1->u.tmp_no = t1;
				Operand o2 = (Operand)malloc(sizeof(struct Operand_));
				o2->kind = TEMP;
				o2->u.tmp_no = t2;
				Operand o3 = (Operand)malloc(sizeof(struct Operand_));
				o3->kind = TEMP;
				o3->u.tmp_no = t3;
				Operand o4 = (Operand)malloc(sizeof(struct Operand_));
				o4->kind = TEMP;
				o4->u.tmp_no = temp_no;
				Operand o5 = (Operand)malloc(sizeof(struct Operand_));
				o5->kind = SUBS;
				o5->u.subs = 4;
				InterCode mctmp=(InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = MUL;
				mctmp->u.binop.op1 = o1;
				mctmp->u.binop.op2 = o5;
				mctmp->u.binop.result = o1;
				insert_InterCode(mctmp);
				mctmp=(InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = ADD;
				mctmp->u.binop.op1 = o1;
				mctmp->u.binop.op2 = o2;
				mctmp->u.binop.result = o1;
				insert_InterCode(mctmp);
				mctmp=(InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = ASSIGNC;
				mctmp->u.assign.left = o1;
				mctmp->u.assign.right = o3;
				insert_InterCode(mctmp);
				mctmp = (InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = ASSIGNA;
				mctmp->u.assign.left = o4;
				mctmp->u.assign.right = o1;
				insert_InterCode(mctmp);

			}
			else
			{
				ret1 = tk_Exp((nd->cld)[0], -1, -1, -1, -1);
				ret2 = tk_Exp((nd->cld)[2], -1, -1, -1, -1);
			}
			
			
			int ans = check_type(ret1, ret2);
			if((ans==0) && rec)
				printf("Error type 5 at Line %d: Type mismatched for assignment.\n", nd->line);
			else ret = ret2;
		}
		if(!NAME((nd->cld)[0]->tag, "ID"))
		{
			FUCtable ans= search_FUCtable((nd->cld)[0]->value);
			if(ans == NULL)
			{
				DATAtable what = search_DATAtable(now, (nd->cld)[0]->value, 3);
				if(what != NULL)
					printf("Error type 11 at Line %d: \"%s\" is not a function.\n", nd->line, (nd->cld)[0]->value);
				else
					printf("Error type 2 at Line %d: Undefined function \"%s\".\n", nd->line, (nd->cld)[0]->value);
			}
			else
			{
				if(ans->para != NULL)
					printf("Error type 9 at Line %d: Wrong parameters.\n", nd->line);
				else
				{
					if(!NAME(ans -> name, "read"))
					{
						Operand ipt = (Operand)malloc(sizeof(struct Operand_));
						ipt->kind = TEMP;
						ipt->u.tmp_no = temp_no;
						InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = READ;
						mctmp->u.lonely = ipt;
						insert_InterCode(mctmp);
					}
					else
					{
						Operand ipt = (Operand)malloc(sizeof(struct Operand_));
						ipt->kind = TEMP;
						ipt->u.tmp_no = temp_no;
						InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
						Operand lonely = (Operand)malloc(sizeof(struct Operand_));
						lonely->kind = VARIABLE;
						lonely->u.name = ans->name;
						mctmp->kind = CALL;
						mctmp->u.call.tv = ipt;
						mctmp->u.call.fname = lonely;
						insert_InterCode(mctmp);

					}
					ret = ans->type;
				}
			}
		}
		if(!NAME((nd->cld)[1]->tag, "DOT"))
		{
			Type ta;

			int t1 = new_temp();
			int t2=-2;
			
			if(VOA != -1)
			{
				ta = tk_Exp((nd->cld)[0], t1, -1, -1, VOA);
			}
			else
			{
				t2 = new_temp();
				ta = tk_Exp((nd->cld)[0], t1, -1, -1, t2);
			}
			if(ta == NULL)
			{
				printf("Error type 13 at Line %d: Illegal use of \".\".\n", nd->line);
			}
			else
			{
				if(ta -> kind != 2)
					printf("Error type 13 at Line %d: Illegal use of \".\".\n", nd->line);
				else
				{
					FieldList ans = ta->u.structure;
					int label = 0;
					int tsize = 0;
					int asize = 0;
					while(ans -> next != NULL)
					{
						if(!NAME(ans->name, (nd->cld)[2]->value) && label==0)
							
						{
							label = 1;
							ret = ans -> type;
						}
						int tmps = calsize(ans->type);
						if(label == 0)
							tsize = tsize + tmps;

						asize = asize + tmps;
						ans = ans->next;
					}
					if(!label)
						printf("Error type 14 at Line %d: Non-existent field \"%s\".\n",nd->line,(nd->cld)[2]->value);
					else
					{
						if(VOA != -1)
						{
							Operand o1=(Operand)malloc(sizeof(struct Operand_));
							o1->kind = TEMP;
							o1->u.tmp_no = t1;
							Operand o2=(Operand)malloc(sizeof(struct Operand_));
							Operand o4=(Operand)malloc(sizeof(struct Operand_));
							o4->kind = SUBS;
							o4->u.subs = asize;

							
							o2->kind = SUBS;
							o2->u.subs = tsize;

							Operand o3=(Operand)malloc(sizeof(struct Operand_));
							o3->kind = TEMP;
							o3->u.tmp_no = VOA;/*????*/
							InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
							mctmp->kind = MUL;
							mctmp->u.binop.op1 = o1;
							mctmp->u.binop.op2 = o4;
							mctmp->u.binop.result = o1;
							insert_InterCode(mctmp);

							mctmp=(InterCode)malloc(sizeof(struct InterCode_));
							mctmp->kind = ADD;
							mctmp->u.binop.op1 = o1;
							mctmp->u.binop.op2 = o2;
							mctmp->u.binop.result = o1;
							insert_InterCode(mctmp);

							mctmp=(InterCode)malloc(sizeof(struct InterCode_));
							mctmp->kind = ADD;
							mctmp->u.binop.op1 = o1;
							mctmp->u.binop.op2 = o3;
							mctmp->u.binop.result = o3;
							insert_InterCode(mctmp);

							Operand o5=(Operand)malloc(sizeof(struct Operand_));
							o5->kind = TEMP;
							o5->u.tmp_no = temp_no;
							Operand o6=(Operand)malloc(sizeof(struct Operand_));
							o6->kind = CONSTANT;
							o6->u.value = C0;
							mctmp = (InterCode)malloc(sizeof(struct InterCode_));
							mctmp->kind = ASSIGN;
							mctmp->u.assign.left = o5;
							mctmp->u.assign.right = o6;
							insert_InterCode(mctmp);
						}
						else
						{
							Operand o1=(Operand)malloc(sizeof(struct Operand_));
							o1->kind = TEMP;
							o1->u.tmp_no = t1;
							Operand o2=(Operand)malloc(sizeof(struct Operand_));
							Operand o4=(Operand)malloc(sizeof(struct Operand_));
							o4->kind = SUBS;
							o4->u.subs = asize;

							
							o2->kind = SUBS;
							o2->u.subs = tsize;

							Operand o3=(Operand)malloc(sizeof(struct Operand_));
							o3->kind = TEMP;
							o3->u.tmp_no = t2;
							InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
							mctmp->kind = MUL;
							mctmp->u.binop.op1 = o1;
							mctmp->u.binop.op2 = o4;
							mctmp->u.binop.result = o1;
							insert_InterCode(mctmp);

							mctmp=(InterCode)malloc(sizeof(struct InterCode_));
							mctmp->kind = ADD;
							mctmp->u.binop.op1 = o1;
							mctmp->u.binop.op2 = o2;
							mctmp->u.binop.result = o1;
							insert_InterCode(mctmp);

							mctmp=(InterCode)malloc(sizeof(struct InterCode_));
							mctmp->kind = ADD;
							mctmp->u.binop.op1 = o1;
							mctmp->u.binop.op2 = o3;
							mctmp->u.binop.result = o3;
							insert_InterCode(mctmp);

							Operand o5=(Operand)malloc(sizeof(struct Operand_));
							o5->kind = TEMP;
							o5->u.tmp_no = temp_no;

							
							mctmp=(InterCode)malloc(sizeof(struct InterCode_));
							mctmp->kind = ASSIGNA;
							mctmp->u.assign.left = o5;
							mctmp->u.assign.right = o3;
							insert_InterCode(mctmp);
						}
					}
				}
			}
		}


	}
	if(nd->ncld == 4)
	{
		if(!NAME((nd->cld)[0]->tag, "ID"))
		{
			FieldList rec = (FieldList)malloc(sizeof(struct FieldList_));
			rec -> name = NULL;
			int pnum1=0;
			arglist arg = (arglist)malloc(sizeof(struct arglist_));
			arg->tag = -1;
			arg->prev = NULL;
			arg->next = NULL;
			tk_Args((nd->cld)[2], rec, &pnum1, arg);
			int pnum2=0;
			FUCtable ans = search_FUCtable((nd->cld)[0]->value);
			if(ans == NULL)
			{
				DATAtable what = search_DATAtable(now, (nd->cld)[0]->value, 3);
				if(what != NULL)
					printf("Error type 11 at Line %d: \"%s\" is not a function.\n", nd->line, (nd->cld)[0]->value);
				else
					printf("Error type 2 at Line %d: Undefined function \"%s\".\n", nd->line, (nd->cld)[0]->value);	
			}
			else
			{
				FieldList tans = ans->para;
				if(ans->para == NULL)
					pnum2=0;
				else
				{
					while(tans->next != NULL)
					{
						pnum2++;
						tans=tans->next;
					}
				}
				if(pnum1!=pnum2)
					printf("Error type 9 at Line %d: The number of parameters are not the same.\n", nd->line);
				else{
				int ry = check_FieldList(rec, ans->para);
				if(!ry) printf("Error type 9 at Line %d: Wrong arguments.\n", nd->line);
				else 
				{
					if(!NAME(ans->name, "write"))
					{
						Operand ipt = (Operand)malloc(sizeof(struct Operand_));
						ipt->kind = TEMP;
						ipt->u.tmp_no = arg->tag;
						InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = WRITE;
						mctmp->u.lonely = ipt;
						insert_InterCode(mctmp);

						ipt = (Operand)malloc(sizeof(struct Operand_));
						ipt->kind = TEMP;
						ipt->u.tmp_no = temp_no;
						Operand rv =(Operand)malloc(sizeof(struct Operand_));
						rv->kind = CONSTANT;
						rv->u.name = C0;
						mctmp = (InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = ASSIGN;
						mctmp->u.assign.left = ipt;
						mctmp->u.assign.right = rv;
						insert_InterCode(mctmp);
					}
					else
					{
						arglist argt = arg;
						while(argt->next != NULL)
						{
							argt = argt->next;
						}
						argt = argt->prev;
						while(argt!= NULL)
						{
							Operand tpa = (Operand)malloc(sizeof(struct Operand_));
							tpa->kind = TEMP;
							tpa->u.tmp_no = argt->tag;
//							printf("%d\n", argt->tag);
							InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
							mctmp->kind = ARG;
							mctmp->u.lonely = tpa;
							insert_InterCode(mctmp);
							argt = argt->prev;
						}
						Operand tt1 = (Operand)malloc(sizeof(struct Operand_));
						tt1->kind = TEMP;
						tt1->u.tmp_no = temp_no;
						Operand f1=(Operand)malloc(sizeof(struct Operand_));
						f1->kind = VARIABLE;
						f1->u.name = ans->name;
						InterCode mctmp=(InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = CALL;
						mctmp->u.call.fname=f1;
						mctmp->u.call.tv=tt1;
						insert_InterCode(mctmp);


					}
					
					ret = ans->type;
				}
				}
			}
		}
		else
		{
			int t1 = new_temp();
			Type ans = tk_Exp((nd->cld)[2], t1, -1, -1, -1);
			if(ans==NULL)
			{
				printf("Error type 12 at Line %d: Wrong subscript.\n", nd->line);
			}
			else
			{
				if(ans->kind!=0)
					printf("Error type 12 at Line %d: Wrong subscript.\n", nd->line);
				else
				{
					if(ans->u.basic!=0)
						printf("Error type 12 at Line %d: Wrong subscript.\n", nd->line);
				
				}
			}

			int t2 = new_temp();
			int t3 = -2;
			Type AR;
			if(VOA != -1)
			{
				AR = tk_Exp((nd->cld)[0], t2, -1, -1, VOA);
			}
			else
			{
				t3 = new_temp();
				AR = tk_Exp((nd->cld)[0], t2, -1, -1, t3);

			}
			if(AR ==NULL)
			{
				printf("Error type 10 at Line %d: Variable is not an array.\n", nd->line);
			}
			else
			{
				if(AR->kind != 1)
				{
					printf("Error type 10 at Line %d: Variable is not an array.\n", nd->line);
				}
				else
				{
					if(VOA != -1)
					{
						Operand o1=(Operand)malloc(sizeof(struct Operand_));
						o1->kind = TEMP;
						o1->u.tmp_no = t2;
						Operand o2=(Operand)malloc(sizeof(struct Operand_));
						o2->kind = SUBS;
						o2->u.subs = AR->u.array.size;
						InterCode mctmp=(InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = MUL;
						mctmp->u.binop.op1 = o1;
						mctmp->u.binop.op2 = o2;
						mctmp->u.binop.result = o1;
						insert_InterCode(mctmp);

						Operand o3=(Operand)malloc(sizeof(struct Operand_));
						o3->kind = TEMP;
						o3->u.tmp_no = t1;
						Operand o4=(Operand)malloc(sizeof(struct Operand_));
						o4->kind = TEMP;
						o4->u.tmp_no = temp_no;
						mctmp=(InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = ADD;
						mctmp->u.binop.op1 = o1;
						mctmp->u.binop.op2 = o3;
						mctmp->u.binop.result = o4;
						insert_InterCode(mctmp);
/*
						if(AR->u.array.elem->kind != ARRAY)
						{
							Operand o6 = (Operand)malloc(sizeof(struct Operand_));

						}
*/


//						printf("%d\n", AR->u.array.size);

					}
					else
					{
//						printf("%d\n", AR->u.array.size);
						Operand o1=(Operand)malloc(sizeof(struct Operand_));
						o1->kind = TEMP;
						o1->u.tmp_no = t2;
						Operand o2=(Operand)malloc(sizeof(struct Operand_));
						o2->kind = SUBS;
						o2->u.subs = AR->u.array.size;
						InterCode mctmp=(InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = MUL;
						mctmp->u.binop.op1 = o1;
						mctmp->u.binop.op2 = o2;
						mctmp->u.binop.result = o1;
						insert_InterCode(mctmp);

						Operand o3=(Operand)malloc(sizeof(struct Operand_));
						o3->kind = TEMP;
						o3->u.tmp_no = t1;
						Operand o4=(Operand)malloc(sizeof(struct Operand_));
						o4->kind = TEMP;
						o4->u.tmp_no = temp_no;
						mctmp=(InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = ADD;
						mctmp->u.binop.op1 = o1;
						mctmp->u.binop.op2 = o3;
						mctmp->u.binop.result = o1;
						insert_InterCode(mctmp);

						Operand o5=(Operand)malloc(sizeof(struct Operand_));
						o5->kind = SUBS;
						o5->u.subs = 4;
						mctmp=(InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = MUL;
						mctmp->u.binop.op1 = o1;
						mctmp->u.binop.op2 = o5;
						mctmp->u.binop.result = o1;
						insert_InterCode(mctmp);

						Operand o6=(Operand)malloc(sizeof(struct Operand_));
						o6->kind = TEMP;
						o6->u.tmp_no = t3;
						mctmp=(InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = ADD;
						mctmp->u.binop.op1 = o1;
						mctmp->u.binop.op2 = o6;
						mctmp->u.binop.result = o1;
						insert_InterCode(mctmp);

						mctmp=(InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = ASSIGNA;
						mctmp->u.assign.left = o4;
						mctmp->u.assign.right = o1;
						insert_InterCode(mctmp);




					}
					
					ret = AR->u.array.elem;
				}
			}

		}
	}
	return ret;
}

void tk_ST(struct Node* nd)
{
	if(nd->ncld == 1)
	{
		tk_CP((nd->cld)[0], 0, NULL);
	}
	else if(nd->ncld == 2)
	{
		tk_Exp((nd->cld)[0], 0, -1, -1, -1);
	}
	else if(nd->ncld == 3)
	{
		int t1 = new_temp();
		Type ret = tk_Exp((nd->cld)[1], t1, -1, -1, -1);
		Operand lonely = (Operand)malloc(sizeof(struct Operand_));
		lonely->kind = TEMP;
		lonely->u.tmp_no = t1;
		InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
		mctmp->kind = RET;
		mctmp->u.lonely = lonely;
		insert_InterCode(mctmp);
		FUCtable temp = Find_by_Name(nfn);
		int ans = 0;
		if(ret != NULL)
			ans = check_type(ret, temp->type);
		if(!ans)
			printf("Error type 8 at Line %d: Type mismatched for return.\n", nd->line);
	}
	else if(nd->ncld == 5)
	{
		if(!NAME((nd->cld)[0]->tag , "IF"))
		{
			int label1 = new_label();
			int label2 = new_label();
			Type ans = tk_Exp((nd->cld)[2], -1, label1, label2, -1);
			Operand lonely = (Operand)malloc(sizeof(struct Operand_));
			lonely->kind = LABEL;
			lonely->u.lab_no = label1;
			InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = LAB;
			mctmp->u.lonely = lonely;
			insert_InterCode(mctmp);
			if(ans == NULL)
			{
				printf("Error type 7 at Line %d: Wrong type in IF.\n", nd->line);
			}
			else{
			if(ans->kind != 0)
			{
				printf("Error type 7 at Line %d: Wrong type in IF.\n", nd->line);
			}
			else
			{
				if(ans->u.basic != 0)
				{
					printf("Error type 7 at Line %d: Wrong type in IF.\n", nd->line);
				}
			}
			}

			tk_ST((nd->cld)[4]);
			lonely = (Operand)malloc(sizeof(struct Operand_));
			lonely->kind = LABEL;
			lonely->u.lab_no = label2;
			mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = LAB;
			mctmp->u.lonely = lonely;
			insert_InterCode(mctmp);
		}
		else
		{
			int label1 = new_label();
			int label2 = new_label();
			int label3 = new_label();
			Operand lonely = (Operand)malloc(sizeof(struct Operand_));
			lonely->kind = LABEL;
			lonely->u.lab_no = label1;
			InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = LAB;
			mctmp->u.lonely = lonely;
			insert_InterCode(mctmp);

			Type ans = tk_Exp((nd->cld)[2], -1, label2, label3, -1);

			lonely = (Operand)malloc(sizeof(struct Operand_));
			lonely->kind = LABEL;
			lonely->u.lab_no = label2;
			mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = LAB;
			mctmp->u.lonely = lonely;
			insert_InterCode(mctmp);

			if(ans == NULL)
			{
				printf("Error type 7 at Line %d: Wrong type in WHILE.\n", nd->line);
			}
			else{
			if(ans->kind != 0)
			{
				printf("Error type 7 at Line %d: Wrong type in WHILE.\n", nd->line);
			}
			else
			{
				if(ans->u.basic != 0)
				{
					printf("Error type 7 at Line %d: Wrong type in WHILE.\n", nd->line);
				}
			}
			}
			tk_ST((nd->cld)[4]);

			lonely = (Operand)malloc(sizeof(struct Operand_));
			lonely->kind = LABEL;
			lonely->u.lab_no = label1;
			mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = GT;
			mctmp->u.lonely = lonely;
			insert_InterCode(mctmp);

			lonely = (Operand)malloc(sizeof(struct Operand_));
			lonely->kind = LABEL;
			lonely->u.lab_no = label3;
			mctmp = (InterCode)malloc(sizeof(struct InterCode_));
			mctmp->kind = LAB;
			mctmp->u.lonely = lonely;
			insert_InterCode(mctmp);

		}
	}
	else
	{
		int label1 = new_label();
		int label2 = new_label();
		int label3 = new_label();

		Type ans = tk_Exp((nd->cld)[2], -1, label1, label2, -1);
		Operand lonely = (Operand)malloc(sizeof(struct Operand_));
		lonely->kind = LABEL;
		lonely->u.lab_no = label1;
		InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
		mctmp->kind = LAB;
		mctmp->u.lonely = lonely;
		insert_InterCode(mctmp);

		if(ans == NULL)
		{
			printf("Error type 7 at Line %d: Wrong type in IF.\n", nd->line);
		}
		else{
		if(ans->kind != 0)
		{
			printf("Error type 7 at Line %d: Wrong type in IF.\n", nd->line);
		}
		else
		{
			if(ans->u.basic != 0)
			{
				printf("Error type 7 at Line %d: Wrong type in IF.\n", nd->line);
			}
		}
		}

		tk_ST((nd->cld)[4]);

		lonely = (Operand)malloc(sizeof(struct Operand_));
		lonely->kind = LABEL;
		lonely->u.lab_no = label3;
		mctmp = (InterCode)malloc(sizeof(struct InterCode_));
		mctmp->kind = GT;
		mctmp->u.lonely = lonely;
		insert_InterCode(mctmp);

		lonely = (Operand)malloc(sizeof(struct Operand_));
		lonely->kind = LABEL;
		lonely->u.lab_no = label2;
		mctmp = (InterCode)malloc(sizeof(struct InterCode_));
		mctmp->kind = LAB;
		mctmp->u.lonely = lonely;
		insert_InterCode(mctmp);


		tk_ST((nd->cld)[6]);

		lonely = (Operand)malloc(sizeof(struct Operand_));
		lonely->kind = LABEL;
		lonely->u.lab_no = label3;
		mctmp = (InterCode)malloc(sizeof(struct InterCode_));
		mctmp->kind = LAB;
		mctmp->u.lonely = lonely;
		insert_InterCode(mctmp);
	}
}

void dfs_StmtList(struct Node* nd)
{
	if(nd != NULL)
	{
		if(nd->type == 0) return;
		if(!NAME(nd->tag, "Stmt"))
			tk_ST(nd);
		else
		{
			for(int i = 0; i < (nd->ncld); i++)
				dfs_StmtList((nd->cld)[i]);
		}
	}
}

void tk_CP(struct Node* nd, int label, char* name)
{
	LVtable newlv = (LVtable)malloc(sizeof(struct LvList_));
	newlv->next = now;
	now = newlv;
	now->Dp = NULL;
	if(label)
	{
		if(name != NULL){
		FUCtable extra = Find_by_Name(name);
		nfn = name;
		FieldList ext = extra -> para;
		if(ext != NULL){
			while(ext->next != NULL)
			{
				DATAtable mid = search_DATAtable(now->next,ext->name, 3);
				if(mid == NULL)
				{
					insert_DATAtable(now, ext->name, ext->type, nd->line, 1, 1);
					InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
					mctmp->kind = PARAM;
					mctmp->u.fname = ext->name;
					insert_InterCode(mctmp);

					/*
					Type midtp = ext->type;
					if(midtp->kind == ARRAY)
					{
						int size = calarray(midtp);
						InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = DEC;
						mctmp->u.dec.name = ext->name;
						mctmp->u.dec.size = size;
						insert_InterCode(mctmp);
					}
					if(midtp->kind == STRUCTURE)
					{
						int size = calstructure(midtp);
						InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
						mctmp->kind = DEC;
						mctmp->u.dec.name = ext->name;
						mctmp->u.dec.size = size;
						insert_InterCode(mctmp);
					}
					*/
				}

				else
					printf("Error type 3 at Line %d: Redefinition of Variable.\n", nd->line);
				ext = ext->next;
			}
		}
		}
	}

	if(label==0 || (label==1 && name!=NULL)){
	FieldList tmp = (FieldList)malloc(sizeof(struct FieldList_));
	tmp->name = NULL;
	dfs_DefList(tmp, (nd->cld)[1], 1);

	dfs_StmtList((nd->cld)[2]);
	}
	
	now = now->next;

}

void tk_ED(struct Node *nd)
{
	Type tmp = (Type)malloc(sizeof(struct Type_));
	char* name = NULL;
	tk_SP(tmp, (nd->cld)[0], &name);
	struct Node* second = (nd->cld)[1];
/*	if(!NAME(second->tag, "SEMI"))
	{
		if(name != NULL)
		{
			insert_DATAtable(now, name, tmp);
		}
	}*/
	if(!NAME(second->tag, "ExtDecList"))
	{
		if(tmp->kind!=2)
			dfs_ExtDecList(second, tmp);
		else
		{
			if(tmp->u.structure != NULL)
				dfs_ExtDecList(second, tmp);
		}
	}
	if(!NAME(second->tag, "FunDec"))
	{
		struct Node* third = (nd->cld)[2];
		if(!NAME(third->tag, "CompSt"))
		{
			char* fname = NULL;
			tk_FD(second, 1, tmp, &fname);
			if(fname != NULL)
			{
				InterCode mctmp = (InterCode)malloc(sizeof(struct InterCode_));
				mctmp->kind = FUC;
				mctmp->u.fname = fname;
				insert_InterCode(mctmp);
			}
			tk_CP(third, 1, fname);
		}
		else
		{
			char* fname = NULL;
			tk_FD(second, 2, tmp, &fname);
//			printf("SEC = %d\n", second->ncld);
		}
	}


/*	if(name!=NULL)
		printf("%s\n",name);
	if(tmp->kind == 2)
	{
		FieldList tap = tmp->u.structure;
		while(tap->next != NULL)
		{
			printf("%s %d\n",tap->name, tap -> type->kind);
			if(tap->type->kind==1)
			{
				printf("tap->type->u.array.size = %d\n", tap->type->u.array.size);
				Type typ = tap->type->u.array.elem;
				printf("typ->u.array.size = %d\n", typ->u.array.size);
				typ = typ->u.array.elem;
                                printf("typ->u.array.size = %d\n", typ->u.array.size);
				printf("typ->kind = %d\n",typ->kind);
                                typ = typ->u.array.elem;
				printf("%d\n",typ->u.basic);
			}
			tap = tap -> next;
		}
	}*/

}

void dfs_mean(struct Node *nd)
{
	if(nd != NULL)
	{
		if(nd->type == 0) return;
		if(!NAME(nd->tag, "ExtDef"))
			tk_ED(nd);
		else
		{
			for(int i = 0; i < (nd->ncld); i++)
				dfs_mean((nd->cld)[i]);
		}
	}
}


void treeAnalyse(struct Node *nd)
{
	now = (LVtable)malloc(sizeof(struct LvList_));
	now->Dp = NULL;
	now->next = NULL;
	head = NULL;
	Type adtp = (Type)malloc(sizeof(struct Type_));
	adtp->kind = BASIC;
	adtp->u.basic = 0;
	FieldList adfl1 = (FieldList)malloc(sizeof(struct FieldList_));
	FieldList adfl2 = (FieldList)malloc(sizeof(struct FieldList_));
	adfl1->name = NULL;
	adfl1->type = adtp;
	adfl1->next = adfl2;
	adfl2->next = NULL;
	adfl2->name = NULL;
	insert_FUCtable("read", 1, adtp, NULL, -1);
	insert_FUCtable("write", 1, adtp, adfl1, -1);
	mc = NULL;
	t_no = 0;
	l_no = 0;
	C0 = (char*)malloc(sizeof(char)*strlen("0"));
	strcpy(C0, "0");
	C1 = (char*)malloc(sizeof(char)*strlen("0"));
	strcpy(C1, "1");
	NEQ = (char*)malloc(sizeof(char)*strlen("!="));
	strcpy(NEQ, "!=");
	nlist = NULL;

	dfs_mean(nd);
//	PrintTable(now);
//	PrintFtable();
	gobackF();
//	printf("TTTT\n");
	if(mid!=NULL)
	generate_mips(mid);
//	printf("%d\n", check_FieldList(now->Dp->type->u.structure, now->Dp->next->type->u.structure));
}



