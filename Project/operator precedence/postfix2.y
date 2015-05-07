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
%type <string> Expo
%type <string> RExpr

%token Ident



%%

Prog        :   StmtSeq                         { };
StmtSeq     :   Stmt StmtSeq                    { };
StmtSeq     :                                   { };
Stmt        :   Id '=' RExpr ';'                 {printf("%s = %s\n", $1, $3);};
RExpr       :   Expr '<' Expr                   {asprintf(&$$, "%s %s <", $1, $3);};
RExpr       :   Expr                            {$$ = $1;};
Expr        :   Expr '+' Term                   {asprintf(&$$, "%s %s +", $1, $3);};
Expr        :   Term                            {$$ = $1;};
Term        :   Term '*' Factor                 {asprintf(&$$, "%s %s *", $1, $3);};
Term        :   Factor                          {$$ = $1;};
Factor      :   Expo '^' Factor                 {asprintf(&$$, "%s %s ^", $1, $3);};
Factor      :   Expo                          {$$ = $1;};
Expo        :   '-' Expo                        {asprintf(&$$, "%s !", $2);};
Expo        :   '(' Expr ')'                    {$$ = $2;};
Expo        :   Id                              {$$ = $1;};
Id          :   Ident                           { $$ = strdup(yytext);};

%%

int yyerror(char *s)  {
    printf("Illegal Character in YACC %s", yytext);
    return 1;
}

int main(int argc, char * argv[]) {
    yyparse();
}

