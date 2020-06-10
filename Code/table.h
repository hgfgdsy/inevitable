#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;

struct Type_
{
	enum {BASIC, ARRAY, STRUCTURE} kind;
	union
	{
		int basic;
		struct {Type elem; int size;} array;
		FieldList structure;
	} u;
	int x;
};

struct FieldList_
{
	char *name;
	Type type;
	FieldList next;
};

typedef struct DATAtable_* DATAtable;

struct DATAtable_
{
	char* name;
	Type type;
	int state;
	int inout;
	int offset;
	DATAtable next;
};

typedef struct FUCtable_* FUCtable;
struct FUCtable_
{
	char* name;
	Type type;
	FieldList para;
	int state;
	int line;
	FUCtable next;
};

typedef struct LvList_* LVtable;
struct LvList_
{
	DATAtable Dp;
	LVtable next;
};

typedef struct arglist_* arglist;
struct arglist_
{
	Type tar;
	int tag;
	arglist next;
	arglist prev;
};

typedef struct bsize_* bsize;
struct bsize_
{
	int size;
	bsize next;
};

void dfs_mean(struct Node* nd);
void tk_ED(struct Node* nd);
void tk_SP(Type record, struct Node* nd, char* *name);
void dfs_DefList(FieldList record, struct Node* nd, int label);
void tk_DE(FieldList record, struct Node* nd, int label);
void dfs_DecList(FieldList record, struct Node* nd, int label, Type tp);
int check_FieldList(FieldList a, FieldList b);
void tk_CP(struct Node* nd, int label, char* name);
Type tk_Exp(struct Node* nd, int temp_no, int l1, int l2, int VOA);
DATAtable search_DATAtable(LVtable now, char* name, int state);

int NAME(char* a, char* b)
{
	return strcmp(a,b);
}

void PrintTable(LVtable now)
{
	while(now!=NULL)
	{
		DATAtable Dtmp = now -> Dp;
		while(Dtmp!=NULL)
		{
			Type Ttmp = Dtmp -> type;
			printf("name = %s\n", Dtmp->name);
			if(Ttmp->kind == 0)
			{
				printf("basic = %d\n", Ttmp->u.basic);
			}
			else if (Ttmp->kind == 1)
			{
				printf("size 1 = %d\n", Ttmp->u.array.size);
			}
			else
			{
				printf("yu1 = %s\n", Ttmp->u.structure->name);
			}
			Dtmp = Dtmp->next;
		}
		now = now->next;
	}
}


int insert_DATAtable(LVtable now, char* name, Type tp, int line, int state, int inout)
{
	if(state == 2)
	{
		DATAtable exist = search_DATAtable(now, name, 3);
		if(exist!=NULL)
		{
			printf("Error type 16 at Line %d: Redefinition of struct.\n", line);
			return 0;
		}
	}
	if(state == 1)
	{
		DATAtable exist = search_DATAtable(now, name, 2);
		if(exist!=NULL)
		{
			printf("Error type 3 at Line %d: Redefinition of Variable.\n", line);
			return 0;
		}

	}
	DATAtable tmp = now->Dp;
	DATAtable tmp1 = (DATAtable)malloc(sizeof(struct DATAtable_));
	if(tmp == NULL)
	{
		tmp1->name = name;
		tmp1->type = tp;
		tmp1->state = state;
		tmp1->inout = inout;
		tmp1->offset = 0;
		tmp1->next = NULL;
		now->Dp = tmp1;
	}
	else
	{
		int label = 1;
		while(tmp != NULL)
		{
			if(!NAME(name, tmp->name))
			{
				label = 0;
				if(tp->kind == 1 || tp->kind == 0)
					printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line,name);
				else
					printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", line, name);
				break;
			}
			tmp = tmp->next;
		}
		if(label)
		{
			
			tmp = now->Dp;
			tmp1->name = name;
			tmp1->type = tp;
			tmp1->state = state;
			tmp1->inout = inout;
			tmp1->offset = 0;
			tmp1->next = tmp;
			now->Dp = tmp1;
		}
	}
	return 1;			
}

DATAtable search_DATAtable(LVtable now, char* name, int state)
{
	DATAtable ret = NULL;
	while(now!=NULL){
		DATAtable tmp = now->Dp;
		if(tmp != NULL)
		{
			while(tmp!=NULL)
			{
				if(!NAME(name, tmp->name) && (tmp->state == state || state == 3))
				{
					ret = tmp;
					break;
				}
				tmp=tmp->next;
			}

		}
		if(ret!=NULL) break;
		now = now->next;
	}
	return ret;
}

FUCtable head;

FUCtable search_FUCtable(char* name)
{
	FUCtable ret = NULL;
	FUCtable tmp = head;
	while(tmp != NULL)
	{
		if(!NAME(tmp->name, name))
		{
			ret = tmp;
			break;
		}
		tmp = tmp->next;
	}
	return ret;
}

