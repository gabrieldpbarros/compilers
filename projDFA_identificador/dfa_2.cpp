#include <cstdlib>
#include <cstdio>
#include <cstring>

/*
-------------------------- Expressão regular simulada --------------------------

r = [A-Za-z_]([A-Za-z0-9]*)

ER para reconhecimento de identificadores nas linguagens C/C++/Java/C#/Pascal/etc
*/

// Estados do autômato
typedef enum {Q0, Q1, Q2, QLixo} Estado;

typedef struct {
    char categoria[50];
    char lexema[64];
    char traco[100];
} Token;

// Q0 -> Q1
bool eh_alfabetico(char c) {
    if (((c >= 'A') && (c <= 'Z')) || // [A-Z]
        ((c >= 'a') && (c <= 'z')) || // [a-z]
        (c == '_'))                   // [_]
        return true;
    else
        return false;
}

// Q1 -> Q1
bool eh_digito(char c) {
    if ((c >= '0') && (c <= '9'))
        return true;
    else
        return false;
}

bool eh_simbolo(char c) {
    return (
        (c == '+') || 
        (c == '-') || 
        (c == '*') || 
        (c == '/') ||
        (c == '=') || 
        (c == ';'));
}

int DFA_ID(char* cadeia, bool verbose, int i_onde_comeca_a_cadeia, Token *token, int *qtde_tokens) {
    strcpy(token->categoria, "INDEFINIDO");
    strcpy(token->lexema, "");
    strcpy(token->traco, "");

    int indice_do_lexema = 0;

    char c = cadeia[i_onde_comeca_a_cadeia];
    Estado estado_do_DFA = Q0;
    strcat(token->traco, "Q0");

    if(!eh_alfabetico(c)) {
        estado_do_DFA = QLixo;
        strcat(token->traco, "->QLixo");
    } else { // O primeiro caracter certamente é alfabético, então não temos erro
        strcpy(token->categoria, "IDENTIFICADOR");
        token->lexema[indice_do_lexema++] = c;
        i_onde_comeca_a_cadeia++;
        estado_do_DFA = Q1;
        strcat(token->traco, "->Q1");

        for (/*indice_da_cadeia = 1*/;
            estado_do_DFA == Q1;
            i_onde_comeca_a_cadeia++) {
            c = cadeia[i_onde_comeca_a_cadeia];
            
            if (eh_alfabetico(c) || eh_digito(c)) {
                //estado_do_DFA = Q1;
                strcat(token->traco, "->Q1");
                token->lexema[indice_do_lexema++] = c;
            } else {
                /*
                Não podemos consumir c, pois não compõe a lógica do DFA
                */
                estado_do_DFA = Q2;
                strcat(token->traco, "->Q2");
                token->lexema[indice_do_lexema] = '\0';
                i_onde_comeca_a_cadeia--; // Estratégia look-ahead
                // Devolvemos o caracter para a acdeia para ser analisado
                // no próximo autômato
            }
        }
    }
    (*qtde_tokens)++;
    
    return i_onde_comeca_a_cadeia;
}

int DFA_Simbolo(char* cadeia, bool verbose, int i_onde_comeca_a_cadeia, Token* token, int* qtde_tokens) {
    strcpy(token->categoria, "INDEFINIDO");
    strcpy(token->lexema, "");
    strcpy(token->traco, "");

    int indice_do_lexema = 0;

    char c = cadeia[i_onde_comeca_a_cadeia];
    Estado estado_do_DFA = Q0;
    strcat(token->traco, "Q0");

    if(!eh_simbolo(c)) {
        estado_do_DFA = QLixo;
        strcat(token->traco, "->QLixo");
    } else { // O primeiro caracter certamente é alfabético, então não temos erro
        strcpy(token->categoria, "SIMBOLO");
        token->lexema[0] = c;
        token->lexema[1] = '\0';
        strcat(token->traco, "->Q1");
        i_onde_comeca_a_cadeia++;
        (*qtde_tokens)++;
    }
    
    return i_onde_comeca_a_cadeia;
}

int DFA_Numero(char* cadeia, bool verbose, int i_onde_comeca_a_cadeia, Token *token, int *qtde_tokens) {
    strcpy(token->categoria, "INDEFINIDO");
    strcpy(token->lexema, "");
    strcpy(token->traco, "");

    int indice_do_lexema = 0;

    char c = cadeia[i_onde_comeca_a_cadeia];
    Estado estado_do_DFA = Q0;
    strcat(token->traco, "Q0");

    if(!eh_digito(c)) {
        estado_do_DFA = QLixo;
        strcat(token->traco, "->QLixo");
    } else { // O primeiro caracter certamente é alfabético, então não temos erro
        estado_do_DFA = Q1;
        strcpy(token->categoria, "NUMERO");
        strcat(token->traco, "->Q1");
        token->lexema[indice_do_lexema++] = c;
        i_onde_comeca_a_cadeia++;

        for (/*indice_da_cadeia = 1*/;
            estado_do_DFA != Q2;
            i_onde_comeca_a_cadeia++) {
            c = cadeia[i_onde_comeca_a_cadeia];
            
            if (eh_digito(c)) {
                //estado_do_DFA = Q1;
                strcat(token->traco, "->Q1");
                token->lexema[indice_do_lexema++] = c;
            } else {
                estado_do_DFA = Q2;
                strcat(token->traco, "->Q2");
                token->lexema[indice_do_lexema] = '\0';
                i_onde_comeca_a_cadeia--; // Estratégia look-ahead
                // Devolvemos o caracter para cadeia para ser analisado
                // no próximo autômato
            }
        }
    }
    (*qtde_tokens)++;
    
    return i_onde_comeca_a_cadeia;
}

void consumo_de_caracter_inutil(char* cadeia, int* i) {
    while (cadeia[*i] == ' ') (*i)++;
}

void varredura(char* cadeia) {
    Token marcacoes[1000];
    int qtde_tokens = 0;

    for (int i = 0; cadeia[i] != '\0'; /*i++*/) {
        consumo_de_caracter_inutil(cadeia, &i);
        i = DFA_ID(cadeia, true, i, &marcacoes[qtde_tokens], &qtde_tokens);

        consumo_de_caracter_inutil(cadeia, &i);
        i = DFA_Simbolo(cadeia, true, i, &marcacoes[qtde_tokens], &qtde_tokens);

        consumo_de_caracter_inutil(cadeia, &i);
        i = DFA_Numero(cadeia, true, i, &marcacoes[qtde_tokens], &qtde_tokens);
    }

    for (int i = 0; i < qtde_tokens; i++) {
        if (strcmp(marcacoes[i].lexema, "") == 0) continue;
        printf("\n---------------------------------------------------\n");
        printf("(%d)-esimo token:\n", i+1);
        printf("<%s, %s>\n", marcacoes[i].categoria, marcacoes[i].lexema);
        printf("Traco: %s\n", marcacoes[i].traco);
        printf("---------------------------------------------------\n");
    }
}

int main() {
    char cadeia[1000] = "x=y+1;";
    varredura(cadeia);

    /*
    bool resposta = eh_identificador_em_C(cadeia, true);
    
    if (resposta == true) printf("Cadeia aceita!\n");
    else printf("Cadeia nao aceita\n");
    */
}