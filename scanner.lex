%{

/* Declarations section */
#include "semanticAnalyzer.h"
#include "hw3_output.hpp"
#include "parser.tab.hpp"
#include <stdio.h>
#include <iostream>
%}
%option nounput
%option yylineno
%option noyywrap
%x REALLYEND
whitespace		([\t\n\r ])


%%
{whitespace}								;
\/\/[^\r\n]*[\r|\n|\r\n]?					;
void										return VOID;
int 									    return INT;
byte										return BYTE;
b											return B;
bool										return BOOL;
and											return AND;	
or											return OR;
not											return NOT;
true										return TRUE;
false										return FALSE;
return 										return RETURN;
if 											return IF;
else										return ELSE;
while										return WHILE;
break										return BREAK;
continue									return CONTINUE;
switch 										return SWITCH;
case										return CASE;
default										return DEFAULT;
\:											return COLON;
\;											return SC;
\,											return COMMA;
\(											return LPAREN;
\)											return RPAREN;
\{											return LBRACE;
\}											return RBRACE;
\=											return ASSIGN;
==|!=										yylval.stringVal = yytext; return EQUALITY;
>|<|<=|>= 									yylval.stringVal = yytext; return RELATIONAL;
\+											return ADD;
\-											return SUB;
\*											return MULT;
\/											return DIV;
[a-zA-Z][a-zA-Z0-9]* 						yylval.stringVal = yytext; return ID;
0|[1-9][0-9]*								yylval.intVal = atoi(yytext); return NUM;
\"([^\n\r\"\\]|\\[rnt"\\])+\"	 			yylval.stringVal = yytext; return STRING;
.											output::errorLex(yylineno); exit(1);
<INITIAL><<EOF>>        { BEGIN(REALLYEND); return EOP; }
<REALLYEND><<EOF>>      { return 0; }
%%

