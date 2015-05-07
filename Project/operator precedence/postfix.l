%{
    #include "y.tab.h"
%}

letter [A-Za-z]
digit [0-9]

%%

{letter}({letter}|{digit})*     {return Ident;}
\+                           {return '+';}
\-                            {return '-';}
\=                              {return '=';}
\*                              {return '*';}
\;                              {return ';';}
\(                              {return '(';}
\)                              {return ')';}

[ ]                             {}
\t                              {}
\r                              {}
\n                              {}

.                               { printf("Illegal Character\n");}

%%


int yywrap () {
    return 1;
}
