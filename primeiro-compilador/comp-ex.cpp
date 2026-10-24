#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <cstdio>
#include <cstring>
#include <string.h>
#include <cctype>

#pragma region GRAMATICA
/*
* S → BEGIN S L
S → IF E THEN S ELSE S
S → PRINT ID
L → ; S L
L → END
E → ID = NUM
V = {S, L, E}
∑ = { ID, NUM, IF, THEN, ELSE, BEGIN, END, PRINT, ; , =}
Start = {S}
*/
#pragma endregion
#pragma region ANALISADOR LEXICO
typedef enum {
    ID, NUM, IF, THEN, ELSE, BEGIN, END, PRINT, PEV, IGUAL,
    FIMDAPILHA, ERRO
} Terminal;

typedef struct {
    int indice_cadeia;
    Terminal token;
    char conteudo[250]; // Caracteres máximos de um token 
    // (vale a pena limitar na análise)
} Lexema;

Terminal KW_ou_ID(char conteudo[]) {
    // converter para maiúsculo...
    for (int i = 0; conteudo[i] != '\0'; conteudo[i++] = toupper
    (conteudo[i]));
    if (strcmp(conteudo, "IF") == 0) return IF;
    if (strcmp(conteudo, "THEN") == 0) return THEN;
    if (strcmp(conteudo, "ELSE") == 0) return ELSE;
    if (strcmp(conteudo, "BEGIN") == 0) return BEGIN;
    if (strcmp(conteudo, "END") == 0) return END;
    if (strcmp(conteudo, "PRINT") == 0) return PRINT;
    
    return ID;
}

const char* Terminal2Str(Terminal t) {
    switch (t) {
        case NUM: return "NUMERO";
        case IF: return "IF";
        case ID: return "ID";
        case THEN: return "THEN";
        case ELSE: return "ELSE";
        case BEGIN: return "BEGIN";
        case END: return "END";
        case PRINT: return "PRINT";
        case PEV: return "PEV";
        case IGUAL: return "IGUAL";
        case FIMDAPILHA: return "$";
        case ERRO: return "ERRO";
        default: return "TOKEN INVALIDO";
    }
}

// Autômato para detectar tokens (IDs/KWs, Numeros, sim. especiais)
int get_next_token(char cadeia[], int indice_cadeia, Lexema* lex) {
    int i = indice_cadeia;
    // avançar caracteres inuteis
    while ((cadeia[i] == ' ') ||
           (cadeia[i] == '\n') || 
           (cadeia[i] == '\t') ||
           (cadeia[i] == '\r')) 
        i++;

    char c = cadeia[i];
    lex->conteudo[0] = c;
    // Tem chance de c == '\0' (fim da linha)
    if (c == '\0') {
        lex->indice_cadeia = i;
        lex->token = FIMDAPILHA;
        lex->conteudo[0] = '$';
        lex->conteudo[1] = '\0';
        return i;
    }
    // Token de ID / KW
    // [a-zA-Z_][a-zA-Z_0-9]*
    if (((c >= 'a') && (c <= 'z')) || 
        ((c >= 'A') && (c <= 'Z')) || 
        (c == '_')) {
        lex->indice_cadeia = i;
        // Consumir os caracteres da cadeia
        int j = 0;
        for (/*lembrando que esta parte é optativa, noob*/;
            (((cadeia[i] >= 'a') && (cadeia[i] <= 'z')) || // alfabético
            ((cadeia[i] >= 'A') && (cadeia[i] <= 'Z')) || // alfabetico
            (cadeia[i] == '_') || // underline
            ((cadeia[i] >= '0') && (cadeia[i] <= '9'))); // numérico
            lex->conteudo[j++] = cadeia[i++]); // consumo

        lex->conteudo[j] = '\0';
        lex->token = KW_ou_ID(lex->conteudo);
        return i;
    }

    // Token de NUMERO
    // [0-9][0-9]*
    if ((c >= '0') && (c <= '9')) {
        lex->indice_cadeia = i;
        // Consumir os caracteres da cadeia
        int j = 0;
        for (/*lembrando que esta parte é optativa, noob*/;
            ((cadeia[i] >= '0') && (cadeia[i] <= '9')); // numérico
            lex->conteudo[j++] = cadeia[i++]); // consumo

        lex->conteudo[j] = '\0';
        lex->token = NUM;
        return i;
    }
    // Símbolo especial [;=]
    switch (c) {
        case ';': 
            lex->token = PEV; 
            i++;
            lex->conteudo[1] = '\0';
            return i;
        case '=': 
            lex->token = IGUAL; 
            i++; 
            lex->conteudo[1] = '\0'; 
            return i;
        default:
            lex->token = ERRO;
            lex->conteudo[0] = '#';
            lex->conteudo[1] = '\0';
        return i;
    }
}
#pragma endregion
#pragma region ANALISADOR SINTATICO
// avanca: vai funcionar se não tiver nenhum erro 
// (os tokens esperado e encontrado casarem)
void avanca(char cadeia[], int* indice_cadeia, Lexema* lex) {
    *indice_cadeia = get_next_token(cadeia, *indice_cadeia, lex);
    // avanco do indice
}

