#ifndef ANALISADOR_LEXICO
#define ANALISADOR_LEXICO
#include "tokens.hpp"
#include <string_view>

namespace analisador_lexico {
    bool eh_sinalizador_string(char caractere);
    bool eh_caractere_string(char caractere);
    bool eh_alfabetico(char caractere);
    bool eh_numero(char caractere);
    bool eh_simbolo(char caractere);
    bool eh_ponto_final(char caractere);
    bool eh_underscore(char caractere);
    bool eh_inutil(char caractere);
    bool eh_fim(int tamanho_cadeia, int &indice_atual);
    std::string_view token_to_string(Token token);
    void _print_token(Token token);
    void check_reserved_words(Lexema* lexema);
    void check_simbols(Lexema* lexema);
    Token get_next_token(std::string_view cadeia, int &indice_atual, Lexema* lexema);
}

#endif