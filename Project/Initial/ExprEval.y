%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yaccExample.h"
#include "Semantics.h"
#include "CodeGen.h"

extern int yylex();	/* The next token function. */
extern char *yytext;   /* The matched token text.  */
extern int yyleng;      /* The token text length.   */
extern int yyparse();
void dumpTable();

extern struct SymTab *table;
extern struct SymEntry *entry;

%}


%union {
  long val;
  char * string;
  struct ExprRes * ExprRes;
  struct InstrSeq * InstrSeq;
  struct BExprRes * BExprRes;
}

%type <string> Id
%type <ExprRes> Factor
%type <ExprRes> Term
%type <ExprRes> Expr
%type <InstrSeq> StmtSeq
%type <InstrSeq> Stmt
%type <BExprRes> BExpr
%type <string> Str

%token Ident 		
%token IntLit 	
%token Int
%token Write
%token WriteLn
%token WriteSp
%token WriteStr
%token IF
%token EQ
%token StrLit

%%

Prog			:	Declarations StmtSeq						{Finish($2); } ;
Declarations	:	Dec Declarations							{ };
Declarations	:											{ };
Dec			:	Int Ident {EnterName(table, yytext, &entry); }';'	{};
StmtSeq 		:	Stmt StmtSeq								{$$ = AppendSeq($1, $2); } ;
StmtSeq		:											{$$ = NULL;} ;
Stmt			:	Write Expr ';'								{$$ = doPrint($2); };
Stmt			:	WriteStr Str ';'								{$$ = doPrintStr($2); };
Str				:	StrLit								{ $$ = doStrLit(yytext); };
Stmt			:	WriteSp Expr ';'								{$$ = doPrintSp($2); };
Stmt			:	WriteLn ';'								{$$ = doPrintLn(); };
Stmt			:	Id '=' Expr ';'								{$$ = doAssign($1, $3);} ;
Stmt			:	IF '(' BExpr ')' '{' StmtSeq '}'					{$$ = doIf($3, $6);};
BExpr		:	Expr EQ Expr								{$$ = doBExpr($1, $3);};
Expr			:	Expr '+' Term								{$$ = doAdd($1, $3); } ;
Expr			:	Expr '-' Term								{$$ = doSub($1, $3); } ;
Expr			:	Term									{$$ = $1; } ;
Term		:	Term '*' Factor								{ $$ = doMult($1, $3); } ;
Term		:	Term '/' Factor								{ $$ = doDiv($1, $3); } ;
Term		:	Term '%' Factor								{ $$ = doMod($1, $3); } ;
Term		:	Factor									{ $$ = $1; } ;
Factor		:	'(' Expr ')'							{ $$ = $2; } ;
Factor		:	'-' Factor								{ $$ = doNeg($2); } ;
Factor		:	IntLit									{ $$ = doIntLit(yytext); };
Factor		:	Ident									{ $$ = doRval(yytext); };
Id			: 	Ident									{ $$ = strdup(yytext);}
 
%%

yyerror(char *s)  {
  WriteIndicator(GetCurrentColumn());
  WriteMessage("Illegal Character in YACC");
}
