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

%token Ident 		
%token IntLit 	
%token Int
%token Write
%token IF
%token EQ
%token LT
%token LTEQ
%token GT
%token GTEQ
%token NEQ

%%

Prog			:	Declarations StmtSeq						{Finish($2); } ;
Declarations	:	Dec Declarations							{ };
Declarations	:											{ };
Dec			:	Int Ident {EnterName(table, yytext, &entry); }';'	{};
StmtSeq 		:	Stmt StmtSeq								{$$ = AppendSeq($1, $2); } ;
StmtSeq		:											{$$ = NULL;} ;
Stmt			:	Write Expr ';'								{$$ = doPrint($2); };
Stmt			:	Id '=' Expr ';'								{$$ = doAssign($1, $3);} ;
Stmt			:	IF '(' BExpr ')' '{' StmtSeq '}'					{$$ = doIf($3, $6);};
BExpr		:	Expr EQ Expr								{$$ = doEQExpr($1, $3);};
BExpr		:	Expr NEQ Expr								{$$ = doNEQExpr($1, $3);}; !!!!!
BExpr		:	CompExpr EQ CompExpr								{$$ = doBExpr($1, $3);}; !!!!!
BExpr		:	CompExpr NEQ CompExpr								{$$ = doBExpr($1, $3);}; !!!!!
CompExpr	:	Expr GT Expr								{$$ = doBExpr($1, $3);}; !!!!!
CompExpr	:	Expr GTEQ Expr								{$$ = doBExpr($1, $3);}; !!!!!
CompExpr	:	Expr LT Expr								{$$ = doBExpr($1, $3);}; !!!!!
CompExpr	:	Expr LTEQ Expr								{$$ = doBExpr($1, $3);}; !!!!!
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
