%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyparse();

extern *FILE yyin;

void yyerror(const char* s);
%}

%union{
    int ival;
    float fval;
}

%token<fval> T_INT
%token<fval> T_FLOAT

%token T_MAIS T_MENOS T_LPAR T_RPAR
%token T_NEWLINE

%left T_MAIS T_MENOS

%type<fval> exp
%type<fval> fator

%start calculo

%%
calculo: // vazio (epsilon)
        | calculo linha // corpo da regra
        ; // finalização da regra

linha:  exp {
            printf("Resultado: %f\n", $1);
        }
        | exp T_NEWLINE {
            printf("Resultado: %f\n", $1);
        }
        | T_NEWLINE // consumo de linhas vazias
        ;

exp:    exp T_MAIS fator { $$ = $1 + $3; } // $$: exp, $1: exp, $2: T_MAIS. $3: fator
        | exp T_MENOS fator { $$ = $1 - $3; }
        | fator { $$ = $1; }
        ;

fator:  T_LPAR exp T_RPAR { $$ = $2; }
        | T_INT { $$ = $1; }
        | T_FLOAT { $$ = $1; }
        ;
%%

int main(int argc, char** argv) {
    if(argc > 1) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            printf("Problema na leitura do arquivo.");
            return -1;
        } else {
            yyin = fopen("exp1.txt", "r");
            if (yyin == NULL) {
                printf("Problema na leitura do arquivo.");
                return -1;
            }
        }

        do { // estrutura em loop para a análise sintática 
            yyparse();
        } while(!feof(yyin));

        fclose(yyin);
        return 0;
    }
}

void yyerror(const char* s) {
    fprintf(stderr, "Parse error, %s\n", s);
    exit(1);
}