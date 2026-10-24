%option noyywrap

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #define YY_DECL int yylex()

    #include "exp1.tab.h"
%}

%%
[ \t] ; // ignorar espaços e tabulações
[0-9]+\.[0-9]+ {
    yylval.fval = atof(yytext);
    return T_FLOAT; // dentro de exp1.tab.h
}
[0-9]+ {
    yylval.ival = atoi(yytext);
    return T_INT;
}
"(" { return T_LPAR; }
")" { return T_RPAR; }
"+" { return T_LMAIS; }
"-" { return T_MENOS; }
"\n" { return T_NEWLINE; }
%%