#include "syntax.tab.h"
#include "tree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


struct Node *cLeaf(char *tag, char *text)
{
	struct Node *nd = (struct Node*)malloc(sizeof(struct Node));
	nd -> ncld = 0;
	nd -> tag = (char*)malloc(sizeof(char)*strlen(tag));
	nd -> tag = tag;
	nd -> value = NULL;
	nd -> type = 1;
	nd -> line = 0;
	if((strcmp(tag, "ID") == 0) || (strcmp(tag, "TYPE") == 0))
	{
		nd -> value = (char*)malloc(sizeof(char)*strlen(text));
		strcpy(nd->value, text);
//		printf("%s : %s\n", nd->tag, nd->value);
	}
	else if(strcmp(tag, "RELOP") == 0)
	{
		nd->value = (char*)malloc(sizeof(char)*strlen(text));
		strcpy(nd->value, text);
	}
	else if(strcmp(tag, "INT") == 0)
	{
		if(*(text) != '0' || strlen(text) == 1)
		{
			nd -> value = (char*)malloc(sizeof(char)*strlen(text));
			strcpy(nd->value, text);
		}
		else
		{
			int val = 0;
			if(*(text+1) == 'x' || *(text+1) == 'X')
			{
				int len = strlen(text);
				for(int i = 2; i < len; i++)
				{
//					printf("%d\n",*(text+i)-'0');
					if('a' <= *(text+i) && *(text+i) <= 'f')
					{
//						printf("ddddd1\n");
						val=16*val;
						val=val+((*(text+i)-'a')+10);
					}
					if('A' <= *(text+i) && *(text+i) <= 'F')
					{
//						printf("ddddd2\n");
						val=16*val;
						val=val + (*(text+i)-'A')+10;
					}
					if('0' <= *(text+i) && *(text+i) <= '9')
					{
//						printf("ddddd3\n");
						val=16*val;
						val=val+(*(text+i)-'0');
					}
//					printf("%d\n",val);
				}
//				printf("%d  %s\n",len,text);
			}
			else
			{
				int len = strlen(text);
				for(int i = 1; i < len; i++)
				{
					val*=8;
					val+=(*(text+i))-'0';
				}
			}
			int ll = 0;
			char tmp[32];
			while(val)
			{
				tmp[ll++] = (val%10) + '0';
				val/=10;
			}
			nd -> value = (char*)malloc(sizeof(char)*ll);
			for(int i = 0; i < ll; i++)
			{
				*(nd -> value + i) = tmp[ll - i -1];
			}
		}
//		printf("INT : %s\n",nd->value);
	}
	else if(strcmp(tag, "FLOAT") == 0)
	{
		int len = strlen(text);
		int ttt = 0;
		for(int i = 0; i < len; i++)
		{
			if(*(text+i) == 'e' || *(text+i) == 'E')
			{
				ttt = i;
				break;
			}
		}
		if(ttt == 0)
		{
			int len = strlen(text);
			int pos = 0;
			int sp = 0;
			char ft[50];
			for(int i=0; i<len; i++)
			{
				if(*(text+i)=='.')
				{
					pos = i;
					break;
				}
				ft[sp++] = *(text+i);

			}
			ft[sp++] = '.';
			if(len - pos - 1 >= 6)
			{
				for(int i = pos+1; i <= pos + 6; i++)
					ft[sp++] = *(text+i);

			}
			else{
				for(int i = 0; i< 6; i++)
					ft[sp++] = '0';
				sp-=6;
				for(int i = pos + 1; i < len ;i++)
					ft[sp++] = *(text+i);
				sp+=(6-(len-pos-1));
			}
			ft[sp++] = '\0';
			nd -> value = (char*)malloc(sizeof(char)*sp);
			strcpy(nd->value,ft);
		}
		else
		{
			int label = 0;
			if(*(text+ttt+1) == '-') label = -1;
			if(*(text+ttt+1) == '+') label = 1;
			int eval = 0;
//			int val = 0;
			if(label == 0)
			{
				for(int i = ttt+1; i < len; i++)
				{
					eval*=10;
					eval += *(text+i)-'0';
				}
				
			}
			if(label == 1 || label == -1)
			{
				for(int i = ttt+2; i < len; i++)
				{
					eval*=10;
					eval += *(text+i)-'0';
				}
			}
			char st[50];
			int lp = 0;
			if(label != -1)
			{
				int pos = 0;
//				int lp = 0;
				for(int i = 0; i < ttt; i++)
				{
					if(*(text+i) == '.')
					{
						pos = i;
						break;
					}
					else
					{
						st[lp++] = *(text+i);
					}
				}
				if(ttt - pos - 1 < eval)
				{
					for(int i = pos + 1; i < ttt; i++)
					{
						st[lp++] = *(text+i);
					}
					int cha = eval - (ttt - pos -1);
					for(int i = 0; i < cha; i++)
					{
						st[lp++] = '0';
					}
					st[lp++] = '.';
					for(int i = 0; i < 6; i++)
					{
						st[lp++] = '0';
					}
//					printf("ddd\n");
				}
				else
				{
					int cha = (ttt - pos -1) - eval;
					for(int i = pos + 1; i < pos + eval + 1; i++)
					{
						st[lp++] = *(text+i);
					}
					st[lp++] = '.';
					int min = 6;
					if(cha < 6) min = cha;
					for(int i = pos + eval + 1; i < min + pos + eval + 1; i++)
					{
						st[lp++] = *(text+i);
					}
					for(int i = 0; i < 6-min; i++)
					{
						st[lp++] = '0';
					}
				}
			}
			else
			{
				int pos = 0;
//				int lp = 0;
				for(int i =0 ; i < ttt; i++)
				{
					if(*(text+i)=='.')
					{
						pos = i;
						break;
					}
				}
				if(pos > eval)
				{
					int mid = pos - eval;
					for(int i = 0; i < mid; i++)
					{
						st[lp++] = *(text+i);
					}
					st[lp++] = '.';
					if(ttt - mid <= 6)
					{
						for(int i = mid; i < pos; i++)
						{
							st[lp++] = *(text+i);
						}
						for(int i = pos+1; i <ttt; i++)
						{
							st[lp++] = *(text+i);

						}
						for(int i = 0; i < 6 - (ttt - mid); i++)
						{
							st[lp++] = '0';
						}
					}
					else
					{
						int cnt = 0;
						for(int i = mid; i < ttt && cnt<6; i++)
						{
							if(i == pos) continue;
							st[lp++] = *(text+i);
							cnt++;
						}
					}
				}
				else
				{
					st[lp++] = '0';
					st[lp++] = '.';
					int diff = eval - pos;
					if(diff >= 6)
					{
						for(int i=0; i<6; i++)
						{
							st[lp++] = '0';
						}
					}
					else{
						int cnt=0;
						for(int i=0; i<diff ; i++)
						{
							st[lp++] = '0';
						}
						for(int i=0; cnt<6-diff && i < ttt; i++)
						{
							if(i==pos) continue;
							st[lp++] = *(text+i);
							cnt++;
						}
						while(cnt+diff<6)
						{
							st[lp++] = '0';
							cnt++;
						}

					}
				}
			}
			st[lp++] = '\0';
			nd->value = (char*)malloc(sizeof(char)*lp);
			strcpy(nd->value, st);
		}
//		printf("FLOAT : %s\n", nd->value);
	}
	else
	{
		nd->value = NULL;
//		printf("OTHER : %s\n", nd->tag);
	}