// Confere o token (pra ser usado como estratégia LookAhead)
Terminal espia(Lexema* lex) {
    return lex->token;
}

bool expect(Terminal token_esperado, 
            char cadeia[], 
            int* indice_cadeia, 
            Lexema* lex) {
    if (lex->token == token_esperado) {
        avanca(cadeia, indice_cadeia, lex);
        return true; // deu tudo certo
    }
    fprintf(stderr, "ERRO: esperado <%s> recebido <%s>", Terminal2Str
    (token_esperado),
    Terminal2Str(lex->token));
    // atualizar para Terminal2Str()..
    return false;
}

// Protótipos ou máscaras
bool S(char[], int*, Lexema*);
bool L(char[], int*, Lexema*);
bool E(char[], int*, Lexema*);
/*
* S → BEGIN S L
S → IF E THEN S ELSE S
S → PRINT ID
L → ; S L
L → END
E → ID = NUM
*/
/*
* S → BEGIN S L
S → IF E THEN S ELSE S
S → PRINT ID
*/

bool S(char cadeia[], int* indice_da_cadeia, Lexema* lex) {
    switch (espia(lex)) { // qual é o token?
        case BEGIN: // S → BEGIN S L
            if (!expect(BEGIN, cadeia, indice_da_cadeia, lex)) {
                // esperado BEGIN, recebido X
                fprintf(stderr, "ERRO: esperado BEGIN recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }

            if (!S(cadeia, indice_da_cadeia, lex)) {
                // esperado S, recebido X
                fprintf(stderr, "ERRO: esperado S() recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }

            if (!L(cadeia, indice_da_cadeia, lex)) {
                // Esperado L, recebido X
                fprintf(stderr, "ERRO: esperado L() recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }
            return true;
            break;
        case IF: // S → IF E THEN S ELSE S
            if (!expect(IF, cadeia, indice_da_cadeia, lex)) {
                // esperado IF, recebido X
                fprintf(stderr, "IF: esperado BEGIN recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }

            if (!E(cadeia, indice_da_cadeia, lex)) {
                // esperado E, recebido X
                fprintf(stderr, "ERRO: esperado E() recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }
            if (!expect(THEN, cadeia, indice_da_cadeia, lex)) {
                // esperado THEN, recebido X
                fprintf(stderr, "ERRO: esperado THEN recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }

            if (!S(cadeia, indice_da_cadeia, lex)) {
                // esperado S, recebido X
                fprintf(stderr, "ERRO: esperado S() recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }

            if (!expect(ELSE, cadeia, indice_da_cadeia, lex)) {
                // esperado ELSE, recebido X
                fprintf(stderr, "ERRO: esperado ELSE recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }

            if (!S(cadeia, indice_da_cadeia, lex)) {
                // esperado S, recebido X
                fprintf(stderr, "ERRO: esperado S() recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }
            return true;
            break;
        case PRINT: // S → PRINT ID
            if (!expect(PRINT, cadeia, indice_da_cadeia, lex)) {
                // esperado PRINT, recebido X
                fprintf(stderr, "ERRO: esperado PRINT recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }

            if (!expect(ID, cadeia, indice_da_cadeia, lex)) {
                // esperado ID, recebido X
                fprintf(stderr, "ERRO: esperado ID recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }
            return true;
            break;
        default:
            fprintf(stderr, "ERRO: esperado {BEGIN, IF, PRINT}, recebido <%s>",
                Terminal2Str(espia(lex)));
            return false;
    }
}

// L → ; S L
// L → END
bool L(char cadeia[], int* indice_da_cadeia, Lexema* lex) {
    switch (espia(lex)) { // qual é o token?
        case PEV: //  L → ; S L
            if (!expect(PEV, cadeia, indice_da_cadeia, lex)) {
                // ERRO
                fprintf(stderr, "ERRO: esperado ; recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }

            if (!S(cadeia, indice_da_cadeia, lex)) {
                // ERRO
                fprintf(stderr, "ERRO: esperado S() recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }

            if (!L(cadeia, indice_da_cadeia, lex)) {
                // ERRO
                fprintf(stderr, "ERRO: esperado L() recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }
            return true;
            break;
        case END: // L → END
            if (!expect(END, cadeia, indice_da_cadeia, lex)) {
                // ERRO
                fprintf(stderr, "ERRO: esperado END recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }
            return true;
            break;
        default:
            fprintf(stderr, "ERRO: esperado ; ou END, recebido <%s>",
                Terminal2Str(espia(lex)));
            return false;
    }
}

//  E → ID = NUM
bool E(char cadeia[], int* indice_da_cadeia, Lexema* lex) {
    switch (espia(lex)) { // qual é o token?
        case ID: //  E → ID = NUM
            if (!expect(ID, cadeia, indice_da_cadeia, lex)) {
                // ERRO
                fprintf(stderr, "ERRO: esperado ID recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }

            if (!expect(IGUAL, cadeia, indice_da_cadeia, lex)) {
                // ERRO
                fprintf(stderr, "ERRO: esperado = recebido <%s>",
                    Terminal2Str(lex->token));
                return false;
            }

            if (!expect(NUM, cadeia, indice_da_cadeia, lex)) {
                // ERRO
                fprintf(stderr, "\nERRO: esperado NUM, recebido <%s>\n",
                    Terminal2Str(espia(lex)));
                return false;
            }
            return true;
            break;
        default:
            fprintf(stderr, "\nERRO: esperado ID, recebido <%s>\n",
                Terminal2Str(espia(lex)));
            return false;
    }
}
#pragma endregion

int main() {
    char programa[1000];
    strcpy(programa, "BEGIN PRINT X; IF x = 10 THEN PRINT y ELSE PRINT z END");
    Lexema lex;
    int i = 0;
    avanca(programa, &i, &lex);
    bool ok = S(programa, &i, &lex) && (espia(&lex) == FIMDAPILHA);
    if (ok) printf("\nACEITA\n");
    else printf("\nREJEITA\n");
    return 0;
}
/*
* Exercício 1: Seria possível programar um analisador sintático 
descendente recursivo
* para a gramática G abaixo? Justifique.
*
* G = ({exp, soma, termo, mult, fator},
*      {(,),+,-,*,/,NUM}, R, exp), sendo R:
*
* exp -> exp soma termo | termo
* soma -> + | -
* termo -> termo mult fator | fator
* mult -> * | /
* fator -> (exp) | NUM
*
* Resp.:
* bool exp(....){
* ...
* if(!exp(...)){
* ...
* return false;
* }
* ...
* }
*
* Exercício 2: Considere a gramática G' =
*  ({exp, soma, termo, mult, fator},
*      {(,),+,-,*,/,NUM}, R, exp), sendo R:
*
* exp -> termo {soma termo}
* soma -> + | -
* termo -> fator {mult fator}
* mult -> * | /
* fator -> (exp) | NUM
*
* Considerando que {.} simboliza a repetição ou ausência de um 
conteúdo.
*
* É possível programar um ASDR para G' considerando estruturas de 
repetição
* na presença de {.}? Caso positivo, programe um ASDR para reconhecer L
(G').
*/