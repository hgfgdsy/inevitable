typedef struct TEMPtable_* TEMPtable;
struct TEMPtable_
{
	int temp_no;
	int offset;
	TEMPtable next;
};

TEMPtable tc;

void insert_TEMPtable(int tno, int ofs)
{
	TEMPtable tmp = (TEMPtable)malloc(sizeof(struct TEMPtable_));
	tmp->temp_no = tno;
	tmp->offset = ofs;
	tmp->next = tc;
	tc = tmp;
}

TEMPtable search_TEMPtable(int tno)
{
	TEMPtable tmp = tc;
	while(tmp!=NULL)
	{
		if(tmp->temp_no == tno)
			return tmp;
		tmp = tmp -> next;
	}
	return NULL;
}


typedef struct VARtable_* VARtable;
struct VARtable_
{
	char* name;
	int offset;
	VARtable next;
};

VARtable vc;

void insert_VARtable(char* name, int ofs)
{
	VARtable tmp = (VARtable)malloc(sizeof(struct VARtable_));
	tmp->name = name;
	tmp->offset = ofs;
	tmp->next = vc;
	vc = tmp;
}

VARtable search_VARtable(char* name)
{
	VARtable tmp = vc;
	while(tmp!=NULL)
	{
		if(!strcmp(tmp->name, name))
			return tmp;
		tmp = tmp -> next;
	}
	return NULL;
}
