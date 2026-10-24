/*
G = (V, Sigma, R, Start), em que

V = {S, E, L}
Sigma = {ID, NUM, PRINT, IF, THEN, ELSE, BEGIN, END, ;, =}
R {
    S -> BEGIN S L
    S -> IF E THEN S ELSE S
    S -> PRINT ID
    L -> ; S L
    L -> END
    E -> ID = NUM
}

Start = S
*/

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>

#pragma region GRAMATICA
typedef enum {
    ID,
    NUM,
    PRINT,
    IF,
    THEN,
    ELSE,
    BEGIN,
    END,
    PEV,
    IGUAL,
    FIMDAPILHA,
    ERRO
} Terminal;

typedef struct {
    int i_cadeia; // indice na string do codigo
    Terminal categoria; // categoria do conteudo
    char conteudo[100]; // conteudo
} Token;
#pragma endregion

#pragma region ANALISADOR LEXICO
const char* Terminal2STR(Terminal t) {
    switch (t) {
        case ID: return "ID";
        case NUM: return "NUMERO";
        case PRINT: return "PRINT";
        case IF: return "IF";
        case THEN: return "THEN";
        case ELSE: return "ELSE";
        case BEGIN: return "BEGIN";
        case END: return "END";
        case PEV: return ";";
        case IGUAL: return "=";
        case FIMDAPILHA: return "$";
        default: return "ERRO";
    }
}

Terminal KW_or_ID(char cadeia[]) {
    for (int i = 0; cadeia[i] != '\0'; i++)
        cadeia[i] = toupper(cadeia[i]);

    // versão programador ponytail
    // for (int i = 0; cadeia[i] != '\0'; cadeia[i++] = toupper(cadeia[i]);)

    if (strcmp(cadeia, "IF") == 0) return IF;
    if (strcmp(cadeia, "ELSE") == 0) return ELSE;
    if (strcmp(cadeia, "THEN") == 0) return THEN;
    if (strcmp(cadeia, "BEGIN") == 0) return BEGIN;
    if (strcmp(cadeia, "END") == 0) return END;
    if (strcmp(cadeia, "PRINT") == 0) return PRINT;
    return ID;
}

int get_next_token(char cadeia[], int indice_da_cadeia, Token* token) {
    int i = indice_da_cadeia;

    // Consome porcariadas desnecessarias
    while ((cadeia[i] == ' ') ||  // espaço
           (cadeia[i] == '\n') || // quebra de linha
           (cadeia[i] == '\t') || // tab
           (cadeia[i] == '\r')) {
        i++;
    }

    // Primeiro caracter relevante
    char c = cadeia[i]; // letra, numero ou simbolo (;, =)
    if (c == '\0') { // Acabou o codigo
        token->conteudo[0] = '$';
        token->categoria = FIMDAPILHA;
        token->i_cadeia = i;

        return i;
    }

    // Ainda resta codigo a ser analisado
    //token->conteudo[0] = c;
    // [a-zA-Z_][a-zA-Z_0-9]*
    if (((c >= 'a') && (c <= 'z')) ||
        ((c >= 'A') && (c <= 'Z')) ||
        (c == '_')) { // primeiro caracter alfabetico (_)
        token->i_cadeia = i;

        for (int j = 1; 
            ((cadeia[i] >= 'a') && (cadeia[i] <= 'z')) ||            
            ((cadeia[i] >= 'A') && (cadeia[i] <= 'Z')) ||
            (cadeia[i] >= '_') || 
            ((cadeia[i] >= '0') && (cadeia[i] <= '9'));
            i++) {
            token->conteudo[j++] = cadeia[i];
        }
        token->categoria = KW_or_ID(token->conteudo);
    
        // Numero
        // [0-9]
        for (; 
            ((cadeia[i] >= '0') && (cadeia[i] <= '9'));
            i++) {
            //token->conteudo[j++] = cadeia[i];
        }

        switch (c) {
            case '=': token->categoria = IGUAL;
                token->conteudo[0] = '=';
                token->conteudo[1] = '\0';
        }
    } 
}
#pragma endregion

#pragma region ANALISADOR SINTATICO
void avanca(char cadeia[], int *indice_da_cadeia, Token* token) {
    // Detectar um token
    // Atualizar o indice da cadeia: CONSUMIR O TOKEN
    *indice_da_cadeia = get_next_token(cadeia, *indice_da_cadeia, token);
}

Terminal espia(Token *token) {
    return token->categoria;
}

// expect
bool espera(Terminal categoria_esperada,
            char cadeia[],
            int* indice_da_cadeia,
            Token* token) {
    if(token->categoria == categoria_esperada) {
        avanca(cadeia, indice_da_cadeia, token);
        return true; // deu tudo certo
    }

    printf("ERRO!!!!!\nEsperado %s, porem encontrado %s", 
        Terminal2STR(categoria_esperada),
        Terminal2STR(token->categoria));
    return false;
}
#pragma endregion