void insert_FUCtable(char* name, int oval, Type tp, FieldList P, int line)
{
	if(head == NULL)
	{
		head = (FUCtable)malloc(sizeof(struct FUCtable_));
		head -> name = name;
		head -> state = oval;
		head -> type = tp;
		head -> para = P;
		head -> line = line;
		head->next = NULL;
	}
	else
	{
		FUCtable tmp = (FUCtable)malloc(sizeof(struct FUCtable_));
		tmp -> name = name;
		tmp -> state = oval;
		tmp -> type = tp;
		tmp -> para = P;
		tmp -> line = line;
		tmp -> next = NULL;

		FUCtable TH = head;
		while(TH->next != NULL)
		{
			TH = TH->next;
		}
		TH->next = tmp;
	}
}	


int check_array(Type at, Type bt)
{
	while(at->kind == 1 && bt->kind == 1)
	{
		at=at->u.array.elem;
		bt=bt->u.array.elem;
	}
	if(at->kind != bt->kind) return 0;
	if(at->kind == 0)
		if(at->u.basic != bt->u.basic) return 0;
	int ret = 1;
	if(at->kind == 2)
		ret = check_FieldList(at->u.structure, bt->u.structure);
	return ret;
}

int check_FieldList(FieldList a, FieldList b)
{
	if(a == NULL || b == NULL) return 0;
	int ret = 1;
	while(a->next != NULL && b->next != NULL)
	{
		if(a->type->kind != b->type->kind) return 0;
		if(a->type->kind == 0)
		{
			if(a->type->u.basic != b->type->u.basic)
				return 0;
		}
		else if(a->type->kind == 1)
		{
			Type at = a->type;
			Type bt = b->type;
			ret = check_array(at, bt);
			if(!ret) break;
		}
		else
		{
			ret = check_FieldList(a->type->u.structure, b->type->u.structure);
			if(!ret) break;
		}
		a = a->next;
		b = b->next;

	}
	if((a->next!=NULL || b->next!=NULL) && ret == 1) return 0;
	return ret;
}	


void PrintFtable()
{
	while(head!=NULL)
	{
		printf("%d %s\n",head->state, head->name);
		head= head->next;
	}
}

FUCtable Find_Last()
{
	FUCtable ret = head;
	while(ret->next != NULL)
	{
		ret = ret->next;
	}
	return ret;
}


int check_type(Type a, Type b)
{
	if(a==NULL || b==NULL) return 0;
	if(a->kind != b->kind) return 0;
	if(a->kind == 0)
		if(a->u.basic != b->u.basic)
			return 0;
	int ret = 1;
	if(a->kind == 1)
		ret = check_array(a, b);
	if(a->kind == 2)
		ret = check_FieldList(a->u.structure, b->u.structure);
	return ret;
}


void gobackF()
{
	FUCtable tmp = head;
	while(tmp != NULL)
	{
		if(tmp->state == 2)
			printf("Error type 18 at Line %d: Undefined function \"%s\".\n", tmp->line, tmp->name);
		tmp = tmp->next;
	}
}


FUCtable Find_by_Name(char* name)
{
	FUCtable ret = head;
	while(ret->next != NULL)
	{
		if(!NAME(ret->name, name))
			break;
		ret = ret->next;
	}
	return ret;
}

int calstructure(Type tp);

int calarray(Type tp)
{
	int ret = tp->u.array.size;
	Type tmp = tp;
	while(tmp->u.array.elem->kind == ARRAY)
	{
		tmp = tmp->u.array.elem;
		ret = ret*(tmp->u.array.size);
	}
	tmp = tmp->u.array.elem;
	if(tmp->kind == BASIC)
	{
		if(tmp->u.basic == 0)
		{
			ret = ret * 4;
		}
	}
	else
	{
		ret = ret * calstructure(tmp);
	}
	return ret;
}


int calstructure(Type tp)
{
	FieldList tmp = tp -> u.structure;
	int ret = 0;
	while(tmp -> next != NULL)
	{
		Type mtp = tmp->type;
		if(mtp->kind == BASIC)
		{
			ret = ret + 4;
		}
		if(mtp->kind == ARRAY)
		{
			ret = ret + calarray(mtp);
		}
		if(mtp->kind == 2 && mtp->u.structure != NULL)
		{
			ret = ret + calstructure(mtp);
		}
		tmp = tmp->next;
	}
	return ret;
}

int calsize(Type tp)
{
	int ret = 0;
	if(tp->kind == BASIC)
		ret=4;
	if(tp->kind == ARRAY)
		ret = calarray(tp);
	if(tp->kind == 2 && tp->u.structure != NULL)
		ret = calstructure(tp);
	return ret;
}
