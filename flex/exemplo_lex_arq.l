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

int main(int argc, char *argv[]) {
    FILE *arch;

    if (argc > 1)
        arch = fopen(argv[1], "r");
    else 
        arch = fopen("text/exemplo_texto.txt", "r");

    if (!arch) {
        perror("Arquivo nao encontrado");
        return 1;
    }

    yyin = arch; 
    yylex();
    printf("Quantidade de digitos encontrada: %d\n", qtd_digitos);

    fclose(yyin);

    return 0;
}
