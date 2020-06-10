#include<stdio.h>
FILE* mid;

struct Node
{
	char* tag;
	char* value;
	struct Node *cld[10];
	int ncld;
	int line;
	int type;
};

int cr;

struct Node *cLeaf(char *tag, char *text);
struct Node *cNode(char *tag, int ncld, struct Node *a[], int line);
struct Node *cEmpty();
void Tpl(struct Node *nd);
void Mpl(struct Node *nd);
void treeAnalyse(struct Node *nd);
