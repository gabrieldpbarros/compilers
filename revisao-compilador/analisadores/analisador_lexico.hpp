#ifndef ANALISADOR_LEXICO
#define ANALISADOR_LEXICO
#include "tokens.hpp"
#include <string_view>

namespace analisador_lexico {
    bool eh_alfabetico(char caractere);
    bool eh_numero(char caractere);
    bool eh_simbolo(char caractere);
    bool eh_ponto_final(char caractere);
    bool eh_underscore(char caractere);
    bool eh_inutil(char caractere);
    Token get_next_token(std::string_view cadeia, int &indice_atual, Lexema* lexema);
}

#endif