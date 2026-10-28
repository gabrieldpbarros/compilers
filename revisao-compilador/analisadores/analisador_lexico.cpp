#include "analisador_lexico.hpp"
#include "tokens.hpp"

namespace analisador_lexico {
    bool eh_alfabetico(char caractere) {
        if ((caractere >= 'a' && caractere <= 'z') ||
            (caractere >= 'A' && caractere <= 'Z'))
            return true;
        return false;
    }

    bool eh_numero(char caractere) {
        if(caractere >= '0' && caractere <= '9') return true;
        return false;
    }

    bool eh_simbolo(char caractere) {
        if(caractere == '+' ||
           caractere == '-' ||
           caractere == '*' ||
           caractere == '/' ||
           caractere == ';') 
            return true;
        return false;
    }

    int get_next_token(char* cadeia, int indice_atual, Token* lexema) {

    }
}