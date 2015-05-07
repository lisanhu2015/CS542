%{
#include "yaccExample.h"
#include "y.tab.h"

#define YY_INPUT(buf,result,max_size) \
	{ int c = GetSourceChar(); \
          result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
        }

%}


letter [A-Za-z]
digit [0-9]

%%
if					{return IF;}
int					{return Int;}
println				{return WriteLn;}
printsp				{return WriteSp;}
print					{return Write;}
printstr			{return WriteStr;}
{letter}({letter}|{digit})*	{return Ident;}
{digit}{digit}*		{return IntLit;}
\=\=					{return EQ;}
\=					{return '=';}
\+					{return '+';}
\-					{return '-';}
\*					{return '*';}
\/					{return '/';}
\%					{return '%';}
\;					{return ';';}
\{					{return '{';}
\}					{return '}';}
\(					{return '(';}
\)					{return ')';}
\".*\"				{return StrLit;}

[ ]				{}
\t				{}
\r				{}
\n				{}

.				{WriteIndicator(GetCurrentColumn());
				   WriteMessage("Illegal Character in lex");}

%%

	
yywrap () {
}
			
	