	return nd;
}

struct Node *cNode(char *tag, int ncld, struct Node *a[], int line)
{
	struct Node *nd = (struct Node*)malloc(sizeof(struct Node));
	nd -> ncld = ncld;
	nd->tag = (char*)malloc(sizeof(char)*strlen(tag));
	nd -> tag = tag;
	nd -> value = NULL;
	for(int i=0; i<nd->ncld; i++)
	{
		(nd->cld)[i] = a[i];
	}
	nd -> line = line;
	nd -> type = 2;
	return nd;
}

struct Node *cEmpty()
{
	struct Node *nd = (struct Node*)malloc(sizeof(struct Node));
	nd -> ncld = 0;
	nd -> tag = NULL;
	nd -> value = NULL;
	nd->type = 0;

	return nd;
}

void treePrintL(struct Node *nd, int lv)
{
	if(nd!=NULL)
	{
		if(nd->type == 0) return;
		for(int i=0; i<2*lv; i++)
			printf(" ");
		if(nd->type == 2)
			printf("%s (%d)\n",nd->tag,nd->line);
		else
		{
			if(nd->value==NULL)
			{
				printf("%s\n",nd->tag);
			}
			else
			{
				printf("%s: %s\n",nd->tag,nd->value);
			}
		}
		for(int i = 0; i<(nd->ncld); i++)
		{
			treePrintL((nd->cld)[i], lv+1);
		}
	}
}
			
				
void Tpl(struct Node *nd)
{
	treePrintL(nd, 0);
}


void Mpl(struct Node *nd)
{
	treeAnalyse(nd);
}







