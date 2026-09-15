%option noyywrap

%{
    #include <stdio.h>
    int qtd_digitos = 0;
    int qtd_maiusculas = 0;
    int qtd_minusculas = 0;
%}

%%
[0-9] {
    printf("Encontramos um digito (%s)!\n", yytext);
    qtd_digitos = qtd_digitos + 1;
}

[A-Z] {
    printf("Encontramos uma letra maiuscula (%s)!\n, yytext);
    qtd_maiusculas += 1;
}

[a-Z] {
    printf("Encontramos uma letra minuscula (%s)!\n, yytext);
    qtd_minusculas++;
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

    printf("\nQuantidade de digitos encontrada: %d", qtd_digitos);
    printf("\nQuantidade de letras maiusculas encontrada: %d", qtd_maiusculas);
    printf("\nQuantidade de letras minusculas encontrada: %d", qtd_minusculas);

    fclose(yyin);

    return 0;
}