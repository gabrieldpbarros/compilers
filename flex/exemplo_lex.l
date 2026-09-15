%option noyywrap

%{
    #include <stdio.h>
    int qtd_digitos = 0;
%}

%%
[0-9] {
    printf("Encontramos um digito (%s)!\n", yytext);
    qtd_digitos = qtd_digitos + 1;
}
%%

int main() {
    yy_scan_string("12343317891");
    yylex();
    printf("Quantidade de digitos encontrada: %d", qtd_digitos);
    return 0;
}