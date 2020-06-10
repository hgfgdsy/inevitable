%{
//#include "tree.h"
//#define YYSTYPE struct Node*
#include "lex.yy.c"
int cldN;
struct Node *cldlist[10];
//extern int cr;

int state1 = 0;
void yyerror()
{
//	printf("Error type B at ");
}
%}

/* declared tokens */
/*
%token <type_node> SEMI COMMA
%token <type_node> ASSIGNOP
%token <type_node> RELOP
%token <type_node> PLUS MINUS STAR DIV
%token <type_node> AND OR
%token <type_node> DOT
%token <type_node> NOT
%token <type_node> TYPE
%token <type_node> LP RP LB RB LC RC
%token <type_node> STRUCT
%token <type_node> RETURN
%token <type_node> IF ELSE
%token <type_node> WHILE
%token <type_node> INT
%token <type_node> FLOAT
%token <type_node> ID
%token <type_node> COMMENTS
*/


//%define api.value.type{struct Node*}
%token SEMI COMMA
%token ASSIGNOP
%token RELOP
%token PLUS MINUS STAR DIV
%token AND OR
%token DOT
%token NOT
%token TYPE
%token LP RP LB RB LC RC
%token STRUCT
%token RETURN
%token IF ELSE
%token WHILE
%token INT
%token FLOAT
%token ID
%token COMMENTS

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left DOT
%left LB RB LP RP

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%

Program : ExtDefList {if(cr) {cldN = 1; cldlist[0] = $1; $$ = cNode("Program", cldN, cldlist, @$.first_line); Mpl($$);}}
	;

ExtDefList : ExtDef ExtDefList {if(cr) {cldN=2; cldlist[0]=$1; cldlist[1]=$2; $$=cNode("ExtDefList", cldN, cldlist, @$.first_line);}}
	   | {$$=cEmpty();}
	   ;

ExtDef : Specifier ExtDecList SEMI {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("ExtDef", cldN, cldlist, @$.first_line);}}
       | Specifier SEMI {if(cr) {cldN=2; cldlist[0]=$1; cldlist[1]=$2; $$=cNode("ExtDef", cldN, cldlist, @$.first_line);}}
       | Specifier FunDec CompSt {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("ExtDef", cldN, cldlist, @$.first_line);}}
       | Specifier FunDec SEMI {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("ExtDef", cldN, cldlist, @$.first_line);}}
       | Specifier ExtDecList error SEMI {cr = 0; printf("Error type B at line %d: Syntax error.\n",@$.first_line);}
       | error {cr = 0; printf("Error type B at line %d: Syntax error\n",@$.first_line);}

       ;

ExtDecList : VarDec {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("ExtDecList", cldN, cldlist, @$.first_line);}}
	   | VarDec COMMA ExtDecList {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("ExtDecList", cldN, cldlist, @$.first_line);}}
           ;

Specifier : TYPE {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("Specifier", cldN, cldlist, @$.first_line);}}
	  | StructSpecifier {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("Specifier", cldN, cldlist, @$.first_line);}}
          ;

StructSpecifier : STRUCT OptTag LC DefList RC {if(cr) {cldN=5; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; cldlist[3]=$4; cldlist[4]=$5; $$=cNode("StructSpecifier", cldN, cldlist, @$.first_line);}}
		| STRUCT Tag {if(cr) {cldN=2; cldlist[0]=$1; cldlist[1]=$2; $$=cNode("StructSpecifier", cldN, cldlist, @$.first_line);}}
                ;

OptTag : ID {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("OptTag", cldN, cldlist, @$.first_line);}}
       | {$$=cEmpty();}
       ;

Tag : ID {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("Tag", cldN, cldlist, @$.first_line);}}
    ;


/* Declarators */
VarDec : ID {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("VarDec", cldN, cldlist, @$.first_line);}}
       | VarDec LB INT RB {if(cr) {cldN=4; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; cldlist[3]=$4; $$=cNode("VarDec", cldN, cldlist, @$.first_line);}} 
       ;

FunDec : ID LP VarList RP {if(cr) {cldN=4; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; cldlist[3]=$4; $$=cNode("FunDec", cldN, cldlist, @$.first_line);}}
       | ID LP RP {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("FunDec", cldN, cldlist, @$.first_line);}}
       | error LP RP {cr = 0; printf("Error type B at line %d: Syntax error.\n",@$.first_line);}
       ;

VarList : ParamDec COMMA VarList {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("VarList", cldN, cldlist, @$.first_line);}}
	| ParamDec {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("VarList", cldN, cldlist, @$.first_line);}}
        ;

