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

bool eh_identificador_em_C(char* cadeia, bool verbose) {
    char lexema[1000] = "",
        traco[1000] = "";

    int indice_da_cadeia = 0,
        indice_do_lexema = 0;

    char c = cadeia[indice_da_cadeia];
    Estado estado_do_DFA = Q0;
    strcat(traco, "Q0");

    if(!eh_alfabetico(c)) {
        estado_do_DFA = QLixo;
        strcat(traco, "->QLixo");
        lexema[0] = '\0';
    } else { // O primeiro caracter certamente é alfabético, então não temos erro
        lexema[indice_do_lexema++] = c;
        estado_do_DFA = Q1;
        strcat(traco, "->Q1");

        for (indice_da_cadeia = 1; estado_do_DFA == Q1; indice_da_cadeia++) {
            c = cadeia[indice_da_cadeia];
            
            if (eh_alfabetico(c) || eh_digito(c)) {
                estado_do_DFA = Q1;
                strcat(traco, "->Q1");
                lexema[indice_do_lexema++] = c;
            } else {
                estado_do_DFA = Q2;
                strcat(traco, "->Q2");
                lexema[indice_do_lexema] = '\0';
            }
        }
    }
    if ((c == '\0') && (indice_do_lexema > 0)) {
        if (verbose)
            printf("Lexema encontrado: %s\n", lexema);
    } else {
        estado_do_DFA = QLixo;
        if (verbose)
            printf("Possivel lexema encontrado: %s\nCaracter problematico: %c\n",
            lexema, c);
    }

    printf("Traco do consumo: %s\n", traco);

    if (estado_do_DFA == QLixo) return false;
    else if (estado_do_DFA == Q2) return true;

    return false;
}

int main() {
    char cadeia[1000] = "abc123";

    bool resposta = eh_identificador_em_C(cadeia, true);

    if (resposta == true) printf("Cadeia aceita!\n");
    else printf("Cadeia nao aceita\n");
}