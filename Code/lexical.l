%option yylineno

%{
#define YYSTYPE struct Node*
#include "syntax.tab.h"
#include "tree.h"
#include <string.h>
int yycolumn = 1;
//extern int cr;
#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
%}
SEMI ";"
COMMA ","
ASSIGNOP "="
RELOP ">"|"<"|">="|"<="|"=="|"!="
PLUS "+"
MINUS "-"
STAR "*"
DIV "/"
AND "&&"
OR "||"
DOT "."
NOT "!"
TYPE "int"|"float"
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
STRUCT "struct"
RETURN "return"
IF "if"
ELSE "else"
WHILE "while"
INT [1-9]*[1-9][0-9]*|0|0[0-7]+|0[xX][0-9a-fA-F]+
FLOAT [0-9]+\.[0-9]+|([0-9]*\.[0-9]+|[0-9]+\.)[eE][-+]?[0-9]+
ID [_a-zA-Z][_0-9a-zA-Z]*
COMMENTS \/\/[^\n]*|(\/\*([^\*]|(\*)*[^\*\/])*(\*)*\*\/)
SPACES [ ]*
%%
{SEMI} {yylval=cLeaf("SEMI",yytext); return SEMI;}
{COMMA} {yylval=cLeaf("COMMA",yytext); return COMMA;}
{RELOP} {yylval=cLeaf("RELOP",yytext); return RELOP;}
{ASSIGNOP} {yylval=cLeaf("ASSIGNOP",yytext); return ASSIGNOP;}
{PLUS} {yylval=cLeaf("PLUS",yytext); return PLUS;}
{MINUS} {yylval=cLeaf("MINUS",yytext); return MINUS;}
{STAR} {yylval=cLeaf("STAR",yytext); return STAR;}
{DIV} {yylval=cLeaf("DIV",yytext); return DIV;}
{AND} {yylval=cLeaf("AND",yytext); return AND;}
{OR} {yylval=cLeaf("OR",yytext); return OR;}
{DOT} {yylval=cLeaf("DOT",yytext); return DOT;}
{NOT} {yylval=cLeaf("NOT",yytext); return NOT;}
{TYPE} {yylval=cLeaf("TYPE",yytext); return TYPE;}
{LP} {yylval=cLeaf("LP",yytext); return LP;}
{RP} {yylval=cLeaf("RP",yytext); return RP;}
{LB} {yylval=cLeaf("LB",yytext); return LB;}
{RB} {yylval=cLeaf("RB",yytext); return RB;}
{LC} {yylval=cLeaf("LC",yytext); return LC;}
{RC} {yylval=cLeaf("RC",yytext); return RC;}
{STRUCT} {yylval=cLeaf("STRUCT",yytext); return STRUCT;}
{RETURN} {yylval=cLeaf("RETURN",yytext); return RETURN;}
{IF} {yylval=cLeaf("IF",yytext); return IF;}
{ELSE} {yylval=cLeaf("ELSE",yytext); return ELSE;}
{WHILE} {yylval=cLeaf("WHILE",yytext); return WHILE;}
{INT} {yylval=cLeaf("INT",yytext); return INT;}
{ID} {yylval=cLeaf("ID",yytext); return ID;}
{FLOAT} {yylval=cLeaf("FLOAT",yytext); return FLOAT;}
{COMMENTS} {}
{SPACES} {}
\n { yycolumn = 1;}
\t {}
\r { yycolumn = 1;}
. {cr = 0; printf("Error type A at Line %d: Mysterious characters \"%s\".\n",yylineno,yytext);}