ParamDec : Specifier VarDec {if(cr) {cldN=2; cldlist[0]=$1; cldlist[1]=$2; $$=cNode("ParamDec", cldN, cldlist, @$.first_line);}}
	 ;

/* Statements */
CompSt : LC DefList StmtList RC {if(cr) {cldN=4; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; cldlist[3] = $4; $$=cNode("CompSt", cldN, cldlist, @$.first_line);}}
       | error RC{cr = 0; printf("Eline %d: Missing \";\".\n",@$.first_line);}
       ;

StmtList : Stmt StmtList {if(cr) {cldN=2; cldlist[0]=$1; cldlist[1]=$2; $$=cNode("StmtList", cldN, cldlist, @$.first_line);}}
	 | {$$=cEmpty();}
         ;

Stmt : Exp SEMI {if(cr) {cldN=2; cldlist[0]=$1; cldlist[1]=$2; $$=cNode("Stmt", cldN, cldlist, @$.first_line);}}
     | CompSt {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("Stmt", cldN, cldlist, @$.first_line);}}
     | RETURN Exp SEMI {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Stmt", cldN, cldlist, @$.first_line);}}
     | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {if(cr) {cldN=5; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; cldlist[3]=$4; cldlist[4]=$5; $$=cNode("Stmt", cldN, cldlist, @$.first_line);}}
     | IF LP Exp RP Stmt ELSE Stmt {if(cr) {cldN=7; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; cldlist[3]=$4; cldlist[4]=$5; cldlist[5]=$6; cldlist[6]=$7; $$=cNode("Stmt", cldN, cldlist, @$.first_line);}}
     | WHILE LP Exp RP Stmt {if(cr) {cldN=5; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; cldlist[3]=$4; cldlist[4]=$5; $$=cNode("Stmt", cldN, cldlist, @$.first_line);}}
     | error SEMI {cr = 0; if(state1!=@$.first_line) printf("Error type B1 at line %d: Syntax error.\n",@$.first_line);}
     ;

/* Local Definitions */
DefList : Def DefList {if(cr) {cldN=2; cldlist[0]=$1; cldlist[1]=$2; $$=cNode("DefList", cldN, cldlist, @$.first_line);}}
	| {$$=cEmpty();}
        ;

Def : Specifier DecList SEMI {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Def", cldN, cldlist, @$.first_line);}}
    | Specifier error SEMI{cr = 0; printf("Error type B at line %d: Syntax error\n",@$.first_line);}
    ;

DecList : Dec {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("DecList", cldN, cldlist, @$.first_line);}}
	| Dec COMMA DecList {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("DecList", cldN, cldlist, @$.first_line);}}
        ;

Dec : VarDec {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("Dec", cldN, cldlist, @$.first_line);}}
    | VarDec ASSIGNOP Exp {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Dec", cldN, cldlist, @$.first_line);}}
    ;

/* Expressions */
Exp : Exp ASSIGNOP Exp {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | Exp AND Exp {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | Exp OR Exp {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | Exp RELOP Exp {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | Exp PLUS Exp {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | Exp MINUS Exp {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | Exp STAR Exp {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | Exp DIV Exp {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | LP Exp RP {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | MINUS Exp {if(cr) {cldN=2; cldlist[0]=$1; cldlist[1]=$2; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | NOT Exp {if(cr) {cldN=2; cldlist[0]=$1; cldlist[1]=$2; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | ID LP Args RP {if(cr) {cldN=4; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; cldlist[3]=$4; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | ID LP RP {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | Exp LB Exp RB {if(cr) {cldN=4; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; cldlist[3]=$4; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | Exp DOT ID {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | ID {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | INT {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | FLOAT {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("Exp", cldN, cldlist, @$.first_line);}}
    | LP error RP {cr = 0; printf("Error Type B at line %d: Invalid expression.\n", @$.first_line);}
    | Exp LB Exp error {cr = 0; state1 = @$.first_line;printf("Error Type B at line %d: Missing \"]\".\n",@$.first_line);}
    | Exp RELOP error {cr = 0; printf("Error type B at line %d: Syntax error\n",@$.first_line);}

    ;

Args : Exp COMMA Args {if(cr) {cldN=3; cldlist[0]=$1; cldlist[1]=$2; cldlist[2]=$3; $$=cNode("Args", cldN, cldlist, @$.first_line);}}
     | Exp {if(cr) {cldN=1; cldlist[0]=$1; $$=cNode("Args", cldN, cldlist, @$.first_line);}}
     ;


%%







