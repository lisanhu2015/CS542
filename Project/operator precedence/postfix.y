%{
    #include <stdio.h>
    #include <string.h>
    
    extern int yylex();	/* The next token function. */
    extern char *yytext;   /* The matched token text.  */
    extern int yyleng;
    extern int yyerror(char *);
    
%}
%union {
    char * string;
}

%type <string> Id
%type <string> Expr
%type <string> Term
%type <string> Factor

%token Ident


%%

Prog        :   StmtSeq                         { };
StmtSeq     :   Stmt StmtSeq                    { };
StmtSeq     :                                   { };
Stmt        :   Id '=' Expr ';'                 {printf("%s = %s\n", $1, $3);};
Expr        :   Expr '+' Term                     {asprintf(&$$, "%s %s +", $1, $3);};
Expr        :   Term                            {$$ = $1;};
Term        :   Term '*' Factor                   {asprintf(&$$, "%s %s *", $1, $3);};
Term        :   Factor                          {$$ = $1;};
Factor      :   '-' Factor                      {asprintf(&$$, "%s !", $2);};
Factor      :   '(' Expr ')'                    {$$ = $2;};
Factor      :   Id                              {$$ = $1;};
Id          :   Ident                           { $$ = strdup(yytext);};

%%

int yyerror(char *s)  {
    printf("Illegal Character in YACC");
    return 1;
}

int main(int argc, char * argv[]) {
    yyparse();
}

